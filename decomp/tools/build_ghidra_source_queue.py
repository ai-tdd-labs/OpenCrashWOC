#!/usr/bin/env python3
"""Build a queue by mapping functions to real source files via Ghidra raw decompiled dump."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


FUNC_TAG_RE = re.compile(r"^/\*\s*([A-Za-z_]\w*)\s*@\s*([0-9a-fA-F]{8})\s*\*/\s*$")
SRC_LINE_RE = re.compile(r"^\s*/\*\s*Source:\s*code/(.+?\.c)\s*$")
FUNC_DEF_RE = re.compile(r"(?m)^[ \t]*[A-Za-z_][\w \t\*\(\),\[\]]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{")


def parse_ghidra_source_map(decomp_c: Path) -> dict[str, str]:
    lines = decomp_c.read_text(encoding="utf-8", errors="ignore").splitlines()
    out: dict[str, str] = {}
    for i, line in enumerate(lines):
        m = FUNC_TAG_RE.match(line)
        if not m:
            continue
        name = m.group(1)
        # Scan a small window after function tag for Source line.
        src = None
        for j in range(i + 1, min(i + 30, len(lines))):
            sm = SRC_LINE_RE.match(lines[j])
            if sm:
                src = sm.group(1).strip().replace("\\", "/")
                break
            # Hit next function tag, stop search.
            if FUNC_TAG_RE.match(lines[j]):
                break
        if src:
            out[name] = src
    return out


def build_unique_name_to_source_map(code_root: Path) -> dict[str, str]:
    by_name: dict[str, set[str]] = {}
    skip = {"if", "for", "while", "switch", "return"}
    for c_path in sorted(code_root.rglob("*.c")):
        rel = c_path.relative_to(code_root).as_posix()
        text = c_path.read_text(encoding="utf-8", errors="ignore")
        for m in FUNC_DEF_RE.finditer(text):
            name = m.group(1)
            if name in skip:
                continue
            by_name.setdefault(name, set()).add(rel)
    uniq: dict[str, str] = {}
    for name, paths in by_name.items():
        if len(paths) == 1:
            uniq[name] = next(iter(paths))
    return uniq


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--functions-json",
        default=r"D:/projects/ps2/crash_bandicoot/analysis/ghidra/raw/main.dol_functions.json",
        help="Ghidra functions JSON with address/size",
    )
    parser.add_argument(
        "--ghidra-decompiled-c",
        default=r"D:/projects/ps2/crash_bandicoot/analysis/ghidra/raw/main.dol_decompiled.c",
        help="Ghidra raw decompiled C dump",
    )
    parser.add_argument(
        "--code-root",
        default="../code/src",
        help="OpenCrashWOC code root relative to decomp/",
    )
    parser.add_argument(
        "--out-queue",
        default="config/GC_USA/ghidra_source_queue.json",
        help="Output queue JSON relative to decomp/",
    )
    parser.add_argument("--max-size", type=int, default=512, help="Max function size to include")
    parser.add_argument("--limit", type=int, default=0, help="Optional max entries")
    args = parser.parse_args()

    decomp_root = Path(__file__).resolve().parents[1]
    code_root = (decomp_root / args.code_root).resolve()
    funcs = json.loads(Path(args.functions_json).resolve().read_text(encoding="utf-8"))
    src_map = parse_ghidra_source_map(Path(args.ghidra_decompiled_c).resolve())
    uniq_map = build_unique_name_to_source_map(code_root)

    out: list[dict] = []
    for f in funcs:
        name = str(f.get("name", ""))
        if not name or name.startswith("FUN_"):
            continue
        if f.get("external") or f.get("thunk"):
            continue
        size = int(f.get("size") or 0)
        if size <= 0 or size > args.max_size:
            continue
        rel = src_map.get(name)
        if not rel:
            rel = uniq_map.get(name)
        if not rel:
            continue
        src_path = (code_root / rel).resolve()
        if not src_path.exists():
            continue
        out.append(
            {
                "name": name,
                "address": "0x" + str(f.get("address", "")).upper(),
                "size": size,
                "kind": "ghidra_source",
                "status": "todo",
                "symbol": name,
                "source": f"../code/src/{rel}",
                "notes": f"Mapped from Ghidra Source: code/{rel}",
            }
        )
        if args.limit and len(out) >= args.limit:
            break

    out_path = (decomp_root / args.out_queue).resolve()
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(json.dumps(out, indent=2) + "\n", encoding="utf-8")
    print(f"Wrote {out_path}")
    print(f"Entries: {len(out)}")


if __name__ == "__main__":
    main()
