#!/usr/bin/env python3
"""Create a queue of proven demo bitmatches mapped to real function names."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--slices", default="config/GC_USA/slices_demo.json", help="Demo slices JSON relative to decomp/")
    parser.add_argument(
        "--functions-json",
        default=r"D:/projects/ps2/crash_bandicoot/analysis/ghidra/raw/main.dol_functions.json",
        help="Ghidra functions JSON",
    )
    parser.add_argument("--out", default="config/GC_USA/demo_match_queue.json", help="Output queue JSON relative to decomp/")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    slices_path = (root / args.slices).resolve()
    funcs_path = Path(args.functions_json).resolve()
    out_path = (root / args.out).resolve()

    slices = json.loads(slices_path.read_text(encoding="utf-8"))
    funcs = json.loads(funcs_path.read_text(encoding="utf-8"))
    by_addr = {int(str(f.get("address")), 16): f for f in funcs if f.get("address")}

    queue: list[dict] = []
    for s in slices:
        addr = int(str(s["address"]), 16)
        fn = by_addr.get(addr)
        if not fn:
            continue
        queue.append(
            {
                "name": fn["name"],
                "address": f"0x{addr:08X}",
                "size": int(fn.get("size") or s.get("size") or 0),
                "source": "",
                "symbol": s["symbol"],
                "status": "matched",
                "last_result": {
                    "timestamp": "imported-from-demo-slices",
                    "match": True,
                    "build_object": s["object"],
                },
            }
        )

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(json.dumps(queue, indent=2) + "\n", encoding="utf-8")
    print(f"Wrote {out_path}")
    print(f"Entries: {len(queue)}")


if __name__ == "__main__":
    main()
