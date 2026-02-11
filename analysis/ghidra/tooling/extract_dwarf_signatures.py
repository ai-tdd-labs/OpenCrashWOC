#!/usr/bin/env python3
"""
Extract function signatures from Crash Bandicoot alpha NGC DWARF dump.

Parses the DWARF dump to extract:
  - Function name
  - Return type
  - Parameter list (name + type)
  - Source file

Merges with existing gc_params_map.json and writes gc_signatures_map.json.
"""

import json
import re
import os
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[3]
DWARF_DUMP = str((REPO_ROOT / "code" / "src" / "dump_alphaNGCport_DWARF.txt").resolve())
PARAMS_MAP = str((REPO_ROOT / "analysis" / "ghidra" / "maps" / "gc_params_map.json").resolve())
OUTPUT_MAP = str((REPO_ROOT / "analysis" / "ghidra" / "maps" / "gc_signatures_map.json").resolve())

# Nintendo SDK / standard library prefixes to skip
SKIP_PREFIXES = (
    "GX", "OS", "VI", "PAD", "DVD", "AR", "AI", "SI", "EXI", "DSP",
    "Card", "CARD", "GBA", "__GBA",
    "IDirect3D", "D3D", "D3DSurface", "D3DTexture", "D3DDevice",
    "D3DPalette", "D3DVertexBuffer", "D3DIndexBuffer", "D3DBaseTexture",
    "D3DResource", "D3DPixelShader", "D3DPushBuffer",
    # C standard library
    "printf", "sprintf", "fprintf", "snprintf", "vprintf", "vsprintf",
    "vfprintf", "vsnprintf", "scanf", "sscanf", "fscanf",
    "malloc", "calloc", "realloc", "free",
    "memcpy", "memset", "memmove", "memcmp",
    "strcpy", "strncpy", "strcat", "strncat", "strcmp", "strncmp",
    "strlen", "strstr", "strchr", "strrchr", "strtol", "strtoul",
    "strtod", "strtof", "atoi", "atof", "atol",
    "abs", "labs", "fabs",
    "fopen", "fclose", "fread", "fwrite", "fseek", "ftell",
    "exit", "abort", "atexit",
    "qsort", "bsearch",
    "rand", "srand",
    "isalpha", "isdigit", "isalnum", "isspace", "isupper", "islower",
    "toupper", "tolower",
    # Metrowerks CW runtime
    "__ct__", "__dt__", "__nw__", "__dl__", "__as__",
    "__ppc_eabi", "__init_", "__destroy_",
    # Dolphin SDK internal
    "DEMO", "__DEMO",
    # DSP
    "DSP",
    # MTX (Dolphin math library)
    "MTX", "VEC", "PSM", "C_MTX", "C_VEC",
    # More Dolphin
    "THPPlayer", "THP",
    "PPCMt", "PPCMf",
    "__flush", "__sync",
)

# Full function names to skip
SKIP_EXACT = {
    "main", "__start", "__init_user", "__init_cpp", "__fini_cpp",
    "__destroy_global_chain", "__init_hardware", "__init_data",
    "__copy_rom_section", "__init_bss_section",
}

# Source file paths indicating SDK/library code to skip
SKIP_SOURCE_PATHS = (
    r"c:\dolphin",
    r"c:\Program Files\Metrowerks",
    "dolphin/build/libraries",
    "PowerPC_EABI_Support",
)


def should_skip_function(func_name, source_file):
    """Check if a function should be skipped (SDK/stdlib)."""
    if func_name in SKIP_EXACT:
        return True

    for prefix in SKIP_PREFIXES:
        if func_name.startswith(prefix):
            return True

    # Skip anonymous/unnamed functions like F23, F25, D35, etc.
    if re.match(r'^[A-Z]\d+$', func_name):
        return True

    # Skip source files from SDK paths
    if source_file:
        source_lower = source_file.lower().replace("\\", "/")
        for skip_path in SKIP_SOURCE_PATHS:
            if skip_path.lower().replace("\\", "/") in source_lower:
                return True

    return False


def parse_param_string(param_str):
    """
    Parse a parameter string like:
      'struct nuvec_s * pos /* r28 */, int db /* r19 */'
    into a list of {'name': ..., 'type': ...} dicts.
    """
    if not param_str or param_str.strip() == "void":
        return []

    params = []
    # We need to split by commas, but be careful of nested types like
    # 'struct foo * * ppBar' and register annotations '/* r28 */'
    # Also handle anonymous structs spanning multiple lines -- those are
    # pre-collapsed by the caller.

    # First, remove register annotations /* ... */
    clean = re.sub(r'/\*[^*]*\*/', '', param_str)
    # Remove address annotations like : 0x80123456
    clean = re.sub(r'\s*:\s*0x[0-9A-Fa-f]+\s*;?', '', clean)

    # Split by top-level commas (not inside braces or parens)
    depth = 0
    current = ""
    for ch in clean:
        if ch in ('(', '{'):
            depth += 1
            current += ch
        elif ch in (')', '}'):
            depth -= 1
            current += ch
        elif ch == ',' and depth == 0:
            params.append(current.strip())
            current = ""
        else:
            current += ch
    if current.strip():
        params.append(current.strip())

    result = []
    for p in params:
        p = p.strip()
        if not p or p == "void" or p == "...":
            if p == "...":
                result.append({"name": "...", "type": "..."})
            continue

        parsed = parse_single_param(p)
        if parsed:
            result.append(parsed)

    return result


def parse_single_param(param_text):
    """
    Parse a single parameter like 'struct nuvec_s * pos' or 'int count'
    into {'name': ..., 'type': ...}.
    """
    param_text = param_text.strip()
    if not param_text:
        return None

    # Handle function pointer params: returntype (* name)(args)
    # e.g., void (* callback)(int, float)
    fp_match = re.match(
        r'^(.+?)\(\s*\*\s*(\w+)\s*\)\s*\(.*\)$', param_text
    )
    if fp_match:
        ret_type = fp_match.group(1).strip()
        name = fp_match.group(2).strip()
        return {"name": name, "type": f"{ret_type}(*)(...)"}

    # Handle array params: type name[size]
    arr_match = re.match(r'^(.+?)\s+(\w+)\s*(\[.*\])$', param_text)
    if arr_match:
        type_part = arr_match.group(1).strip()
        name = arr_match.group(2).strip()
        arr_suffix = arr_match.group(3).strip()
        return {"name": name, "type": f"{type_part} {arr_suffix}"}

    # General case: split off the last word as the name
    # But handle pointer types carefully:
    # "struct nuvec_s * pos" -> type="struct nuvec_s *", name="pos"
    # "struct nuvec_s * * ppBar" -> type="struct nuvec_s * *", name="ppBar"
    # "int count" -> type="int", name="count"
    # "unsigned int flags" -> type="unsigned int", name="flags"

    # Strategy: Find the last identifier token that's not a pointer star
    # and not a type keyword.
    tokens = param_text.split()
    if not tokens:
        return None

    if len(tokens) == 1:
        # Just a type with no name
        return {"name": "", "type": tokens[0]}

    # The last non-* token should be the name
    # Work backwards to find the parameter name
    name = tokens[-1]

    # If the last token is *, this is a return of a pointer with no name
    if name == '*':
        return {"name": "", "type": param_text}

    type_part = " ".join(tokens[:-1]).strip()

    # Clean up extra spaces
    type_part = re.sub(r'\s+', ' ', type_part)

    return {"name": name, "type": type_part}


def extract_source_file(line_num, compile_units):
    """Find which compile unit a given line belongs to."""
    result = ""
    for cu_line, cu_file in compile_units:
        if cu_line <= line_num:
            result = cu_file
        else:
            break
    return result


def normalize_source_path(path):
    """Normalize a source path to a relative project path."""
    path = path.replace("\\", "/")
    # Remove the C:/source/crashwoc/code/ prefix
    markers = ["crashwoc/code/", "source/crashwoc/code/"]
    for marker in markers:
        idx = path.lower().find(marker.lower())
        if idx >= 0:
            return path[idx + len(marker):]
    return path


def main():
    print(f"Reading DWARF dump from: {DWARF_DUMP}")
    print(f"Reading params map from: {PARAMS_MAP}")

    # Load existing params map
    with open(PARAMS_MAP, 'r', encoding='utf-8') as f:
        existing_params = json.load(f)
    print(f"Loaded {len(existing_params)} functions from existing params map")

    # First pass: find all compile unit boundaries
    print("Pass 1: Finding compile unit boundaries...")
    compile_units = []  # list of (line_num, source_file)
    cu_pattern = re.compile(r'^// Compile unit:\s*(.+)$')

    with open(DWARF_DUMP, 'r', encoding='utf-8', errors='replace') as f:
        for line_num, line in enumerate(f, 1):
            m = cu_pattern.match(line.rstrip())
            if m:
                compile_units.append((line_num, m.group(1).strip()))

    print(f"  Found {len(compile_units)} compile units")

    # Second pass: extract function signatures
    # We match lines that look like function definitions/declarations:
    #   returntype funcname(params) {
    #   returntype funcname(params) {}
    #   static returntype funcname(params) {
    #   static returntype funcname(params) {}
    # But NOT:
    #   static inline ... (skip inline SDK wrappers)
    #   typedef ... (skip typedef function types)

    print("Pass 2: Extracting function signatures...")

    # This regex matches the START of a function definition line.
    # We need to handle multi-line cases where the param list extends
    # across lines (anonymous struct params).
    #
    # Pattern: optional_static type_tokens funcname(
    func_start_pattern = re.compile(
        r'^(static\s+)?'               # optional 'static'
        r'((?:(?:unsigned|signed|short|long|const|volatile|struct|union|enum)\s+)*'  # type qualifiers
        r'[\w]+(?:\s*\*)*)'            # base type + optional pointers
        r'\s+'                          # space
        r'(\w+)'                        # function name
        r'\s*\('                        # opening paren
    )

    # Alternative for struct/pointer return types like "struct nuvec_s * FuncName("
    func_start_pattern2 = re.compile(
        r'^(static\s+)?'
        r'((?:(?:unsigned|signed|short|long|const|volatile|struct|union|enum)\s+)*'
        r'[\w]+(?:\s+\*)+)'            # type with pointer (space before *)
        r'\s+'
        r'(\w+)'                        # function name
        r'\s*\('
    )

    signatures = {}
    skipped_sdk = 0
    skipped_inline = 0
    total_found = 0
    multi_line_functions = 0

    with open(DWARF_DUMP, 'r', encoding='utf-8', errors='replace') as f:
        lines = f.readlines()

    print(f"  Loaded {len(lines)} lines")

    i = 0
    while i < len(lines):
        line = lines[i].rstrip('\n\r')

        # Skip inline functions
        if line.startswith("static inline "):
            skipped_inline += 1
            i += 1
            continue

        # Skip typedef lines
        if line.startswith("typedef "):
            i += 1
            continue

        # Try to match a function definition start
        m = func_start_pattern.match(line) or func_start_pattern2.match(line)
        if not m:
            i += 1
            continue

        static_prefix = m.group(1) or ""
        return_type = m.group(2).strip()
        func_name = m.group(3).strip()

        # Get the rest of the line after the opening paren
        after_paren_start = line.index('(', m.start(3)) + 1
        rest = line[after_paren_start:]

        # Collect the full parameter text, handling multi-line cases
        full_text = rest
        brace_depth = 0
        paren_depth = 1  # We already consumed the opening paren

        # Count parens in rest to see if we've closed them
        for ch in rest:
            if ch == '(':
                paren_depth += 1
            elif ch == ')':
                paren_depth -= 1
            elif ch == '{':
                brace_depth += 1

        # If parens aren't balanced, read more lines
        j = i + 1
        if paren_depth > 0:
            multi_line_functions += 1
            while j < len(lines) and paren_depth > 0:
                next_line = lines[j].rstrip('\n\r')
                full_text += " " + next_line
                for ch in next_line:
                    if ch == '(':
                        paren_depth += 1
                    elif ch == ')':
                        paren_depth -= 1
                    elif ch == '{':
                        brace_depth += 1
                j += 1

        # Now full_text has everything from after '(' to end
        # Extract param text: everything up to the closing ')'
        # Find the matching closing paren
        paren_depth = 1
        param_end = -1
        for ci, ch in enumerate(full_text):
            if ch == '(':
                paren_depth += 1
            elif ch == ')':
                paren_depth -= 1
                if paren_depth == 0:
                    param_end = ci
                    break

        if param_end < 0:
            i = max(i + 1, j)
            continue

        param_text = full_text[:param_end].strip()
        after_close = full_text[param_end + 1:].strip()

        # Check it ends with { or {} (it's a function definition)
        if not ('{' in after_close or after_close.startswith('{')):
            i = max(i + 1, j)
            continue

        total_found += 1

        # Determine source file
        source_file = extract_source_file(i + 1, compile_units)
        normalized_source = normalize_source_path(source_file) if source_file else ""

        # Check if we should skip this function
        if should_skip_function(func_name, source_file):
            skipped_sdk += 1
            i = max(i + 1, j)
            continue

        # Parse parameters
        params = parse_param_string(param_text)

        # Store in signatures map (last occurrence wins for duplicates,
        # but prefer definitions with parameter names over those without)
        if func_name in signatures:
            existing = signatures[func_name]
            # Prefer the version with more parameter info
            existing_has_names = any(p["name"] for p in existing.get("params", []))
            new_has_names = any(p["name"] for p in params)
            if not new_has_names and existing_has_names:
                i = max(i + 1, j)
                continue

        entry = {
            "return_type": return_type,
            "params": params,
            "source_file": normalized_source,
        }

        signatures[func_name] = entry
        i = max(i + 1, j)

    # Print statistics
    print(f"\n--- Extraction Results ---")
    print(f"Total function definitions found: {total_found}")
    print(f"Skipped inline functions: {skipped_inline}")
    print(f"Skipped SDK/stdlib functions: {skipped_sdk}")
    print(f"Multi-line function signatures: {multi_line_functions}")
    print(f"Game/engine functions extracted: {len(signatures)}")

    # Count statistics
    has_return_type = sum(1 for s in signatures.values() if s["return_type"] != "void")
    has_void_return = sum(1 for s in signatures.values() if s["return_type"] == "void")
    has_params = sum(1 for s in signatures.values() if len(s["params"]) > 0)
    has_full_info = sum(
        1 for s in signatures.values()
        if s["return_type"] and len(s["params"]) > 0 and all(p["name"] for p in s["params"])
    )

    print(f"\nReturn type breakdown:")
    print(f"  void returns: {has_void_return}")
    print(f"  non-void returns: {has_return_type}")
    print(f"  total with return type info: {len(signatures)} (all have return types)")
    print(f"\nParameter info:")
    print(f"  functions with parameters: {has_params}")
    print(f"  functions with no params: {len(signatures) - has_params}")
    print(f"  functions with full param info (names+types): {has_full_info}")

    # Compare with existing params map
    existing_names = set(existing_params.keys())
    new_names = set(signatures.keys())
    overlap = existing_names & new_names
    only_in_existing = existing_names - new_names
    only_in_new = new_names - existing_names

    print(f"\nComparison with gc_params_map.json ({len(existing_params)} functions):")
    print(f"  Functions in both maps: {len(overlap)}")
    print(f"  New functions (not in params map): {len(only_in_new)}")
    print(f"  Only in params map (not in DWARF): {len(only_in_existing)}")

    if only_in_existing:
        print(f"\n  Functions only in params map (sample of first 20):")
        for name in sorted(only_in_existing)[:20]:
            print(f"    - {name}")

    # For overlapping functions, merge: add return_type from DWARF to functions
    # that exist in the params map. Also verify param consistency.
    merge_matches = 0
    merge_mismatches = 0
    for name in overlap:
        dwarf_sig = signatures[name]
        existing_sig = existing_params[name]
        # Check if params are consistent
        dwarf_params = dwarf_sig["params"]
        existing_p = existing_sig.get("params", [])
        if len(dwarf_params) == len(existing_p):
            merge_matches += 1
        else:
            merge_mismatches += 1

    print(f"\n  Overlap parameter count match: {merge_matches}")
    print(f"  Overlap parameter count mismatch: {merge_mismatches}")

    # Collect return type distribution
    return_types = {}
    for sig in signatures.values():
        rt = sig["return_type"]
        return_types[rt] = return_types.get(rt, 0) + 1

    print(f"\nTop 20 return types:")
    for rt, count in sorted(return_types.items(), key=lambda x: -x[1])[:20]:
        print(f"  {rt}: {count}")

    # Collect source file distribution
    source_files = {}
    for sig in signatures.values():
        sf = sig["source_file"]
        if sf:
            source_files[sf] = source_files.get(sf, 0) + 1

    print(f"\nFunctions by source file:")
    for sf, count in sorted(source_files.items(), key=lambda x: -x[1]):
        print(f"  {sf}: {count}")

    # Write output
    # Sort by function name for readability
    sorted_sigs = dict(sorted(signatures.items()))

    with open(OUTPUT_MAP, 'w', encoding='utf-8') as f:
        json.dump(sorted_sigs, f, indent=2, ensure_ascii=False)

    print(f"\nWrote {len(sorted_sigs)} function signatures to: {OUTPUT_MAP}")

    # Also print some example entries
    print("\n--- Sample entries ---")
    sample_names = list(sorted_sigs.keys())[:5]
    for name in sample_names:
        sig = sorted_sigs[name]
        params_str = ", ".join(
            f"{p['type']} {p['name']}" for p in sig['params']
        )
        print(f"  {sig['return_type']} {name}({params_str})")
        print(f"    source: {sig['source_file']}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
