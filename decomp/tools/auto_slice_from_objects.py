#!/usr/bin/env python3
"""Auto-generate slice definitions by searching object .text bytes in a DOL."""

from __future__ import annotations

import argparse
import json
import struct
from pathlib import Path


def u32be(data: bytes, off: int) -> int:
    return struct.unpack_from(">I", data, off)[0]


def dol_text_sections(dol: bytes) -> list[tuple[int, int, bytes]]:
    text_off = [u32be(dol, 0x00 + i * 4) for i in range(7)]
    text_addr = [u32be(dol, 0x48 + i * 4) for i in range(7)]
    text_size = [u32be(dol, 0x90 + i * 4) for i in range(7)]
    out: list[tuple[int, int, bytes]] = []
    for off, addr, size in zip(text_off, text_addr, text_size):
        if size:
            out.append((off, addr, dol[off : off + size]))
    return out


def elf_text_bytes(obj_path: Path) -> bytes:
    data = obj_path.read_bytes()
    if data[:4] != b"\x7fELF":
        raise ValueError(f"Not ELF: {obj_path}")
    e_shoff = struct.unpack_from(">I", data, 0x20)[0]
    e_shentsize = struct.unpack_from(">H", data, 0x2E)[0]
    e_shnum = struct.unpack_from(">H", data, 0x30)[0]
    e_shstrndx = struct.unpack_from(">H", data, 0x32)[0]

    sections = []
    for i in range(e_shnum):
        o = e_shoff + i * e_shentsize
        sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size, sh_link, sh_info, sh_addralign, sh_entsize = struct.unpack_from(
            ">IIIIIIIIII", data, o
        )
        sections.append((sh_name, sh_offset, sh_size))

    str_off, str_sz = sections[e_shstrndx][1], sections[e_shstrndx][2]
    shstr = data[str_off : str_off + str_sz]

    def sec_name(idx: int) -> str:
        n = sections[idx][0]
        e = shstr.find(b"\x00", n)
        return shstr[n:e].decode("ascii", "ignore") if e != -1 else ""

    for i in range(e_shnum):
        if sec_name(i) == ".text":
            off, sz = sections[i][1], sections[i][2]
            return data[off : off + sz]

    raise ValueError(f".text not found: {obj_path}")


def primary_text_symbol(obj_path: Path) -> str:
    data = obj_path.read_bytes()
    e_shoff = struct.unpack_from(">I", data, 0x20)[0]
    e_shentsize = struct.unpack_from(">H", data, 0x2E)[0]
    e_shnum = struct.unpack_from(">H", data, 0x30)[0]
    e_shstrndx = struct.unpack_from(">H", data, 0x32)[0]

    sections = []
    for i in range(e_shnum):
        o = e_shoff + i * e_shentsize
        sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size, sh_link, sh_info, sh_addralign, sh_entsize = struct.unpack_from(
            ">IIIIIIIIII", data, o
        )
        sections.append(
            {
                "name_off": sh_name,
                "type": sh_type,
                "offset": sh_offset,
                "size": sh_size,
                "link": sh_link,
                "entsize": sh_entsize,
            }
        )

    shstr = sections[e_shstrndx]
    shstr_data = data[shstr["offset"] : shstr["offset"] + shstr["size"]]
    for s in sections:
        n = s["name_off"]
        e = shstr_data.find(b"\x00", n)
        s["name"] = shstr_data[n:e].decode("ascii", "ignore") if e != -1 else ""

    symtab = None
    text_idx = None
    for i, s in enumerate(sections):
        if s["name"] == ".symtab":
            symtab = s
        if s["name"] == ".text":
            text_idx = i
    if symtab is None or text_idx is None:
        return obj_path.stem

    strtab = sections[symtab["link"]]
    strdata = data[strtab["offset"] : strtab["offset"] + strtab["size"]]
    entsize = symtab["entsize"] or 16

    for off in range(symtab["offset"], symtab["offset"] + symtab["size"], entsize):
        st_name, st_value, st_size, st_info, st_other, st_shndx = struct.unpack_from(">IIIBBH", data, off)
        if st_name == 0 or st_shndx != text_idx:
            continue
        bind = st_info >> 4
        typ = st_info & 0x0F
        if bind == 0 or typ != 2:  # skip local/non-function
            continue
        end = strdata.find(b"\x00", st_name)
        if end == -1:
            continue
        name = strdata[st_name:end].decode("ascii", "ignore")
        if name:
            return name

    return obj_path.stem


def find_hits(pattern: bytes, text_sections: list[tuple[int, int, bytes]]) -> list[int]:
    hits: list[int] = []
    for _, base_addr, blob in text_sections:
        start = 0
        while True:
            idx = blob.find(pattern, start)
            if idx < 0:
                break
            hits.append(base_addr + idx)
            start = idx + 1
    return hits


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--dol",
        default="../../crash_bandicoot/roms/gc/usa_v1.00/extracted/main.dol",
        help="Path to reference DOL (relative to decomp root by default)",
    )
    parser.add_argument(
        "--objects-dir",
        default="build/demo_o",
        help="Directory with object files (relative to decomp root by default)",
    )
    parser.add_argument(
        "--glob",
        default="*.o",
        help="Object glob inside --objects-dir",
    )
    parser.add_argument(
        "--out",
        default="config/GC_USA/slices_demo.json",
        help="Output slices JSON path",
    )
    parser.add_argument(
        "--allow-ambiguous",
        action="store_true",
        help="If set, keep objects even when bytes appear multiple times in DOL (pick first hit)",
    )
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]  # decomp/
    dol_path = (root / args.dol).resolve()
    obj_dir = (root / args.objects_dir).resolve()
    out_path = (root / args.out).resolve()

    dol = dol_path.read_bytes()
    sections = dol_text_sections(dol)
    objects = sorted(obj_dir.glob(args.glob))
    if not objects:
        raise SystemExit(f"No objects found: {obj_dir} ({args.glob})")

    slices = []
    skipped = []
    for obj in objects:
        name = obj.stem
        text = elf_text_bytes(obj)
        hits = find_hits(text, sections)
        if len(hits) != 1 and not args.allow_ambiguous:
            skipped.append((name, len(hits)))
            continue
        if not hits:
            skipped.append((name, 0))
            continue
        addr = hits[0]
        slices.append(
            {
                "name": name,
                "address": f"0x{addr:08X}",
                "size": len(text),
                "object": str(obj.relative_to(root)).replace("\\", "/"),
                "symbol": primary_text_symbol(obj),
            }
        )

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(json.dumps(slices, indent=2) + "\n", encoding="utf-8")

    print(f"Wrote {out_path}")
    print(f"Included: {len(slices)}")
    if skipped:
        print("Skipped:")
        for name, count in skipped:
            print(f"  - {name}: hits={count}")


if __name__ == "__main__":
    main()
