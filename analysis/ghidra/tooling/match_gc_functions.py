#!/usr/bin/env python3
"""
Match GameCube retail function addresses (FUN_80XXXXXX) to real function names.

Uses multiple strategies:
  1. String-based matching - error messages containing function names
  2. Direct string match from PS2 names in GC decompiled code
  3. DWARF label + compile unit order matching
  4. Source file grouping + anchor-based alignment
  5. Unique size matching (conservative)

Inputs:
  - main.dol_decompiled.c   (GC retail decompiled code)
  - main.dol_functions.json  (GC retail function metadata)
  - SLES_503.86_ccc_functions.c (PS2 EU functions by source file)
  - dump_alphaNGCport_DWARF.txt (DWARF dump with function info)
  - main.dol_strings.txt     (Ghidra extracted strings)

Outputs:
  - gc_name_map.json   {"80003100": "FunctionName", ...}
  - gc_params_map.json {"FunctionName": {"params": [...], "locals": [...]}}
"""

import json
import re
import os
import bisect
from collections import defaultdict
from pathlib import Path

# === File paths ===
BASE = str(Path(__file__).resolve().parents[3])
RAW = os.path.join(BASE, "analysis", "ghidra", "raw")
MAPS = os.path.join(BASE, "analysis", "ghidra", "maps")

DECOMPILED_C = os.path.join(RAW, "main.dol_decompiled.c")
FUNCTIONS_JSON = os.path.join(RAW, "main.dol_functions.json")
PS2_FUNCTIONS = os.path.join(RAW, "SLES_503.86_ccc_functions.c")
DWARF_DUMP = os.path.join(BASE, "code", "src", "dump_alphaNGCport_DWARF.txt")
STRINGS_TXT = os.path.join(RAW, "main.dol_strings.txt")

OUT_NAME_MAP = os.path.join(MAPS, "gc_name_map.json")
OUT_PARAMS_MAP = os.path.join(MAPS, "gc_params_map.json")


# ===========================================================================
# Data loaders
# ===========================================================================

def load_gc_functions():
    """Load GC retail function list from JSON."""
    with open(FUNCTIONS_JSON, "r", encoding="utf-8") as f:
        funcs = json.load(f)
    by_addr = {}
    for fn in funcs:
        addr = fn["address"].lower()
        by_addr[addr] = fn
    return funcs, by_addr


def load_gc_strings():
    """Load strings from main.dol_strings.txt -> {addr_hex: text}"""
    strings = {}
    if not os.path.exists(STRINGS_TXT):
        print(f"  [!] Strings file not found: {STRINGS_TXT}")
        return strings
    with open(STRINGS_TXT, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            m = re.match(r"^([0-9a-fA-F]+):\s+(.*)$", line.rstrip("\n"))
            if m:
                strings[m.group(1).lower()] = m.group(2)
    return strings


def load_ps2_functions():
    """Parse PS2 EU functions file.
    Returns (ps2_names, ps2_by_file, ps2_by_name)
    """
    ps2_names = set()
    ps2_by_file = defaultdict(list)
    ps2_by_name = {}
    current_file = None

    with open(PS2_FUNCTIONS, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            file_m = re.match(r"^//\s*FILE\s*--\s*(.+)$", line.strip())
            if file_m:
                current_file = file_m.group(1).strip()
                continue
            func_m = re.match(
                r"^/\*\s*([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s*\*/\s*(?:static\s+)?(\w+)\s*\(",
                line.strip()
            )
            if func_m:
                addr = int(func_m.group(1), 16)
                size = int(func_m.group(2), 16)
                name = func_m.group(3)
                ps2_names.add(name)
                if current_file:
                    ps2_by_file[current_file].append((name, addr, size))
                ps2_by_name[name] = (addr, size, current_file)

    return ps2_names, ps2_by_file, ps2_by_name


def load_dwarf_functions():
    """Parse DWARF dump for function signatures, params, locals, labels, and
    compile unit ordering.
    Returns:
      - dwarf_funcs: ordered list of func dicts
      - dwarf_by_name: {name: func_dict}
      - dwarf_label_to_func: {alpha_addr_hex: func_name}
      - dwarf_compile_units: [(unit_path, [func_name, ...])]
    """
    dwarf_funcs = []
    dwarf_by_name = {}
    dwarf_label_to_func = {}
    dwarf_compile_units = []

    current_compile_unit = None
    current_unit_funcs = []
    current_func = None

    re_compile = re.compile(r"^//\s*Compile unit:\s*(.+)$")
    re_func = re.compile(
        r"^(?:static\s+)?(?:inline\s+)?"
        r"([\w\s*]+?)\s+"
        r"(\w+)\s*"
        r"\(([^)]*)\)\s*\{$"
    )
    re_label = re.compile(r"^\s+(\w+):\s*//\s*(0x[0-9a-fA-F]+)")
    re_local_var = re.compile(
        r"^\s+([\w\s*\[\]]+?)\s+(\w+(?:\[\d+\])?)\s*;\s*//\s*(\S+)"
    )
    re_local_var_noreg = re.compile(
        r"^\s+([\w\s*\[\]]+?)\s+(\w+(?:\[\d+\])?)\s*;"
    )
    re_param = re.compile(
        r"([\w\s*]+?)\s+(\w+)\s*/\*\s*(\w+)\s*\*/"
    )

    in_func = False
    in_locals = False
    brace_depth = 0

    def finish_func():
        nonlocal current_func, in_func
        if current_func is None:
            return
        dwarf_funcs.append(current_func)
        if current_func["name"] not in dwarf_by_name:
            dwarf_by_name[current_func["name"]] = current_func
        for lbl_addr in current_func["labels"]:
            dwarf_label_to_func[lbl_addr] = current_func["name"]
        current_unit_funcs.append(current_func["name"])
        current_func = None
        in_func = False

    with open(DWARF_DUMP, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            line_stripped = line.rstrip("\n")

            # Compile unit
            m = re_compile.match(line_stripped)
            if m:
                # Save previous unit
                if current_compile_unit and current_unit_funcs:
                    dwarf_compile_units.append(
                        (current_compile_unit, current_unit_funcs[:]))
                current_compile_unit = m.group(1).strip()
                current_unit_funcs = []
                continue

            # Skip various non-function lines
            stripped = line_stripped.lstrip()
            if stripped.startswith("//"):
                continue
            if "IDirect3D" in line_stripped:
                continue

            # Function signature
            m = re_func.match(line_stripped)
            if m:
                ret_type = m.group(1).strip()
                name = m.group(2).strip()
                params_str = m.group(3).strip()

                if name in ("struct", "enum", "union"):
                    continue
                # Skip inline functions (they don't get their own addresses)
                if "inline" in line_stripped.split(name)[0]:
                    continue

                # Parse parameters
                params = []
                if params_str and params_str != "void":
                    for pm in re_param.finditer(params_str):
                        ptype = pm.group(1).strip()
                        pname = pm.group(2).strip()
                        params.append({"name": pname, "type": ptype})
                    if not params:
                        for part in params_str.split(","):
                            part = part.strip()
                            if part in ("void", "..."):
                                continue
                            tokens = part.rsplit(None, 1)
                            if len(tokens) == 2:
                                params.append({
                                    "name": tokens[1].strip("*"),
                                    "type": tokens[0].strip()
                                })

                current_func = {
                    "name": name,
                    "return_type": ret_type,
                    "params": params,
                    "locals": [],
                    "labels": [],
                    "compile_unit": current_compile_unit,
                }
                in_func = True
                in_locals = False
                brace_depth = 1
                continue

            if not in_func:
                continue

            # Track brace depth
            brace_depth += line_stripped.count("{") - line_stripped.count("}")
            if brace_depth <= 0:
                finish_func()
                continue

            if "// Local variables" in line_stripped:
                in_locals = True
                continue
            if "// Labels" in line_stripped:
                in_locals = False
                continue

            # Parse labels
            m = re_label.match(line_stripped)
            if m:
                label_addr = m.group(2).lower().replace("0x", "")
                current_func["labels"].append(label_addr)
                continue

            # Parse local variables
            if in_locals and current_func:
                m = re_local_var.match(line_stripped)
                if m:
                    vtype = m.group(1).strip()
                    vname = m.group(2).strip()
                    if not any(l["name"] == vname and l["type"] == vtype
                              for l in current_func["locals"]):
                        current_func["locals"].append(
                            {"name": vname, "type": vtype})
                    continue
                m = re_local_var_noreg.match(line_stripped)
                if m:
                    vtype = m.group(1).strip()
                    vname = m.group(2).strip()
                    if vtype not in ("struct", "enum", "union") and "{" not in vtype:
                        if not any(l["name"] == vname and l["type"] == vtype
                                   for l in current_func["locals"]):
                            current_func["locals"].append(
                                {"name": vname, "type": vtype})

    # Save last compile unit
    if current_compile_unit and current_unit_funcs:
        dwarf_compile_units.append(
            (current_compile_unit, current_unit_funcs[:]))

    return dwarf_funcs, dwarf_by_name, dwarf_label_to_func, dwarf_compile_units


def parse_gc_decompiled_functions():
    """Stream-parse the decompiled C file to extract per-function string refs
    and call targets.
    Returns {addr_hex: {"string_refs": [...], "string_addrs": [...], "calls": [...]}}
    """
    gc_func_info = {}
    current_addr = None
    current_info = None

    re_func_header = re.compile(r"^/\*\s*FUN_(80[0-9a-fA-F]+)\s*@")
    re_string_ref = re.compile(r"\bs_([\w]+?)_(80[0-9a-fA-F]{4,})\b")
    re_fun_call = re.compile(r"\bFUN_(80[0-9a-fA-F]+)\b")

    with open(DECOMPILED_C, "r", encoding="utf-8", errors="replace") as f:
        for line_no, line in enumerate(f, 1):
            m = re_func_header.match(line)
            if m:
                addr = m.group(1).lower()
                if current_addr and current_info:
                    gc_func_info[current_addr] = current_info
                current_addr = addr
                current_info = {
                    "string_refs": [],
                    "string_addrs": [],
                    "calls": set(),
                    "line_start": line_no,
                }
                continue

            if current_info is None:
                continue

            for sm in re_string_ref.finditer(line):
                label_text = sm.group(1)
                str_addr = sm.group(2).lower()
                current_info["string_refs"].append((label_text, str_addr))
                current_info["string_addrs"].append(str_addr)

            for fm in re_fun_call.finditer(line):
                call_addr = fm.group(1).lower()
                if call_addr != current_addr:
                    current_info["calls"].add(call_addr)

    if current_addr and current_info:
        gc_func_info[current_addr] = current_info

    for info in gc_func_info.values():
        info["calls"] = list(info["calls"])

    return gc_func_info


# ===========================================================================
# Helpers
# ===========================================================================

def normalize_source_path(path):
    """Extract the significant suffix from a source path for cross-platform matching."""
    path = path.replace("\\", "/").lower()
    for prefix in ["gamecode/", "gamelib/", "nu3dx/", "nucore/", "numath/",
                    "nuxbox/", "system/gc/", "system/"]:
        idx = path.find(prefix)
        if idx >= 0:
            return path[idx:]
    parts = path.rstrip("/").split("/")
    if len(parts) >= 2:
        return "/".join(parts[-2:])
    return path


def build_func_range_index(gc_funcs_list):
    """Build a sorted index for binary-searching which function contains an address."""
    ranges = []
    for fn in gc_funcs_list:
        start = int(fn["address"], 16)
        end = start + fn["size"]
        ranges.append((start, end, fn["address"].lower()))
    ranges.sort()
    starts = [r[0] for r in ranges]
    return ranges, starts


def find_containing_func(addr_int, func_ranges, starts):
    """Binary search to find which GC function contains an address."""
    idx = bisect.bisect_right(starts, addr_int) - 1
    if idx >= 0:
        s, e, faddr = func_ranges[idx]
        if s <= addr_int < e:
            return faddr
    return None


# ===========================================================================
# Strategy 1: String-based matching (function names in error strings)
# ===========================================================================

def strategy_string_match(gc_func_info, gc_strings, ps2_names):
    """Find PS2 function names mentioned in string literals within each GC function.
    The error prolog pattern puts function names in strings like:
    'NuRndrTri2d : Vertex buffer full!'
    """
    matches = {}
    match_details = {}

    for addr, info in gc_func_info.items():
        if addr in matches:
            continue

        # Collect all actual string content for this function
        func_strings = []
        for label_text, str_addr in info["string_refs"]:
            if str_addr in gc_strings:
                func_strings.append(gc_strings[str_addr])
            else:
                func_strings.append(label_text.replace("_", " ").strip())

        # Find best matching function name
        best_match = None
        best_confidence = 0
        best_source = None

        for s in func_strings:
            for name in ps2_names:
                if len(name) < 4:
                    continue
                # Check patterns: "FuncName :", "FuncName:", "FuncName ("
                for pat in (name + " :", name + ":", name + " -", name + "("):
                    if pat in s:
                        confidence = len(name)
                        if confidence > best_confidence:
                            best_match = name
                            best_confidence = confidence
                            best_source = s
                        break

        if best_match and best_confidence >= 4:
            matches[addr] = best_match
            match_details[addr] = f"String: '{best_source}'"

    return matches, match_details


# ===========================================================================
# Strategy 2: Direct PS2 name search in GC strings
# ===========================================================================

def strategy_direct_string_name(gc_func_info, gc_strings, ps2_names):
    """Search for PS2 function names appearing as string content in GC code."""
    matches = {}
    match_details = {}

    # Reverse index: string_addr -> [gc_func_addr, ...]
    string_to_funcs = defaultdict(list)
    for addr, info in gc_func_info.items():
        for str_addr in info["string_addrs"]:
            string_to_funcs[str_addr].append(addr)

    for name in ps2_names:
        if len(name) < 5:
            continue
        for str_addr, text in gc_strings.items():
            if text.startswith(name + " :") or text.startswith(name + ":") or text == name:
                for gc_addr in string_to_funcs.get(str_addr, []):
                    if gc_addr not in matches:
                        matches[gc_addr] = name
                        match_details[gc_addr] = \
                            f"Direct string: '{text}' at {str_addr}"

    return matches, match_details


# ===========================================================================
# Strategy 3: DWARF label + compile unit order matching
# ===========================================================================

def strategy_dwarf_ordering(gc_funcs_list, gc_by_addr, dwarf_label_to_func,
                            dwarf_compile_units, dwarf_by_name,
                            ps2_by_name, already_matched):
    """Match functions using DWARF compile unit ordering.

    The DWARF dump is from the alpha NGC build. Functions within each compile
    unit appear in the same order as in the retail binary. We use label-based
    anchors (from DWARF labels with 0x80XXXXXX addresses) to determine which
    GC retail address range corresponds to each compile unit, then align the
    function lists.

    Uses segment-based alignment between pairs of anchors to handle cases where
    the alpha and retail builds have different numbers of functions (due to
    inlining, code removal, etc.).
    """
    matches = {}
    match_details = {}

    func_ranges, starts = build_func_range_index(gc_funcs_list)

    # Sort GC functions by address
    gc_sorted_addrs = sorted(gc_by_addr.keys(), key=lambda a: int(a, 16))
    gc_addr_to_idx = {a: i for i, a in enumerate(gc_sorted_addrs)}

    # Step 0: Direct label-based matches (highest confidence)
    # DWARF labels have alpha NGC addresses - if a label falls inside a
    # retail function, that function must be the label's parent function.
    for label_addr_hex, func_name in dwarf_label_to_func.items():
        label_addr_int = int(label_addr_hex, 16)
        gc_addr = find_containing_func(label_addr_int, func_ranges, starts)
        if gc_addr and gc_addr not in already_matched and gc_addr not in matches:
            matches[gc_addr] = func_name
            match_details[gc_addr] = \
                f"DWARF label at 0x{label_addr_hex} -> {func_name}"

    # Step 1: Build anchors from DWARF labels (highest priority) and
    # already_matched names (+ label matches from Step 0)
    combined_matched = {**already_matched, **matches}
    for unit_path, dwarf_func_list in dwarf_compile_units:
        # Collect anchors: (dwarf_idx, gc_global_idx, source_info)
        anchors = []
        for di, dname in enumerate(dwarf_func_list):
            gc_addr = None
            src = None

            # Priority 1: label-based match (direct address in function)
            if dname in dwarf_by_name:
                for lbl in dwarf_by_name[dname].get("labels", []):
                    lbl_int = int(lbl, 16)
                    ga = find_containing_func(lbl_int, func_ranges, starts)
                    if ga:
                        gc_addr = ga
                        src = f"label@0x{lbl}"
                        break

            # Priority 2: already-matched name (including label matches from Step 0)
            if gc_addr is None:
                for ga, matched_name in combined_matched.items():
                    if matched_name == dname:
                        gc_addr = ga
                        src = "name_match"
                        break

            if gc_addr is not None:
                gc_idx = gc_addr_to_idx.get(gc_addr)
                if gc_idx is not None:
                    anchors.append((di, gc_idx, gc_addr, src))

        if not anchors:
            continue

        # Sort anchors by DWARF index
        anchors.sort(key=lambda a: a[0])

        # Step 2: Use segment-based alignment between pairs of anchors
        # Between two consecutive anchors (di1, gi1) and (di2, gi2):
        #   If dwarf_gap == gc_gap, the functions align 1:1
        #   If they differ, only match functions close to anchors

        def try_assign(dwarf_idx, gc_global_idx, conf_tag):
            """Try to assign a DWARF function to a GC address."""
            if gc_global_idx < 0 or gc_global_idx >= len(gc_sorted_addrs):
                return
            gc_addr = gc_sorted_addrs[gc_global_idx]
            dname = dwarf_func_list[dwarf_idx]

            if gc_addr in combined_matched or gc_addr in matches:
                return

            # Size validation
            gc_size = gc_by_addr[gc_addr]["size"]
            ps2_info = ps2_by_name.get(dname)
            if ps2_info:
                ps2_size = ps2_info[1]
                if ps2_size > 0 and gc_size > 0:
                    ratio = gc_size / ps2_size
                    if ratio < 0.05 or ratio > 20.0:
                        return

            matches[gc_addr] = dname
            match_details[gc_addr] = \
                f"DWARF order: {unit_path} [{dwarf_idx}] ({conf_tag})"

        # Process segments between consecutive anchors
        for ai in range(len(anchors)):
            di_a, gi_a, _, _ = anchors[ai]

            if ai < len(anchors) - 1:
                di_b, gi_b, _, _ = anchors[ai + 1]
                dwarf_gap = di_b - di_a - 1
                gc_gap = gi_b - gi_a - 1

                if dwarf_gap == gc_gap and dwarf_gap >= 0:
                    # Perfect alignment between anchors
                    for offset in range(1, dwarf_gap + 1):
                        try_assign(di_a + offset, gi_a + offset, "segment_aligned")
                elif dwarf_gap >= 0 and gc_gap >= 0:
                    # Imperfect alignment - only match from each anchor side
                    # Match forward from anchor A (up to half the gap)
                    safe_fwd = min(dwarf_gap, gc_gap, max(1, min(dwarf_gap, gc_gap) // 2))
                    for offset in range(1, safe_fwd + 1):
                        try_assign(di_a + offset, gi_a + offset, "segment_partial_fwd")
                    # Match backward from anchor B
                    safe_bwd = min(dwarf_gap, gc_gap, max(1, min(dwarf_gap, gc_gap) // 2))
                    for offset in range(1, safe_bwd + 1):
                        try_assign(di_b - offset, gi_b - offset, "segment_partial_bwd")

            # Extend backward from first anchor
            if ai == 0:
                # Extend up to 30 functions backward, with size checks
                for offset in range(1, min(di_a, gi_a, 30) + 1):
                    d_idx = di_a - offset
                    g_idx = gi_a - offset
                    if d_idx < 0 or g_idx < 0:
                        break
                    gc_addr_check = gc_sorted_addrs[g_idx]
                    dname_check = dwarf_func_list[d_idx]
                    gc_sz = gc_by_addr[gc_addr_check]["size"]
                    ps2_inf = ps2_by_name.get(dname_check)
                    if ps2_inf and ps2_inf[1] > 0 and gc_sz > 0:
                        r = gc_sz / ps2_inf[1]
                        if not (0.1 < r < 10.0):
                            break
                    try_assign(d_idx, g_idx, "extend_backward")

            # Extend forward from last anchor
            if ai == len(anchors) - 1:
                remaining_d = len(dwarf_func_list) - di_a - 1
                remaining_g = len(gc_sorted_addrs) - gi_a - 1
                for offset in range(1, min(remaining_d, remaining_g, 30) + 1):
                    d_idx = di_a + offset
                    g_idx = gi_a + offset
                    if d_idx >= len(dwarf_func_list) or g_idx >= len(gc_sorted_addrs):
                        break
                    gc_addr_check = gc_sorted_addrs[g_idx]
                    dname_check = dwarf_func_list[d_idx]
                    gc_sz = gc_by_addr[gc_addr_check]["size"]
                    ps2_inf = ps2_by_name.get(dname_check)
                    if ps2_inf and ps2_inf[1] > 0 and gc_sz > 0:
                        r = gc_sz / ps2_inf[1]
                        if not (0.1 < r < 10.0):
                            break
                    try_assign(d_idx, g_idx, "extend_forward")

    return matches, match_details


# ===========================================================================
# Strategy 4: Source file grouping + anchor-based alignment
# ===========================================================================

def strategy_source_file_grouping(gc_func_info, gc_funcs_list, gc_by_addr,
                                  gc_strings, ps2_by_file, ps2_by_name,
                                  already_matched):
    """Group GC functions by source file using string references,
    then use anchored alignment with size validation.
    """
    matches = {}
    match_details = {}

    # Build source file string addr -> source path
    source_file_strings = {}
    for str_addr, text in gc_strings.items():
        if text.startswith("C:/source/crashwoc/code/") and text.endswith(".c"):
            source_file_strings[str_addr] = text

    # Build PS2 file lookup by normalized path
    ps2_file_lookup = {}
    for ps2_path, funcs in ps2_by_file.items():
        norm = normalize_source_path(ps2_path)
        ps2_file_lookup[norm] = (ps2_path, funcs)

    # Determine source file for each GC function
    gc_func_source = {}
    for addr, info in gc_func_info.items():
        for str_addr in info["string_addrs"]:
            if str_addr in source_file_strings:
                src = source_file_strings[str_addr]
                norm = normalize_source_path(src)
                gc_func_source[addr] = norm
                break

    # Group GC functions by source file
    gc_by_source = defaultdict(list)
    for addr, norm in gc_func_source.items():
        gc_by_source[norm].append(addr)

    # Also determine source file from already_matched names
    for gc_addr, name in already_matched.items():
        if name in ps2_by_name:
            _, _, src_file = ps2_by_name[name]
            if src_file:
                norm = normalize_source_path(src_file)
                if gc_addr not in gc_func_source:
                    gc_func_source[gc_addr] = norm
                    gc_by_source[norm].append(gc_addr)

    # For each source file group, align with PS2 function list
    for norm_path, gc_addrs in gc_by_source.items():
        if norm_path not in ps2_file_lookup:
            continue

        ps2_path, ps2_funcs = ps2_file_lookup[norm_path]
        ps2_func_names = [f[0] for f in ps2_funcs]

        # Sort GC functions by address
        gc_addrs_sorted = sorted(set(gc_addrs), key=lambda a: int(a, 16))

        # Find anchors from already_matched
        anchors = []
        for gi, gc_addr in enumerate(gc_addrs_sorted):
            if gc_addr in already_matched:
                matched_name = already_matched[gc_addr]
                if matched_name in ps2_func_names:
                    pi = ps2_func_names.index(matched_name)
                    anchors.append((gi, pi))

        if not anchors:
            continue

        # Use anchors to fill gaps
        for ai in range(len(anchors)):
            gc_i, ps2_i = anchors[ai]

            # Between consecutive anchors
            if ai < len(anchors) - 1:
                next_gc_i, next_ps2_i = anchors[ai + 1]
                gc_gap = next_gc_i - gc_i - 1
                ps2_gap = next_ps2_i - ps2_i - 1
                if gc_gap == ps2_gap and gc_gap > 0:
                    for offset in range(1, gc_gap + 1):
                        fill_addr = gc_addrs_sorted[gc_i + offset]
                        fill_name = ps2_func_names[ps2_i + offset]
                        if fill_addr not in already_matched and fill_addr not in matches:
                            matches[fill_addr] = fill_name
                            match_details[fill_addr] = \
                                f"Src file interp: {norm_path}"

            # Extend backward from first anchor
            if ai == 0 and gc_i > 0 and ps2_i > 0:
                for offset in range(1, min(gc_i, ps2_i) + 1):
                    fill_addr = gc_addrs_sorted[gc_i - offset]
                    fill_name = ps2_func_names[ps2_i - offset]
                    gc_size = gc_by_addr.get(fill_addr, {}).get("size", 0)
                    ps2_size = ps2_by_name.get(fill_name, (0, 0, ""))[1]
                    if gc_size > 0 and ps2_size > 0:
                        ratio = gc_size / ps2_size
                        if not (0.15 < ratio < 7.0):
                            break
                    if fill_addr not in already_matched and fill_addr not in matches:
                        matches[fill_addr] = fill_name
                        match_details[fill_addr] = \
                            f"Src file backfill: {norm_path}"

            # Extend forward from last anchor
            if ai == len(anchors) - 1:
                remaining_gc = len(gc_addrs_sorted) - gc_i - 1
                remaining_ps2 = len(ps2_func_names) - ps2_i - 1
                for offset in range(1, min(remaining_gc, remaining_ps2) + 1):
                    fill_addr = gc_addrs_sorted[gc_i + offset]
                    fill_name = ps2_func_names[ps2_i + offset]
                    gc_size = gc_by_addr.get(fill_addr, {}).get("size", 0)
                    ps2_size = ps2_by_name.get(fill_name, (0, 0, ""))[1]
                    if gc_size > 0 and ps2_size > 0:
                        ratio = gc_size / ps2_size
                        if not (0.15 < ratio < 7.0):
                            break
                    if fill_addr not in already_matched and fill_addr not in matches:
                        matches[fill_addr] = fill_name
                        match_details[fill_addr] = \
                            f"Src file forward: {norm_path}"

    return matches, match_details


# ===========================================================================
# Strategy 5: Unique size matching
# ===========================================================================

def strategy_unique_size_match(gc_funcs_list, gc_by_addr, ps2_by_name,
                               already_matched):
    """Match functions with sizes unique in both GC and PS2 sets."""
    matches = {}
    match_details = {}

    gc_size_map = defaultdict(list)
    for fn in gc_funcs_list:
        addr = fn["address"].lower()
        if addr not in already_matched:
            gc_size_map[fn["size"]].append(addr)

    ps2_size_map = defaultdict(list)
    for name, (a, size, sf) in ps2_by_name.items():
        ps2_size_map[size].append(name)

    for size, gc_addrs in gc_size_map.items():
        if len(gc_addrs) != 1 or size < 32:
            continue
        if size not in ps2_size_map or len(ps2_size_map[size]) != 1:
            continue
        matches[gc_addrs[0]] = ps2_size_map[size][0]
        match_details[gc_addrs[0]] = f"Unique size: {size} bytes"

    return matches, match_details


# ===========================================================================
# Build params map from DWARF
# ===========================================================================

def build_params_map(all_matches, dwarf_by_name):
    """Build parameter/locals map for matched functions using DWARF data."""
    params_map = {}
    for addr, name in all_matches.items():
        if name in dwarf_by_name:
            dw = dwarf_by_name[name]
            entry = {}
            if dw["params"]:
                entry["params"] = dw["params"]
            if dw["locals"]:
                entry["locals"] = dw["locals"]
            if entry:
                params_map[name] = entry
    return params_map


# ===========================================================================
# Main
# ===========================================================================

def main():
    print("=" * 70)
    print("GameCube Retail Function Name Matcher")
    print("=" * 70)

    # --- Load data ---
    print("\n[1/6] Loading GC retail functions...")
    gc_funcs_list, gc_by_addr = load_gc_functions()
    print(f"  Loaded {len(gc_funcs_list)} GC functions")

    print("[2/6] Loading GC strings...")
    gc_strings = load_gc_strings()
    print(f"  Loaded {len(gc_strings)} strings")

    print("[3/6] Loading PS2 EU functions...")
    ps2_names, ps2_by_file, ps2_by_name = load_ps2_functions()
    print(f"  Loaded {len(ps2_names)} unique PS2 function names "
          f"across {len(ps2_by_file)} source files")

    print("[4/6] Loading DWARF dump...")
    dwarf_funcs, dwarf_by_name, dwarf_label_to_func, dwarf_compile_units = \
        load_dwarf_functions()
    print(f"  Loaded {len(dwarf_funcs)} DWARF functions, "
          f"{len(dwarf_label_to_func)} label addresses, "
          f"{len(dwarf_compile_units)} compile units")

    print("[5/6] Parsing GC decompiled code for string references...")
    gc_func_info = parse_gc_decompiled_functions()
    print(f"  Parsed {len(gc_func_info)} GC functions with string/call info")

    # --- Run matching strategies ---
    all_matches = {}
    all_details = {}
    strategy_counts = {}

    print("\n[6/6] Running matching strategies...\n")

    # Strategy 1: String-based matching
    print("  Strategy 1: String-based matching (error messages)...")
    s1_matches, s1_details = strategy_string_match(
        gc_func_info, gc_strings, ps2_names)
    for addr, name in s1_matches.items():
        if addr not in all_matches:
            all_matches[addr] = name
            all_details[addr] = f"[S1] {s1_details[addr]}"
    strategy_counts["S1: String error msgs"] = len(s1_matches)
    print(f"    -> {len(s1_matches)} matches")

    # Strategy 2: Direct string name match
    print("  Strategy 2: Direct string name match...")
    s2_matches, s2_details = strategy_direct_string_name(
        gc_func_info, gc_strings, ps2_names)
    new_s2 = 0
    for addr, name in s2_matches.items():
        if addr not in all_matches:
            all_matches[addr] = name
            all_details[addr] = f"[S2] {s2_details[addr]}"
            new_s2 += 1
    strategy_counts["S2: Direct string (total)"] = len(s2_matches)
    strategy_counts["S2: Direct string (new)"] = new_s2
    print(f"    -> {len(s2_matches)} matches ({new_s2} new)")

    # Strategy 3: DWARF label + compile unit ordering
    print("  Strategy 3: DWARF compile unit ordering...")
    s3_matches, s3_details = strategy_dwarf_ordering(
        gc_funcs_list, gc_by_addr, dwarf_label_to_func,
        dwarf_compile_units, dwarf_by_name, ps2_by_name, all_matches)
    new_s3 = 0
    for addr, name in s3_matches.items():
        if addr not in all_matches:
            all_matches[addr] = name
            all_details[addr] = f"[S3] {s3_details[addr]}"
            new_s3 += 1
    strategy_counts["S3: DWARF ordering (total)"] = len(s3_matches)
    strategy_counts["S3: DWARF ordering (new)"] = new_s3
    print(f"    -> {len(s3_matches)} matches ({new_s3} new)")

    # Strategy 4: Source file grouping (now uses S1-S3 matches as anchors)
    print("  Strategy 4: Source file grouping + alignment...")
    s4_matches, s4_details = strategy_source_file_grouping(
        gc_func_info, gc_funcs_list, gc_by_addr, gc_strings,
        ps2_by_file, ps2_by_name, all_matches)
    new_s4 = 0
    for addr, name in s4_matches.items():
        if addr not in all_matches:
            all_matches[addr] = name
            all_details[addr] = f"[S4] {s4_details[addr]}"
            new_s4 += 1
    strategy_counts["S4: Source file align (total)"] = len(s4_matches)
    strategy_counts["S4: Source file align (new)"] = new_s4
    print(f"    -> {len(s4_matches)} matches ({new_s4} new)")

    # Strategy 5: Unique size matching
    print("  Strategy 5: Unique size matching...")
    s5_matches, s5_details = strategy_unique_size_match(
        gc_funcs_list, gc_by_addr, ps2_by_name, all_matches)
    new_s5 = 0
    for addr, name in s5_matches.items():
        if addr not in all_matches:
            all_matches[addr] = name
            all_details[addr] = f"[S5] {s5_details[addr]}"
            new_s5 += 1
    strategy_counts["S5: Unique size (total)"] = len(s5_matches)
    strategy_counts["S5: Unique size (new)"] = new_s5
    print(f"    -> {len(s5_matches)} matches ({new_s5} new)")

    # --- Build params map ---
    print("\nBuilding parameter/locals map from DWARF...")
    params_map = build_params_map(all_matches, dwarf_by_name)
    print(f"  {len(params_map)} functions have DWARF param/local info")

    # --- Write outputs ---
    print(f"\nWriting outputs...")
    os.makedirs(MAPS, exist_ok=True)

    name_map = dict(sorted(all_matches.items()))
    with open(OUT_NAME_MAP, "w", encoding="utf-8") as f:
        json.dump(name_map, f, indent=2)
    print(f"  -> {OUT_NAME_MAP}")

    with open(OUT_PARAMS_MAP, "w", encoding="utf-8") as f:
        json.dump(params_map, f, indent=2)
    print(f"  -> {OUT_PARAMS_MAP}")

    # --- Print statistics ---
    print("\n" + "=" * 70)
    print("RESULTS SUMMARY")
    print("=" * 70)
    print(f"\nTotal GC functions:        {len(gc_funcs_list)}")
    print(f"Total matched:             {len(all_matches)}")
    print(f"Match rate:                {100*len(all_matches)/len(gc_funcs_list):.1f}%")
    print(f"Functions with DWARF info: {len(params_map)}")
    print(f"\nMatches by strategy:")
    for key, count in sorted(strategy_counts.items()):
        print(f"  {key}: {count}")

    # Sample matches from each strategy
    print(f"\n{'='*70}")
    print("SAMPLE MATCHES")
    print("=" * 70)

    by_strategy = defaultdict(list)
    for addr, detail in all_details.items():
        prefix = detail[:4]
        by_strategy[prefix].append((addr, all_matches[addr], detail))

    for prefix in sorted(by_strategy.keys()):
        items = by_strategy[prefix]
        print(f"\n{prefix} ({len(items)} matches):")
        for addr, name, detail in items[:10]:
            size = gc_by_addr.get(addr, {}).get("size", "?")
            print(f"  FUN_{addr} -> {name} (size={size}) -- {detail[5:]}")
        if len(items) > 10:
            print(f"  ... and {len(items) - 10} more")

    # Show functions with DWARF params
    print(f"\n{'='*70}")
    print("SAMPLE FUNCTIONS WITH DWARF PARAMS/LOCALS")
    print("=" * 70)
    for name, info in list(params_map.items())[:10]:
        params = info.get("params", [])
        locals_ = info.get("locals", [])
        param_str = ", ".join(f"{p['type']} {p['name']}" for p in params)
        print(f"\n  {name}({param_str})")
        if locals_:
            for lv in locals_[:5]:
                print(f"    local: {lv['type']} {lv['name']}")
            if len(locals_) > 5:
                print(f"    ... +{len(locals_) - 5} more locals")

    print(f"\n{'='*70}")
    print("Done!")
    print("=" * 70)


if __name__ == "__main__":
    main()
