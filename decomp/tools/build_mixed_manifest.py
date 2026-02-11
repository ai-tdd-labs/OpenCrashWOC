#!/usr/bin/env python3
"""Build a full mixed manifest: C object slices where available, ASM bins otherwise."""

from __future__ import annotations

import argparse
import json
import struct
from pathlib import Path


def u32be(data: bytes, off: int) -> int:
    return struct.unpack_from(">I", data, off)[0]


def find_dol_offset(dol_data: bytes, address: int, size: int) -> int:
    text_off = [u32be(dol_data, 0x00 + i * 4) for i in range(7)]
    text_addr = [u32be(dol_data, 0x48 + i * 4) for i in range(7)]
    text_size = [u32be(dol_data, 0x90 + i * 4) for i in range(7)]
    for off, addr, length in zip(text_off, text_addr, text_size):
        if length and addr <= address and address + size <= addr + length:
            return off + (address - addr)
    raise ValueError(f"Address not fully in text section: 0x{address:08X}+0x{size:X}")


def load_queue_index(queue_items: list[dict]) -> dict[str, dict]:
    return {str(x.get("name")): x for x in queue_items}


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--functions-json",
        default=r"D:/projects/ps2/crash_bandicoot/analysis/ghidra/raw/main.dol_functions.json",
        help="Ghidra functions JSON",
    )
    parser.add_argument(
        "--ref-dol",
        default="../../crash_bandicoot/roms/gc/usa_v1.00/extracted/main.dol",
        help="Reference DOL path relative to decomp/",
    )
    parser.add_argument("--out-manifest", default="build/GC_USA/mixed_manifest.json", help="Output manifest JSON")
    parser.add_argument("--asm-dir", default="build/GC_USA/asm_bins", help="Output directory for ASM bins")
    parser.add_argument(
        "--queue-files",
        default="config/GC_USA/leaf_queue.json,config/GC_USA/project_match_queue.json",
        help="Comma-separated queue JSON files (relative to decomp/) used as C candidates",
    )
    parser.add_argument(
        "--c-statuses",
        default="matched",
        help="Comma-separated queue statuses that should use C objects (e.g. matched,mismatch)",
    )
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    funcs_path = Path(args.functions_json).resolve()
    ref_dol_path = (root / args.ref_dol).resolve()
    out_manifest = (root / args.out_manifest).resolve()
    asm_dir = (root / args.asm_dir).resolve()

    funcs = json.loads(funcs_path.read_text(encoding="utf-8"))
    queue_items: list[dict] = []
    for qf in [x.strip() for x in args.queue_files.split(",") if x.strip()]:
        qp = (root / qf).resolve()
        if qp.exists():
            queue_items.extend(json.loads(qp.read_text(encoding="utf-8")))
    queue = load_queue_index(queue_items)
    c_statuses = {x.strip() for x in args.c_statuses.split(",") if x.strip()}
    dol = ref_dol_path.read_bytes()

    asm_dir.mkdir(parents=True, exist_ok=True)
    manifest: list[dict] = []
    c_count = 0
    asm_count = 0

    for f in funcs:
        if f.get("external") or f.get("thunk"):
            continue
        name = str(f.get("name", ""))
        if not name:
            continue
        size = int(f.get("size") or 0)
        if size <= 0:
            continue
        address = int(str(f.get("address")), 16)

        q = queue.get(name)
        if q and q.get("status") in c_statuses:
            obj_rel = q.get("last_result", {}).get("build_object", f"build/leaf_o/{name}.o")
            manifest.append(
                {
                    "name": name,
                    "address": f"0x{address:08X}",
                    "size": size,
                    "kind": "c_obj",
                    "symbol": q.get("symbol", name),
                    "path": obj_rel,
                }
            )
            c_count += 1
            continue

        off = find_dol_offset(dol, address, size)
        blob = dol[off : off + size]
        asm_name = f"{name}_{address:08X}.bin"
        asm_path = asm_dir / asm_name
        asm_path.write_bytes(blob)
        manifest.append(
            {
                "name": name,
                "address": f"0x{address:08X}",
                "size": size,
                "kind": "asm_bin",
                "path": str(asm_path.relative_to(root)).replace("\\", "/"),
            }
        )
        asm_count += 1

    out_manifest.parent.mkdir(parents=True, exist_ok=True)
    out_manifest.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"Wrote {out_manifest}")
    print(f"Total entries: {len(manifest)}")
    print(f"C obj entries: {c_count}")
    print(f"ASM bin entries: {asm_count}")


if __name__ == "__main__":
    main()
