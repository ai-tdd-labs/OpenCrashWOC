"""
Extract local variable information (names, types, register assignments) from the
Crash Bandicoot alpha NGC DWARF dump.

Parses function signatures and bodies to extract:
  - Function name
  - Parameters with types and register assignments
  - Local variables with types and register assignments

Output: JSON map keyed by function name with params/locals arrays.
"""

import re
import json
import sys
from collections import OrderedDict
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[3]
DUMP_PATH = str((REPO_ROOT / "code" / "src" / "dump_alphaNGCport_DWARF.txt").resolve())
OUTPUT_PATH = str((REPO_ROOT / "analysis" / "ghidra" / "maps" / "gc_locals_map.json").resolve())

# Source file path prefixes to INCLUDE (game code and Nu2 engine)
INCLUDE_PREFIXES = [
    "c:/source/crashwoc/code/gamecode/",
    "c:/source/crashwoc/code/gamelib/",
    "c:/source/crashwoc/code/nu3dx/",
    "c:/source/crashwoc/code/nucore/",
    "c:/source/crashwoc/code/numath/",
    "c:/source/crashwoc/code/nuxbox/",
    "c:/source/crashwoc/code/system/",
]

# Explicit EXCLUDE patterns (Nintendo SDK, C library, Metrowerks)
EXCLUDE_PATTERNS = [
    "dolphin/build/libraries/",
    "dolphin/include/",
    "metrowerks/",
    "msl/",
    "powerpc_eabi_support/",
    "/dolphin\\build\\",  # Windows-style paths
]


def should_include_file(source_file):
    """Check if a source file should be included based on path prefixes."""
    sf_lower = source_file.lower().replace("\\", "/")
    for excl in EXCLUDE_PATTERNS:
        if excl.lower() in sf_lower:
            return False
    for prefix in INCLUDE_PREFIXES:
        if sf_lower.startswith(prefix.lower()):
            return True
    return False


def make_relative_path(source_file):
    """Convert absolute source path to relative path from code root."""
    # Normalize to forward slashes
    sf = source_file.replace("\\", "/")
    # Find 'code/' prefix and strip everything before it
    idx = sf.lower().find("/code/")
    if idx >= 0:
        return sf[idx + 1:]  # returns "code/gamecode/ai.c"
    return sf


def parse_register(comment):
    """Extract register from a comment like '// r30', '// f1', '// r1+0x8'."""
    if comment is None:
        return None
    m = re.match(r'(r\d+(?:\+0x[0-9a-fA-F]+)?|f\d+)', comment.strip())
    if m:
        return m.group(1)
    return None


def parse_param_register(comment):
    """Extract register from parameter comment like '/* r30 */' or '/* f1 */'."""
    if comment is None:
        return None
    m = re.match(r'(r\d+(?:\+0x[0-9a-fA-F]+)?|f\d+)', comment.strip())
    if m:
        return m.group(1)
    return None


def extract_functions(lines):
    """
    Main extraction logic. Parses the DWARF dump line by line, tracking
    compile units and extracting function signatures + local variables.
    """
    results = OrderedDict()
    current_source = None
    n = len(lines)
    i = 0

    while i < n:
        line = lines[i]

        # Track compile unit changes
        cu_match = re.match(r'^// Compile unit:\s*(.+)$', line)
        if cu_match:
            current_source = cu_match.group(1).strip()
            i += 1
            continue

        # Skip files we don't care about
        if current_source and not should_include_file(current_source):
            i += 1
            continue

        # Look for function definitions - lines that contain '(' and end with ') {' or ') {}'
        # But we need to handle multi-line signatures with inline struct defs
        # A function definition starts at the top level (not indented with spaces for locals)
        # and contains a '(' character

        # Quick check: does this line look like it could be a function start?
        # Function lines are NOT indented (no leading whitespace), and contain '('
        stripped = line.rstrip()
        if not stripped or stripped.startswith('//') or stripped.startswith(' ') or stripped.startswith('\t'):
            i += 1
            continue

        # Skip typedefs (they look like function declarations but aren't)
        if stripped.startswith('typedef '):
            i += 1
            continue

        # Skip global variable declarations with address annotations
        # e.g., "static float buglight_fade : 0x803C3420; // size: 0x4"
        if ': 0x' in stripped and '(' not in stripped:
            i += 1
            continue

        # Check for struct/enum definitions at top level (not functions)
        if stripped.startswith('struct ') and '{' in stripped and '(' not in stripped:
            i += 1
            continue
        if stripped.startswith('enum ') and '{' in stripped:
            i += 1
            continue

        # Check if this could be a function signature
        if '(' not in stripped:
            i += 1
            continue

        # Accumulate multi-line function signature
        # The signature ends when we see ') {}' or ') {' at the end of a line
        # We need to handle inline struct definitions in parameters
        sig_lines = [stripped]
        sig_end = i
        brace_depth = 0

        # Count braces to handle inline structs
        for ch in stripped:
            if ch == '{':
                brace_depth += 1
            elif ch == '}':
                brace_depth -= 1

        # Check if signature is complete on this line
        sig_complete = False
        if stripped.endswith(') {}') or stripped.endswith(') {}'):
            if brace_depth <= 0:
                sig_complete = True
                has_body = False
        elif stripped.endswith(') {'):
            if brace_depth <= 1:  # The trailing '{' is the function body brace
                sig_complete = True
                has_body = True

        # If not complete, accumulate more lines
        if not sig_complete:
            j = i + 1
            while j < n and not sig_complete:
                next_line = lines[j].rstrip()
                sig_lines.append(next_line)
                for ch in next_line:
                    if ch == '{':
                        brace_depth += 1
                    elif ch == '}':
                        brace_depth -= 1

                if next_line.endswith(') {}'):
                    if brace_depth <= 0:
                        sig_complete = True
                        has_body = False
                        sig_end = j
                elif next_line.endswith(') {'):
                    if brace_depth <= 1:
                        sig_complete = True
                        has_body = True
                        sig_end = j

                # Safety: if we've gone too far, this isn't a function
                # Note: some functions have very large inline struct params (300+ lines)
                if j - i > 500:
                    break
                j += 1

        if not sig_complete:
            i += 1
            continue

        full_sig = ' '.join(sig_lines)

        # Parse the function name and parameters from the signature
        func_info = parse_function_signature(full_sig)
        if func_info is None:
            i = sig_end + 1
            continue

        func_name, params = func_info

        # Skip if the function has no useful info (no params with registers AND no body)
        # We'll check for locals below

        locals_list = []

        if has_body:
            # Parse the function body for local variables
            j = sig_end + 1
            body_brace_depth = 1  # We're inside the function body '{'

            while j < n:
                body_line = lines[j].rstrip()

                # Track braces for inline struct types in locals
                for ch in body_line:
                    if ch == '{':
                        body_brace_depth += 1
                    elif ch == '}':
                        body_brace_depth -= 1

                # If we've closed the function body brace, we're done
                if body_brace_depth <= 0:
                    sig_end = j
                    break

                # Skip comments like "// Local variables" and "// Labels"
                stripped_body = body_line.strip()
                if stripped_body.startswith('// '):
                    j += 1
                    continue

                # Skip label lines like "Loop: // 0x8000520C"
                if re.match(r'^\s+\w+:\s*//', stripped_body):
                    j += 1
                    continue

                # Skip struct member lines (inside inline struct definitions)
                # These have "// offset 0x..." or "// total size:"
                if '// offset 0x' in body_line or '// total size:' in body_line:
                    j += 1
                    continue

                # Skip lines that are just struct/closing braces
                if stripped_body in ('{', '}', '};', 'struct {'):
                    j += 1
                    continue

                # Skip lines that start struct definitions within locals
                if re.match(r'^\s+struct\s*\{', body_line) or re.match(r'^\s+struct\s+\w+\s*\{', body_line):
                    j += 1
                    continue

                # Parse local variable line
                # Format: "    type name; // rN" or "    type name;" (no register)
                # The closing of an inline struct: "    } * varname; // rN"
                local_info = parse_local_variable(body_line)
                if local_info:
                    locals_list.append(local_info)

                j += 1

            sig_end = max(sig_end, j)

        # Deduplicate locals (DWARF often lists them twice for different scopes)
        locals_deduped = deduplicate_locals(locals_list)
        params_deduped = deduplicate_params(params)

        # Only include functions that have at least some useful info
        has_param_registers = any(p.get('register') for p in params_deduped)
        has_local_registers = any(l.get('register') for l in locals_deduped)
        has_any_locals = len(locals_deduped) > 0

        if has_param_registers or has_local_registers or has_any_locals:
            # Handle duplicate function names by appending source file info
            key = func_name
            if key in results:
                # If same source file, merge/skip; if different, make unique
                existing_source = results[key].get('source_file', '')
                new_source = make_relative_path(current_source) if current_source else ''
                if existing_source != new_source:
                    # Different source files - make key unique
                    key = f"{func_name}__{new_source.replace('/', '_').replace('.', '_')}"
                else:
                    # Same source file - keep whichever has more info
                    existing_total = len(results[key].get('params', [])) + len(results[key].get('locals', []))
                    new_total = len(params_deduped) + len(locals_deduped)
                    if new_total <= existing_total:
                        i = sig_end + 1
                        continue

            entry = OrderedDict()
            entry['source_file'] = make_relative_path(current_source) if current_source else ''
            entry['params'] = params_deduped
            entry['locals'] = locals_deduped
            results[key] = entry

        i = sig_end + 1

    return results


def parse_function_signature(sig):
    """
    Parse a function signature string to extract function name and parameters.

    The signature format is:
      [static] [inline] returntype funcname(paramtype paramname /* rN */, ...) {

    Parameters can have inline struct definitions spanning the signature.
    """
    # Remove trailing ' {}' or ' {'
    sig_clean = sig.rstrip()
    if sig_clean.endswith(' {}'):
        sig_clean = sig_clean[:-3]
    elif sig_clean.endswith(' {'):
        sig_clean = sig_clean[:-2]

    sig_clean = sig_clean.rstrip()

    # The signature ends with ')'
    if not sig_clean.endswith(')'):
        return None

    # Find the matching '(' for the outermost parameter list
    # We need to handle nested braces from inline structs
    depth = 0
    paren_start = -1
    for idx in range(len(sig_clean) - 1, -1, -1):
        ch = sig_clean[idx]
        if ch == ')':
            depth += 1
        elif ch == '(':
            depth -= 1
            if depth == 0:
                paren_start = idx
                break

    if paren_start < 0:
        return None

    # Everything before '(' is the return type + function name
    prefix = sig_clean[:paren_start].strip()
    params_str = sig_clean[paren_start + 1:-1]  # Remove outer parens

    # Extract function name (last word in prefix)
    # Handle cases like "static int FuncName", "void FuncName", "struct type * FuncName"
    # The function name is the last identifier before '('
    # Handle pointer returns: "type *FuncName" or "type * FuncName"
    parts = prefix.split()
    if not parts:
        return None

    func_name = parts[-1]
    # Remove leading * from function name (pointer return type)
    func_name = func_name.lstrip('*')

    if not func_name or not re.match(r'^[a-zA-Z_]\w*$', func_name):
        return None

    # Parse parameters
    params = parse_parameters(params_str)

    return func_name, params


def parse_parameters(params_str):
    """
    Parse the parameter list string, handling inline struct definitions.
    Returns list of dicts with name, type, register.
    """
    params = []

    if not params_str.strip() or params_str.strip() == 'void':
        return params

    # Split parameters at top-level commas (not inside braces/parens)
    param_parts = split_at_top_level_commas(params_str)

    for part in param_parts:
        part = part.strip()
        if not part:
            continue

        param_info = parse_single_param(part)
        if param_info:
            params.append(param_info)

    return params


def split_at_top_level_commas(s):
    """Split string at commas that are not inside braces or parens."""
    parts = []
    depth = 0
    current = []
    for ch in s:
        if ch in '({':
            depth += 1
            current.append(ch)
        elif ch in ')}':
            depth -= 1
            current.append(ch)
        elif ch == ',' and depth == 0:
            parts.append(''.join(current))
            current = []
        else:
            current.append(ch)
    if current:
        parts.append(''.join(current))
    return parts


def parse_single_param(param_str):
    """
    Parse a single parameter like:
      "int n /* r4 */"
      "struct creature_s * c /* r30 */"
      "struct { ... } * cam /* r31 */"
      "float lateral /* f31 */"
    """
    param_str = param_str.strip()

    # Extract register comment /* rN */
    reg = None
    reg_match = re.search(r'/\*\s*(r\d+(?:\+0x[0-9a-fA-F]+)?|f\d+)\s*\*/', param_str)
    if reg_match:
        reg = reg_match.group(1)
        # Remove the comment from the string
        param_str = param_str[:reg_match.start()].rstrip() + param_str[reg_match.end():]
        param_str = param_str.strip()

    if not param_str:
        return None

    # Now parse type and name
    # Handle inline struct: "struct { ... } * name" or "struct { ... } name"
    # First, collapse inline struct defs
    collapsed = collapse_inline_structs(param_str)

    # Split into type and name
    # The name is the last identifier token
    # Handle pointers: "type * name", "type ** name"
    tokens = collapsed.split()
    if not tokens:
        return None

    name = tokens[-1]
    # Remove array suffixes from name
    array_match = re.match(r'^(\w+)(\[.*\])$', name)
    array_suffix = ''
    if array_match:
        name = array_match.group(1)
        array_suffix = array_match.group(2)

    if not re.match(r'^[a-zA-Z_]\w*$', name):
        return None

    type_str = ' '.join(tokens[:-1])
    if array_suffix:
        type_str = type_str + ' ' + array_suffix

    # Clean up type string
    type_str = type_str.strip()
    if not type_str:
        return None

    result = OrderedDict()
    result['name'] = name
    result['type'] = type_str
    if reg:
        result['register'] = reg

    return result


def collapse_inline_structs(s):
    """
    Collapse inline struct definitions to just 'struct {...}' for type extraction.
    E.g., "struct { int x; int y; } * foo" -> "struct {...} * foo"
    """
    result = []
    i = 0
    while i < len(s):
        if s[i] == '{':
            depth = 1
            i += 1
            while i < len(s) and depth > 0:
                if s[i] == '{':
                    depth += 1
                elif s[i] == '}':
                    depth -= 1
                i += 1
            result.append('{...}')
        else:
            result.append(s[i])
            i += 1
    return ''.join(result)


def parse_local_variable(line):
    """
    Parse a local variable line like:
      "    int i; // r30"
      "    float dx; // f1"
      "    char name[16]; // r1+0x8"
      "    int i;"  (no register)
      "    } * ai; // r28"  (closing of inline struct)
    """
    line = line.rstrip()
    stripped = line.strip()

    if not stripped:
        return None

    # Skip lines that don't look like variable declarations
    if stripped.startswith('//'):
        return None
    if stripped.startswith('{'):
        return None

    # Must end with ';' optionally followed by a comment
    # Pattern: "type name; // register" or "type name;" or "} * name; // register"
    # Also handle: "} * name; // rN"

    # Extract register comment
    reg = None
    reg_match = re.search(r'//\s*(r\d+(?:\+0x[0-9a-fA-F]+)?|f\d+)\s*$', stripped)
    if reg_match:
        reg = reg_match.group(1)
        stripped = stripped[:reg_match.start()].rstrip()

    # Remove trailing semicolon
    if not stripped.endswith(';'):
        return None
    stripped = stripped[:-1].strip()

    if not stripped:
        return None

    # Handle closing of inline struct: "} * name" or "} name"
    if stripped.startswith('}'):
        # This is the end of an inline struct type definition
        remainder = stripped[1:].strip()
        # remainder is like "* ai" or "ai"
        tokens = remainder.split()
        if not tokens:
            return None
        name = tokens[-1]
        array_match = re.match(r'^(\w+)(\[.*\])$', name)
        array_suffix = ''
        if array_match:
            name = array_match.group(1)
            array_suffix = array_match.group(2)

        if not re.match(r'^[a-zA-Z_]\w*$', name):
            return None

        ptr_part = ' '.join(tokens[:-1])
        type_str = 'struct {...} ' + ptr_part
        if array_suffix:
            type_str = type_str.strip() + ' ' + array_suffix
        type_str = type_str.strip()

        result = OrderedDict()
        result['name'] = name
        result['type'] = type_str
        if reg:
            result['register'] = reg
        return result

    # Normal variable declaration: "type name" possibly with array brackets
    tokens = stripped.split()
    if len(tokens) < 2:
        return None

    name = tokens[-1]
    array_match = re.match(r'^(\w+)(\[.*\])$', name)
    array_suffix = ''
    if array_match:
        name = array_match.group(1)
        array_suffix = array_match.group(2)

    if not re.match(r'^[a-zA-Z_]\w*$', name):
        return None

    type_str = ' '.join(tokens[:-1])
    if array_suffix:
        type_str = type_str + ' ' + array_suffix
    type_str = type_str.strip()

    if not type_str:
        return None

    result = OrderedDict()
    result['name'] = name
    result['type'] = type_str
    if reg:
        result['register'] = reg
    return result


def deduplicate_locals(locals_list):
    """
    Deduplicate local variables. DWARF often lists the same variable twice
    (for different lexical scopes). Keep the first occurrence with a register,
    or the first occurrence if none have registers.
    """
    seen = {}
    deduped = []
    for local in locals_list:
        key = local['name']
        if key in seen:
            # If existing entry has no register but this one does, replace
            existing_idx = seen[key]
            if 'register' not in deduped[existing_idx] and 'register' in local:
                deduped[existing_idx] = local
            continue
        seen[key] = len(deduped)
        deduped.append(local)
    return deduped


def deduplicate_params(params_list):
    """Deduplicate parameters (unlikely but handle it)."""
    seen = set()
    deduped = []
    for param in params_list:
        key = param['name']
        if key in seen:
            continue
        seen.add(key)
        deduped.append(param)
    return deduped


def generate_report(results):
    """Generate summary statistics."""
    total_functions = len(results)
    total_params = 0
    total_locals = 0
    total_params_with_reg = 0
    total_locals_with_reg = 0
    by_source = OrderedDict()

    for func_name, info in results.items():
        source = info.get('source_file', 'unknown')
        if source not in by_source:
            by_source[source] = {'functions': 0, 'params': 0, 'locals': 0}
        by_source[source]['functions'] += 1

        n_params = len(info.get('params', []))
        n_locals = len(info.get('locals', []))
        total_params += n_params
        total_locals += n_locals
        by_source[source]['params'] += n_params
        by_source[source]['locals'] += n_locals

        total_params_with_reg += sum(1 for p in info.get('params', []) if 'register' in p)
        total_locals_with_reg += sum(1 for l in info.get('locals', []) if 'register' in l)

    print("=" * 70)
    print("DWARF Local Variable Extraction Report")
    print("=" * 70)
    print(f"Total functions with local info:     {total_functions}")
    print(f"Total parameters extracted:          {total_params}")
    print(f"  - with register assignments:       {total_params_with_reg}")
    print(f"Total local variables extracted:      {total_locals}")
    print(f"  - with register assignments:       {total_locals_with_reg}")
    print(f"Total variables (params+locals):     {total_params + total_locals}")
    print()
    print("Breakdown by source file:")
    print("-" * 70)

    # Sort by source file name
    for source in sorted(by_source.keys()):
        counts = by_source[source]
        print(f"  {source}")
        print(f"    functions: {counts['functions']}, params: {counts['params']}, locals: {counts['locals']}")
    print()


def main():
    print(f"Reading DWARF dump: {DUMP_PATH}")
    with open(DUMP_PATH, 'r', encoding='utf-8', errors='replace') as f:
        lines = f.readlines()
    print(f"Read {len(lines)} lines")

    # Strip trailing newlines
    lines = [line.rstrip('\n').rstrip('\r') for line in lines]

    print("Extracting function/local variable information...")
    results = extract_functions(lines)

    print(f"Writing output to: {OUTPUT_PATH}")
    with open(OUTPUT_PATH, 'w', encoding='utf-8') as f:
        json.dump(results, f, indent=2, ensure_ascii=False)
    print(f"Written {len(results)} functions to JSON")

    generate_report(results)

    return 0


if __name__ == '__main__':
    sys.exit(main())
