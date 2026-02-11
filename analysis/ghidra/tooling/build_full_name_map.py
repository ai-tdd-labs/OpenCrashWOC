#!/usr/bin/env python3
"""
Build gc_name_map_full.json by matching OpenCrashWOC source function names
to Ghidra decompiled GC retail function addresses.

Strategies:
  A. String literal matching - match unique strings from source to decompiled code
  B. Call graph patterns - match functions by their pattern of calling named functions
  C. Named callee matching - if a decompiled function calls only named funcs that
     match a source function's callees, match it

Merges results with existing gc_name_map.json (409 DWARF-based entries).
"""

import json
import re
import os
from collections import defaultdict
from pathlib import Path

BASE = str(Path(__file__).resolve().parents[3])
SRC_DIR = os.path.join(BASE, "code", "src")
RAW = os.path.join(BASE, "analysis", "ghidra", "raw")
MAPS = os.path.join(BASE, "analysis", "ghidra", "maps")

DECOMPILED_C = os.path.join(RAW, "main.dol_decompiled.c")
FUNCTIONS_JSON = os.path.join(RAW, "main.dol_functions.json")
STRINGS_TXT = os.path.join(RAW, "main.dol_strings.txt")
EXISTING_MAP = os.path.join(MAPS, "gc_name_map.json")
OUTPUT_MAP = os.path.join(MAPS, "gc_name_map_full.json")


# ============================================================================
# Step 1: Extract functions from OpenCrashWOC source
# ============================================================================

def extract_source_functions():
    """Extract function names, string literals, and called functions from
    all .c files in OpenCrashWOC/code/src/.

    Returns: {func_name: {"file": str, "strings": [str], "calls": [str], "line_count": int}}
    """
    # Match C function definitions - handle various return types
    type_quals = (
        r'(?:(?:static|inline|void|int|long|short|char|unsigned|signed|'
        r'float|double|const|volatile|extern|struct\s+\w+|enum\s+\w+|'
        r'u?(?:int(?:8|16|32|64)_t)|[us](?:8|16|32|64)|f32|f64|'
        r'NuVec\s*\*?|NuMtx\s*\*?|NuQuat\s*\*?|NuSndSFXSlot\s*\*?|'
        r'EdObj\s*\*?|CrateType\s*\*?|CutScene\s*\*?|LevelAI\s*\*?|'
        r'NuSndSampleInfo\s*\*?|BOOL|'
        r'\w+\s*\*)\s+)*'
    )
    # More robust: just find word( at start of line not indented
    func_def_re = re.compile(
        r'^[A-Za-z_][\w\s*]+?\s+(\w+)\s*\([^;]*\)\s*\{',
        re.MULTILINE
    )
    str_lit_re = re.compile(r'"([^"\\]*(?:\\.[^"\\]*)*)"')
    call_re = re.compile(r'\b([A-Za-z_]\w+)\s*\(')

    skip_names = {
        'if', 'while', 'for', 'switch', 'return', 'else', 'do', 'sizeof',
        'struct', 'typedef', 'enum', 'union', 'case', 'break', 'continue',
        'goto', 'default', 'volatile', 'const', 'static', 'extern',
        'inline', 'register', 'defined'
    }

    all_funcs = {}
    file_count = 0

    for root, dirs, files in os.walk(SRC_DIR):
        for fname in sorted(files):
            if not fname.endswith('.c'):
                continue
            fpath = os.path.join(root, fname)
            rel_path = os.path.relpath(fpath, SRC_DIR).replace('\\', '/')
            file_count += 1

            with open(fpath, 'r', encoding='utf-8', errors='replace') as f:
                content = f.read()

            lines = content.split('\n')

            # Find all function definition positions
            func_positions = []
            for m in func_def_re.finditer(content):
                name = m.group(1)
                if name in skip_names:
                    continue
                # Get line number
                line_no = content[:m.start()].count('\n')
                func_positions.append((line_no, name, m.start()))

            # For each function, extract body strings and calls
            for fi, (start_line, func_name, char_pos) in enumerate(func_positions):
                # Find end of function by brace matching from char_pos
                brace_start = content.find('{', char_pos)
                if brace_start < 0:
                    continue
                depth = 0
                pos = brace_start
                while pos < len(content):
                    ch = content[pos]
                    if ch == '{':
                        depth += 1
                    elif ch == '}':
                        depth -= 1
                        if depth == 0:
                            break
                    elif ch == '"':
                        # Skip string literal
                        pos += 1
                        while pos < len(content) and content[pos] != '"':
                            if content[pos] == '\\':
                                pos += 1
                            pos += 1
                    elif ch == '/' and pos + 1 < len(content):
                        if content[pos+1] == '/':
                            # Line comment
                            while pos < len(content) and content[pos] != '\n':
                                pos += 1
                        elif content[pos+1] == '*':
                            pos += 2
                            while pos < len(content) - 1:
                                if content[pos] == '*' and content[pos+1] == '/':
                                    pos += 1
                                    break
                                pos += 1
                    pos += 1

                func_body = content[brace_start:pos+1]
                body_lines = func_body.count('\n') + 1

                # Extract string literals
                strings = []
                for sm in str_lit_re.finditer(func_body):
                    s = sm.group(1)
                    if len(s) >= 3:  # Skip very short strings
                        strings.append(s)

                # Extract function calls
                calls = set()
                for cm in call_re.finditer(func_body):
                    cname = cm.group(1)
                    if cname not in skip_names and cname != func_name:
                        calls.add(cname)

                if func_name not in all_funcs:
                    all_funcs[func_name] = {
                        'file': rel_path,
                        'strings': strings,
                        'calls': sorted(calls),
                        'line_count': body_lines,
                    }

    print(f"  Extracted {len(all_funcs)} functions from {file_count} .c files")
    return all_funcs


# ============================================================================
# Step 2: Parse Ghidra decompiled functions
# ============================================================================

def parse_ghidra_decompiled():
    """Parse decompiled C to get per-function info:
    {addr_hex: {"strings": [str], "string_addrs": [hex], "calls_fun": [addr],
                "calls_named": [name], "body_lines": int}}
    """
    re_func_header = re.compile(r'^/\*\s*FUN_(80[0-9a-fA-F]+)\s*@')
    re_string_ref = re.compile(r'\bs_(\w+?)_(80[0-9a-fA-F]{4,})\b')
    re_fun_call = re.compile(r'\bFUN_(80[0-9a-fA-F]+)\b')
    re_named_call = re.compile(r'\b([A-Za-z_][A-Za-z0-9_]+)\s*\(')

    # Reserved / not-function names
    skip_calls = {
        'if', 'while', 'for', 'switch', 'return', 'else', 'do', 'sizeof',
        'void', 'int', 'char', 'short', 'long', 'float', 'double',
        'unsigned', 'signed', 'uint', 'bool', 'true', 'false', 'null',
        'NULL', 'undefined', 'undefined4', 'undefined2', 'undefined8',
        'CONCAT44', 'CONCAT22', 'CONCAT11', 'SUB41', 'SUB42', 'SUB81',
        'SEXT14', 'SEXT12', 'SEXT24', 'SEXT48', 'ZEXT14', 'ZEXT24',
        'sync', 'returnFromInterrupt', 'dcbi', 'icbi',
        'struct', 'enum', 'union', 'const', 'volatile', 'static',
        'goto', 'case', 'break', 'continue', 'default',
    }

    funcs = {}
    current_addr = None
    current_info = None
    line_count = 0

    with open(DECOMPILED_C, 'r', encoding='utf-8', errors='replace') as f:
        for line in f:
            m = re_func_header.match(line)
            if m:
                # Save previous
                if current_addr and current_info:
                    current_info['body_lines'] = line_count
                    funcs[current_addr] = current_info
                current_addr = m.group(1).lower()
                current_info = {
                    'strings': [],
                    'string_addrs': [],
                    'calls_fun': [],
                    'calls_named': set(),
                    'body_lines': 0,
                }
                line_count = 0
                continue

            if current_info is None:
                continue
            line_count += 1

            # String references
            for sm in re_string_ref.finditer(line):
                label = sm.group(1)
                addr = sm.group(2).lower()
                current_info['string_addrs'].append(addr)

            # FUN_ calls
            for fm in re_fun_call.finditer(line):
                call_addr = fm.group(1).lower()
                if call_addr != current_addr:
                    current_info['calls_fun'].append(call_addr)

            # Named calls
            for nm in re_named_call.finditer(line):
                name = nm.group(1)
                if name not in skip_calls and not name.startswith('FUN_'):
                    current_info['calls_named'].add(name)

    if current_addr and current_info:
        current_info['body_lines'] = line_count
        funcs[current_addr] = current_info

    # Convert sets to sorted lists
    for info in funcs.values():
        info['calls_named'] = sorted(info['calls_named'])
        info['calls_fun'] = list(dict.fromkeys(info['calls_fun']))  # dedup preserving order

    print(f"  Parsed {len(funcs)} decompiled functions")
    return funcs


def load_gc_strings():
    """Load GC string table: {addr_hex: text}"""
    strings = {}
    with open(STRINGS_TXT, 'r', encoding='utf-8', errors='replace') as f:
        for line in f:
            m = re.match(r'^([0-9a-fA-F]+):\s+(.*)$', line.rstrip('\n'))
            if m:
                strings[m.group(1).lower()] = m.group(2)
    print(f"  Loaded {len(strings)} GC strings")
    return strings


def load_gc_functions_json():
    """Load function metadata from JSON."""
    with open(FUNCTIONS_JSON, 'r', encoding='utf-8') as f:
        funcs = json.load(f)
    by_addr = {}
    for fn in funcs:
        addr = fn['address'].lower()
        by_addr[addr] = fn
    print(f"  Loaded {len(funcs)} GC function metadata entries")
    return funcs, by_addr


def load_existing_map():
    """Load existing gc_name_map.json."""
    with open(EXISTING_MAP, 'r', encoding='utf-8') as f:
        m = json.load(f)
    print(f"  Loaded {len(m)} existing name mappings")
    return m


# ============================================================================
# Strategy A: String literal matching
# ============================================================================

def strategy_string_matching(src_funcs, ghidra_funcs, gc_strings, existing_map):
    """Match source functions to Ghidra functions via shared unique string literals.

    For each source function, find its string literals in the GC string table,
    then find which Ghidra function references those string addresses.
    """
    matches = {}
    details = {}

    # Build reverse index: gc_string_addr -> list of ghidra_func_addrs
    str_addr_to_funcs = defaultdict(set)
    for addr, info in ghidra_funcs.items():
        for sa in info['string_addrs']:
            str_addr_to_funcs[sa].add(addr)

    # Build reverse index: string_text -> gc_string_addrs
    text_to_addrs = defaultdict(list)
    for sa, text in gc_strings.items():
        text_to_addrs[text].append(sa)

    already = set(existing_map.keys())

    for func_name, src_info in src_funcs.items():
        if not src_info['strings']:
            continue

        # Find GC string addresses that match this function's string literals
        candidate_funcs = defaultdict(int)  # ghidra_addr -> match_count
        matched_strings = []

        for src_str in src_info['strings']:
            # Try exact match
            addrs = text_to_addrs.get(src_str, [])
            if not addrs:
                # Try with format string normalization (e.g., %s -> *)
                continue

            for sa in addrs:
                for ga in str_addr_to_funcs.get(sa, set()):
                    if ga not in already:
                        candidate_funcs[ga] += 1
                        matched_strings.append(src_str)

        if not candidate_funcs:
            continue

        # Find best candidate (most string matches)
        best_addr = max(candidate_funcs, key=candidate_funcs.get)
        best_count = candidate_funcs[best_addr]

        # Require at least 1 unique string match, prefer more
        if best_count >= 1:
            # Check uniqueness: is this the only source function matching this addr?
            # and is this addr matched by only one source function significantly?
            if best_addr not in matches or candidate_funcs[best_addr] > 1:
                matches[best_addr] = func_name
                details[best_addr] = f"String match ({best_count} strings)"

    # Resolve conflicts: if multiple source funcs map to same addr, pick best
    addr_to_candidates = defaultdict(list)
    for func_name, src_info in src_funcs.items():
        for src_str in src_info['strings']:
            addrs = text_to_addrs.get(src_str, [])
            for sa in addrs:
                for ga in str_addr_to_funcs.get(sa, set()):
                    if ga not in already:
                        addr_to_candidates[ga].append((func_name, src_str))

    # Refine: for each ghidra addr, pick source func with most matching strings
    refined = {}
    refined_details = {}
    for addr, candidates in addr_to_candidates.items():
        if addr in already:
            continue
        func_counts = defaultdict(int)
        func_strings_found = defaultdict(list)
        for fname, sstr in candidates:
            func_counts[fname] += 1
            func_strings_found[fname].append(sstr)

        best_func = max(func_counts, key=func_counts.get)
        best_cnt = func_counts[best_func]

        # Only accept if reasonably unique
        second_best = sorted(func_counts.values(), reverse=True)
        if len(second_best) > 1 and second_best[0] == second_best[1]:
            # Tied - skip unless one has much more
            continue

        if best_cnt >= 1:
            refined[addr] = best_func
            sample = func_strings_found[best_func][0][:40]
            refined_details[addr] = f"String match ({best_cnt} strings, e.g. '{sample}')"

    return refined, refined_details


# ============================================================================
# Strategy B: Call graph pattern matching
# ============================================================================

def strategy_call_graph(src_funcs, ghidra_funcs, existing_map, gc_func_json_by_addr):
    """Match by comparing which named functions each function calls.

    Uses BOTH decompiled code analysis and JSON call graph metadata.
    If a source function calls {NuVecAdd, NuVecScale, NuSplineFindPartial}
    and a Ghidra function calls the same set of named functions, match them.
    """
    matches = {}
    details = {}
    already = set(existing_map.keys())

    # Build name->addr reverse map from existing
    name_to_addr = {}
    for addr, name in existing_map.items():
        name_to_addr[name] = addr

    # For each Ghidra function, get the set of named callees
    # Merge from both decompiled code AND JSON metadata
    ghidra_named_callees = {}
    for addr, info in ghidra_funcs.items():
        if addr in already:
            continue
        named = set(info['calls_named'])
        # Resolve FUN_ calls to names via existing map
        for fun_addr in info['calls_fun']:
            if fun_addr in existing_map:
                named.add(existing_map[fun_addr])

        # Also use JSON calls list (has better coverage)
        json_info = gc_func_json_by_addr.get(addr, {})
        json_calls = json_info.get('calls', [])
        for call_name in json_calls:
            if call_name.startswith('FUN_'):
                call_addr = call_name.replace('FUN_', '').lower()
                if call_addr in existing_map:
                    named.add(existing_map[call_addr])
            elif not call_name.startswith('thunk_FUN_'):
                named.add(call_name)

        if len(named) >= 2:
            ghidra_named_callees[addr] = named

    # For each source function, get called function names
    src_call_sets = {}
    for func_name, src_info in src_funcs.items():
        calls = set(src_info['calls'])
        if len(calls) >= 2:
            src_call_sets[func_name] = calls

    # Match: find (src_func, ghidra_addr) pairs where call sets overlap significantly
    for ghidra_addr, ghidra_calls in ghidra_named_callees.items():
        best_match = None
        best_score = 0
        best_overlap = 0
        second_score = 0

        for src_name, src_calls in src_call_sets.items():
            if src_name in name_to_addr:
                continue

            overlap = ghidra_calls & src_calls
            if len(overlap) < 2:
                continue

            # Jaccard-like score
            union_size = len(ghidra_calls | src_calls)
            score = len(overlap) / union_size if union_size > 0 else 0

            # Coverage: what fraction of ghidra's named calls are in src?
            coverage = len(overlap) / len(ghidra_calls) if ghidra_calls else 0

            combined = score * 0.4 + coverage * 0.6

            if combined > best_score and len(overlap) >= 2:
                second_score = best_score
                best_match = src_name
                best_score = combined
                best_overlap = len(overlap)
            elif combined > second_score:
                second_score = combined

        # Require clear winner (best significantly better than second)
        margin = best_score - second_score
        if best_match and best_score >= 0.3 and best_overlap >= 2 and margin >= 0.1:
            matches[ghidra_addr] = best_match
            details[ghidra_addr] = f"Call graph ({best_overlap} shared callees, score={best_score:.2f})"
            name_to_addr[best_match] = ghidra_addr

    return matches, details


# ============================================================================
# Strategy C: Named callee matching (function calls only already-named funcs)
# ============================================================================

def strategy_named_callee_match(src_funcs, ghidra_funcs, existing_map,
                                all_matches_so_far, gc_func_json_by_addr):
    """For Ghidra functions where most calls go to named functions,
    match against source functions with the same call pattern.
    Uses both decompiled info and JSON call metadata."""
    matches = {}
    details = {}
    already = {**existing_map, **all_matches_so_far}
    already_addrs = set(already.keys())

    name_to_addr = {}
    for addr, name in already.items():
        name_to_addr[name] = addr

    for ghidra_addr in ghidra_funcs:
        if ghidra_addr in already_addrs:
            continue

        info = ghidra_funcs[ghidra_addr]

        # Get all callees as names from decompiled code
        all_fun_calls = info['calls_fun']
        named_calls_direct = set(info['calls_named'])

        # Also use JSON calls
        json_info = gc_func_json_by_addr.get(ghidra_addr, {})
        json_calls = json_info.get('calls', [])
        for call_name in json_calls:
            if call_name.startswith('FUN_'):
                call_addr = call_name.replace('FUN_', '').lower()
                if call_addr in already:
                    named_calls_direct.add(already[call_addr])
                elif call_addr not in set(all_fun_calls):
                    all_fun_calls.append(call_addr)
            elif not call_name.startswith('thunk_FUN_'):
                named_calls_direct.add(call_name)

        # Resolve FUN_ calls
        resolved = set()
        unresolved = 0
        for fun_addr in all_fun_calls:
            if fun_addr in already:
                resolved.add(already[fun_addr])
            else:
                unresolved += 1

        all_named = named_calls_direct | resolved
        total_calls = len(set(all_fun_calls)) + len(named_calls_direct)

        if total_calls == 0:
            continue

        # Only proceed if enough calls are resolved
        if unresolved > total_calls * 0.6:
            continue
        if len(all_named) < 2:
            continue

        # Find source function with best call set match
        best_match = None
        best_score = 0
        second_score = 0

        for src_name, src_info in src_funcs.items():
            if src_name in name_to_addr:
                continue
            src_calls = set(src_info['calls'])
            if len(src_calls) < 2:
                continue

            overlap = all_named & src_calls
            if len(overlap) < 2:
                continue

            # Score based on overlap fraction
            ghidra_coverage = len(overlap) / len(all_named)
            src_coverage = len(overlap) / len(src_calls)
            score = (ghidra_coverage + src_coverage) / 2

            if score > best_score:
                second_score = best_score
                best_match = src_name
                best_score = score
            elif score > second_score:
                second_score = score

        margin = best_score - second_score
        if best_match and best_score >= 0.35 and margin >= 0.08:
            matches[ghidra_addr] = best_match
            details[ghidra_addr] = f"Named callee match (score={best_score:.2f})"
            name_to_addr[best_match] = ghidra_addr

    return matches, details


# ============================================================================
# Strategy D: Error message function name extraction
# ============================================================================

def strategy_error_message_names(ghidra_funcs, gc_strings, src_funcs):
    """Find function names embedded in error/debug strings like:
    'FuncName : error message'
    'FuncName: warning'
    """
    matches = {}
    details = {}
    src_names = set(src_funcs.keys())

    # Build string addr -> ghidra func addr reverse index
    str_to_func = defaultdict(set)
    for addr, info in ghidra_funcs.items():
        for sa in info['string_addrs']:
            str_to_func[sa].add(addr)

    for str_addr, text in gc_strings.items():
        # Check patterns: "FuncName : msg", "FuncName: msg", "FuncName - msg"
        for pat in [
            re.match(r'^(\w{4,})\s*:\s', text),
            re.match(r'^(\w{4,})\s+-\s', text),
            re.match(r'^(\w{4,})\s*\(\)\s*:', text),
        ]:
            if pat:
                candidate_name = pat.group(1)
                if candidate_name in src_names:
                    for ga in str_to_func.get(str_addr, set()):
                        if ga not in matches:
                            matches[ga] = candidate_name
                            details[ga] = f"Error msg name: '{text[:50]}'"
                    break

    return matches, details


# ============================================================================
# Strategy E: Unique function name in strings (assert-style)
# ============================================================================

def strategy_assert_strings(ghidra_funcs, gc_strings, src_funcs):
    """Match functions that reference a string containing exactly one
    source function name (common in assert/debug macros)."""
    matches = {}
    details = {}
    src_names = set(src_funcs.keys())

    str_to_func = defaultdict(set)
    for addr, info in ghidra_funcs.items():
        for sa in info['string_addrs']:
            str_to_func[sa].add(addr)

    # For each string, check if it contains exactly one source function name
    for str_addr, text in gc_strings.items():
        if len(text) < 4:
            continue
        # Find all source function names in this string
        found = []
        for name in src_names:
            if len(name) >= 5 and name in text:
                # Verify it's a whole word
                idx = text.find(name)
                before = text[idx-1] if idx > 0 else ' '
                after = text[idx+len(name)] if idx+len(name) < len(text) else ' '
                if not before.isalnum() and before != '_' and not after.isalnum() and after != '_':
                    found.append(name)

        if len(found) == 1:
            name = found[0]
            for ga in str_to_func.get(str_addr, set()):
                if ga not in matches:
                    matches[ga] = name
                    details[ga] = f"Unique name in string: '{text[:50]}'"

    return matches, details


# ============================================================================
# Strategy F: Source file path strings -> function ordering
# ============================================================================

def strategy_source_file_ordering(ghidra_funcs, gc_strings, gc_funcs_list,
                                  gc_by_addr, src_funcs, all_matches):
    """Use source file path strings (e.g., 'C:/source/crashwoc/code/gamecode/ai.c')
    to group Ghidra functions by source file, then align with OpenCrashWOC function
    order in the same file.
    """
    matches = {}
    details = {}
    already = set(all_matches.keys())

    # Find source file path strings
    file_path_strings = {}
    for sa, text in gc_strings.items():
        if 'crashwoc/code/' in text.lower().replace('\\', '/') and text.endswith('.c'):
            file_path_strings[sa] = text

    # Map string addrs to ghidra func addrs
    str_to_func = defaultdict(set)
    for addr, info in ghidra_funcs.items():
        for sa in info['string_addrs']:
            str_to_func[sa].add(addr)

    # Group Ghidra functions by source file
    ghidra_by_file = defaultdict(set)
    for sa, path in file_path_strings.items():
        norm = normalize_path(path)
        for ga in str_to_func.get(sa, set()):
            ghidra_by_file[norm].add(ga)

    # Also add functions from existing matches
    src_func_to_file = {}
    for fname, info in src_funcs.items():
        src_func_to_file[fname] = info['file']

    for addr, name in all_matches.items():
        if name in src_func_to_file:
            norm = normalize_path(src_func_to_file[name])
            ghidra_by_file[norm].add(addr)

    # Group OpenCrashWOC functions by file
    src_by_file = defaultdict(list)
    for fname, info in src_funcs.items():
        norm = normalize_path(info['file'])
        src_by_file[norm].append(fname)

    # Sort GC functions by address for each file
    gc_sorted = sorted(gc_by_addr.keys(), key=lambda a: int(a, 16))
    gc_addr_to_idx = {a: i for i, a in enumerate(gc_sorted)}

    for norm_path in ghidra_by_file:
        if norm_path not in src_by_file:
            continue

        gc_addrs = sorted(ghidra_by_file[norm_path], key=lambda a: int(a, 16))
        src_names = src_by_file[norm_path]

        if len(gc_addrs) < 2 or len(src_names) < 2:
            continue

        # Find anchor points (already matched functions in this file)
        anchors = []  # (gc_idx_in_file, src_idx_in_file)
        for gi, ga in enumerate(gc_addrs):
            if ga in all_matches:
                matched_name = all_matches[ga]
                if matched_name in src_names:
                    si = src_names.index(matched_name)
                    anchors.append((gi, si))

        if not anchors:
            continue

        anchors.sort()

        # Interpolate between anchors
        for ai in range(len(anchors)):
            gi_a, si_a = anchors[ai]

            if ai < len(anchors) - 1:
                gi_b, si_b = anchors[ai + 1]
                gc_gap = gi_b - gi_a - 1
                src_gap = si_b - si_a - 1

                if gc_gap == src_gap and gc_gap > 0:
                    for off in range(1, gc_gap + 1):
                        ga = gc_addrs[gi_a + off]
                        sn = src_names[si_a + off]
                        if ga not in already and ga not in matches:
                            matches[ga] = sn
                            details[ga] = f"File order ({norm_path})"

            # Extend backward from first anchor
            if ai == 0:
                for off in range(1, min(gi_a, si_a) + 1):
                    ga = gc_addrs[gi_a - off]
                    sn = src_names[si_a - off]
                    if ga not in already and ga not in matches:
                        matches[ga] = sn
                        details[ga] = f"File order bwd ({norm_path})"
                    else:
                        break

            # Extend forward from last anchor
            if ai == len(anchors) - 1:
                rem_gc = len(gc_addrs) - gi_a - 1
                rem_src = len(src_names) - si_a - 1
                for off in range(1, min(rem_gc, rem_src) + 1):
                    ga = gc_addrs[gi_a + off]
                    sn = src_names[si_a + off]
                    if ga not in already and ga not in matches:
                        matches[ga] = sn
                        details[ga] = f"File order fwd ({norm_path})"
                    else:
                        break

    return matches, details


def normalize_path(path):
    """Normalize a source file path for cross-reference."""
    path = path.replace('\\', '/').lower()
    # Strip to relative path from code/
    for prefix in ['c:/source/crashwoc/code/', 'code/src/', '']:
        idx = path.find(prefix)
        if idx >= 0 and prefix:
            path = path[idx + len(prefix):]
            break
    # Strip leading slashes
    path = path.lstrip('/')
    return path


# ============================================================================
# Main
# ============================================================================

def main():
    print("=" * 70)
    print("Building gc_name_map_full.json")
    print("OpenCrashWOC Source -> Ghidra GC Retail Function Matching")
    print("=" * 70)

    # Load data
    print("\n[1/5] Loading existing name map...")
    existing_map = load_existing_map()

    print("[2/5] Extracting OpenCrashWOC source functions...")
    src_funcs = extract_source_functions()

    print("[3/5] Parsing Ghidra decompiled code...")
    ghidra_funcs = parse_ghidra_decompiled()

    print("[4/5] Loading GC strings...")
    gc_strings = load_gc_strings()

    print("[5/5] Loading GC function metadata...")
    gc_funcs_list, gc_by_addr = load_gc_functions_json()

    # Run strategies
    all_matches = dict(existing_map)  # Start with existing DWARF matches
    all_details = {addr: "Existing DWARF match" for addr in existing_map}
    used_names = set(existing_map.values())  # Track used names to prevent duplicates
    strategy_stats = {}

    def add_match(addr, name, detail_str):
        """Add a match if addr not taken and name not already used."""
        if addr in all_matches:
            return False
        if name in used_names:
            return False
        all_matches[addr] = name
        all_details[addr] = detail_str
        used_names.add(name)
        return True

    print(f"\nStarting with {len(all_matches)} existing matches\n")

    # Strategy D: Error message function names (high confidence)
    print("  Strategy D: Error message function names...")
    sd_matches, sd_details = strategy_error_message_names(
        ghidra_funcs, gc_strings, src_funcs)
    new_d = 0
    for addr, name in sd_matches.items():
        if add_match(addr, name, f"[SD] {sd_details[addr]}"):
            new_d += 1
    strategy_stats["D: Error msg names"] = new_d
    print(f"    -> {new_d} new matches")

    # Strategy E: Assert/unique name in strings
    print("  Strategy E: Unique name in strings...")
    se_matches, se_details = strategy_assert_strings(
        ghidra_funcs, gc_strings, src_funcs)
    new_e = 0
    for addr, name in se_matches.items():
        if add_match(addr, name, f"[SE] {se_details[addr]}"):
            new_e += 1
    strategy_stats["E: Unique name strings"] = new_e
    print(f"    -> {new_e} new matches")

    # Strategy A: String literal matching
    print("  Strategy A: String literal matching...")
    sa_matches, sa_details = strategy_string_matching(
        src_funcs, ghidra_funcs, gc_strings, all_matches)
    new_a = 0
    for addr, name in sa_matches.items():
        if add_match(addr, name, f"[SA] {sa_details[addr]}"):
            new_a += 1
    strategy_stats["A: String literals"] = new_a
    print(f"    -> {new_a} new matches")

    # Strategy B: Call graph matching
    print("  Strategy B: Call graph pattern matching...")
    sb_matches, sb_details = strategy_call_graph(
        src_funcs, ghidra_funcs, all_matches, gc_by_addr)
    new_b = 0
    for addr, name in sb_matches.items():
        if add_match(addr, name, f"[SB] {sb_details[addr]}"):
            new_b += 1
    strategy_stats["B: Call graph"] = new_b
    print(f"    -> {new_b} new matches")

    # Strategy C: Named callee matching
    print("  Strategy C: Named callee matching...")
    sc_matches, sc_details = strategy_named_callee_match(
        src_funcs, ghidra_funcs, all_matches, {}, gc_by_addr)
    new_c = 0
    for addr, name in sc_matches.items():
        if add_match(addr, name, f"[SC] {sc_details[addr]}"):
            new_c += 1
    strategy_stats["C: Named callees"] = new_c
    print(f"    -> {new_c} new matches")

    # Strategy F: Source file ordering
    print("  Strategy F: Source file ordering...")
    sf_matches, sf_details = strategy_source_file_ordering(
        ghidra_funcs, gc_strings, gc_funcs_list, gc_by_addr,
        src_funcs, all_matches)
    new_f = 0
    for addr, name in sf_matches.items():
        if add_match(addr, name, f"[SF] {sf_details[addr]}"):
            new_f += 1
    strategy_stats["F: File ordering"] = new_f
    print(f"    -> {new_f} new matches")

    # Iterative passes - each round may unlock new matches as more functions
    # get named, providing better anchors for call graph matching
    for iteration in range(1, 5):
        prev_count = len(all_matches)
        print(f"\n  --- Iterative pass {iteration} ({len(all_matches)} matches so far) ---")

        # Call graph matching (with accumulated matches as context)
        sbi_matches, sbi_details = strategy_call_graph(
            src_funcs, ghidra_funcs, all_matches, gc_by_addr)
        new_bi = 0
        for addr, name in sbi_matches.items():
            if add_match(addr, name, f"[SB{iteration+1}] {sbi_details[addr]}"):
                new_bi += 1
        strategy_stats[f"B{iteration+1}: Call graph pass {iteration+1}"] = new_bi
        print(f"    Call graph: {new_bi} new")

        # Named callee matching
        sci_matches, sci_details = strategy_named_callee_match(
            src_funcs, ghidra_funcs, all_matches, {}, gc_by_addr)
        new_ci = 0
        for addr, name in sci_matches.items():
            if add_match(addr, name, f"[SC{iteration+1}] {sci_details[addr]}"):
                new_ci += 1
        strategy_stats[f"C{iteration+1}: Named callees pass {iteration+1}"] = new_ci
        print(f"    Named callees: {new_ci} new")

        # Source file ordering (with more anchors now)
        sfi_matches, sfi_details = strategy_source_file_ordering(
            ghidra_funcs, gc_strings, gc_funcs_list, gc_by_addr,
            src_funcs, all_matches)
        new_fi = 0
        for addr, name in sfi_matches.items():
            if add_match(addr, name, f"[SF{iteration+1}] {sfi_details[addr]}"):
                new_fi += 1
        strategy_stats[f"F{iteration+1}: File ordering pass {iteration+1}"] = new_fi
        print(f"    File ordering: {new_fi} new")

        total_new = len(all_matches) - prev_count
        print(f"    Total new this round: {total_new}")
        if total_new == 0:
            print("    Converged, stopping iterations.")
            break

    # Write output
    print(f"\n{'='*70}")
    print("RESULTS")
    print(f"{'='*70}")
    print(f"Existing DWARF matches: {len(existing_map)}")
    new_total = len(all_matches) - len(existing_map)
    print(f"New matches added:      {new_total}")
    print(f"Total matches:          {len(all_matches)}")
    print(f"Total GC functions:     {len(gc_funcs_list)}")
    print(f"Coverage:               {100*len(all_matches)/len(gc_funcs_list):.1f}%")
    print(f"\nBy strategy:")
    for k, v in sorted(strategy_stats.items()):
        print(f"  {k}: {v}")

    # Validate: no duplicate names
    # Keep existing DWARF matches over new ones, then first by address
    name_counts = defaultdict(list)
    for addr, name in all_matches.items():
        name_counts[name].append(addr)
    dupes = {n: addrs for n, addrs in name_counts.items() if len(addrs) > 1}
    if dupes:
        print(f"\nWARNING: {len(dupes)} duplicate names found, resolving:")
        cleaned = {}
        seen_names = set()
        # First pass: add all existing DWARF matches (they take priority)
        for addr in sorted(existing_map.keys(), key=lambda a: int(a, 16)):
            name = existing_map[addr]
            if name not in seen_names:
                cleaned[addr] = name
                seen_names.add(name)
        # Second pass: add new matches, skipping duplicates
        for addr in sorted(all_matches.keys(), key=lambda a: int(a, 16)):
            if addr in cleaned:
                continue
            name = all_matches[addr]
            if name not in seen_names:
                cleaned[addr] = name
                seen_names.add(name)
            else:
                print(f"  Dropping {addr} -> {name} (duplicate)")
        all_matches = cleaned

    # Sort by address and write
    sorted_map = dict(sorted(all_matches.items(), key=lambda x: int(x[0], 16)))

    os.makedirs(MAPS, exist_ok=True)
    with open(OUTPUT_MAP, 'w', encoding='utf-8') as f:
        json.dump(sorted_map, f, indent=2)

    print(f"\nWrote {len(sorted_map)} entries to {OUTPUT_MAP}")

    # Show sample new matches
    print(f"\n{'='*70}")
    print("SAMPLE NEW MATCHES")
    print(f"{'='*70}")
    new_entries = [(a, n, all_details.get(a, '')) for a, n in sorted_map.items()
                   if a not in existing_map]
    for addr, name, detail in new_entries[:30]:
        print(f"  {addr} -> {name}  ({detail})")
    if len(new_entries) > 30:
        print(f"  ... and {len(new_entries) - 30} more")


if __name__ == '__main__':
    main()
