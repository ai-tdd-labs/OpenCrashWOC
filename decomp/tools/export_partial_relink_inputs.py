#!/usr/bin/env python3
"""Export partial relink inputs from mixed manifest (prototype stage)."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", default="build/GC_USA/mixed_manifest.json", help="Mixed manifest JSON")
    parser.add_argument("--out-json", default="build/GC_USA/partial_relink_inputs.json", help="Output JSON")
    parser.add_argument("--out-ld", default="build/GC_USA/partial_relink.ld", help="Output linker script skeleton")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    manifest_path = (root / args.manifest).resolve()
    out_json = (root / args.out_json).resolve()
    out_ld = (root / args.out_ld).resolve()

    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    c_entries = [m for m in manifest if m.get("kind") == "c_obj"]
    c_entries.sort(key=lambda x: int(str(x["address"]), 16))

    inputs = {
        "count": len(c_entries),
        "entries": [
            {
                "name": e["name"],
                "address": e["address"],
                "size": e["size"],
                "object": e["path"],
                "symbol": e.get("symbol", e["name"]),
            }
            for e in c_entries
        ],
    }
    out_json.parent.mkdir(parents=True, exist_ok=True)
    out_json.write_text(json.dumps(inputs, indent=2) + "\n", encoding="utf-8")

    lines: list[str] = []
    lines.append("/* Auto-generated partial relink linker script skeleton. */")
    lines.append("SECTIONS")
    lines.append("{")
    for e in c_entries:
        addr = int(str(e["address"]), 16)
        obj = e["path"]
        # Prototype: place each object .text at expected function address.
        lines.append(f"  .text_{e['name']} 0x{addr:08X} : {{ {obj}(.text) }}")
    lines.append("}")
    out_ld.write_text("\n".join(lines) + "\n", encoding="utf-8")

    print(f"Wrote {out_json}")
    print(f"Wrote {out_ld}")
    print(f"C relink entries: {len(c_entries)}")


if __name__ == "__main__":
    main()
