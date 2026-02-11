#!/usr/bin/env python3
"""Build/update leaf candidate queue from Ghidra exports and extract candidate C files."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


def extract_function_block(decomp_text: str, name: str) -> str | None:
    # Match a top-level function block in Ghidra decompiled C.
    pattern = re.compile(
        rf"(?ms)^([^\n]*\b{name}\b[^\n]*\n\s*\n\{{.*?^\}})\s*$"
    )
    m = pattern.search(decomp_text)
    return m.group(1) if m else None


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--functions-json",
        default=r"D:/projects/ps2/crash_bandicoot/analysis/ghidra/raw/main.dol_functions.json",
        help="Path to Ghidra function metadata JSON",
    )
    parser.add_argument(
        "--decompiled-c",
        default=r"D:/projects/ps2/crash_bandicoot/analysis/ghidra/raw/main.dol_decompiled.c",
        help="Path to Ghidra decompiled C file",
    )
    parser.add_argument("--queue", default="config/GC_USA/leaf_queue.json", help="Queue file path (relative to decomp/)")
    parser.add_argument("--min-size", type=int, default=64, help="Min function size")
    parser.add_argument("--max-size", type=int, default=160, help="Max function size")
    parser.add_argument("--limit", type=int, default=10, help="Max new candidates to add")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    queue_path = (root / args.queue).resolve()
    funcs_path = Path(args.functions_json).resolve()
    decomp_path = Path(args.decompiled_c).resolve()

    funcs = json.loads(funcs_path.read_text(encoding="utf-8"))
    decomp_text = decomp_path.read_text(encoding="utf-8", errors="ignore")

    queue = json.loads(queue_path.read_text(encoding="utf-8")) if queue_path.exists() else []
    existing_names = {x.get("name") for x in queue}

    added = 0
    for f in funcs:
        if added >= args.limit:
            break
        name = f.get("name", "")
        if not name.startswith("FUN_"):
            continue
        if name in existing_names:
            continue
        if f.get("external") or f.get("thunk"):
            continue
        calls = f.get("calls", [])
        if calls:
            continue
        size = int(f.get("size") or 0)
        if size < args.min_size or size > args.max_size:
            continue

        block = extract_function_block(decomp_text, name)
        if not block:
            continue

        cand_dir = root / "candidates" / name
        cand_dir.mkdir(parents=True, exist_ok=True)
        source_rel = f"candidates/{name}/{name}.c"
        (cand_dir / f"{name}.c").write_text(block + "\n", encoding="utf-8")

        queue.append(
            {
                "name": name,
                "address": "0x" + str(f.get("address", "")).upper(),
                "size": size,
                "kind": "leaf",
                "status": "todo",
                "symbol": name,
                "source": source_rel,
                "notes": "Auto-imported from Ghidra leaf filter.",
            }
        )
        existing_names.add(name)
        added += 1

    queue_path.parent.mkdir(parents=True, exist_ok=True)
    queue_path.write_text(json.dumps(queue, indent=2) + "\n", encoding="utf-8")
    print(f"Queue: {queue_path}")
    print(f"Added: {added}")
    print(f"Total: {len(queue)}")


if __name__ == "__main__":
    main()
