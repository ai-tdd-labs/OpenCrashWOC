#!/usr/bin/env python3
"""Compare two DOL files and emit mismatch summary."""

from __future__ import annotations

import argparse
import hashlib
import struct
from pathlib import Path


def u32be(data: bytes, off: int) -> int:
    return struct.unpack_from(">I", data, off)[0]


def sha1_hex(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def dol_sections(dol: bytes) -> list[tuple[str, int, int, int]]:
    out: list[tuple[str, int, int, int]] = []
    for i in range(7):
        off = u32be(dol, 0x00 + i * 4)
        addr = u32be(dol, 0x48 + i * 4)
        size = u32be(dol, 0x90 + i * 4)
        if size:
            out.append((f"text{i}", off, addr, size))
    for i in range(11):
        off = u32be(dol, 0x1C + i * 4)
        addr = u32be(dol, 0x64 + i * 4)
        size = u32be(dol, 0xAC + i * 4)
        if size:
            out.append((f"data{i}", off, addr, size))
    return out


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--base",
        default="../../crash_bandicoot/roms/gc/usa_v1.00/extracted/main.dol",
        help="Reference/base DOL path (relative to decomp/)",
    )
    parser.add_argument("--target", required=True, help="Candidate/target DOL path (relative to decomp/)")
    parser.add_argument("--out", default=None, help="Optional report output file")
    parser.add_argument("--max-lines", type=int, default=40, help="Max mismatch lines to print")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    base_path = (root / args.base).resolve()
    target_path = (root / args.target).resolve()
    base = base_path.read_bytes()
    target = target_path.read_bytes()

    lines: list[str] = []
    lines.append(f"Base:   {base_path}")
    lines.append(f"Target: {target_path}")
    lines.append(f"Base SHA1:   {sha1_hex(base)}")
    lines.append(f"Target SHA1: {sha1_hex(target)}")

    if len(base) != len(target):
        lines.append(f"Size mismatch: base={len(base)} target={len(target)}")
    max_len = min(len(base), len(target))

    mismatches = [i for i in range(max_len) if base[i] != target[i]]
    lines.append(f"Byte mismatches: {len(mismatches)}")

    if not mismatches and len(base) == len(target):
        lines.append("IDENTICAL")
    else:
        for off in mismatches[: args.max_lines]:
            lines.append(f"  file_off=0x{off:06X} base={base[off]:02X} target={target[off]:02X}")

    # Section-level mismatch counts using base DOL headers.
    section_counts: list[str] = []
    for name, off, addr, size in dol_sections(base):
        end = off + size
        if end > max_len:
            continue
        count = 0
        for i in range(off, end):
            if base[i] != target[i]:
                count += 1
        if count:
            section_counts.append(f"  {name} @0x{addr:08X} size=0x{size:X}: {count}")
    if section_counts:
        lines.append("Section mismatch counts:")
        lines.extend(section_counts)

    text = "\n".join(lines) + "\n"
    print(text, end="")
    if args.out:
        out_path = (root / args.out).resolve()
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(text, encoding="utf-8")
        print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
