#!/usr/bin/env python3
"""Build a candidate DOL by patching matched queue entries into a reference DOL."""

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


def elf32_be_sections(obj_data: bytes) -> list[dict]:
    if obj_data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")
    if obj_data[4] != 1 or obj_data[5] != 2:
        raise ValueError("Only 32-bit big-endian ELF is supported")

    e_shoff = struct.unpack_from(">I", obj_data, 0x20)[0]
    e_shentsize = struct.unpack_from(">H", obj_data, 0x2E)[0]
    e_shnum = struct.unpack_from(">H", obj_data, 0x30)[0]
    e_shstrndx = struct.unpack_from(">H", obj_data, 0x32)[0]

    sections: list[dict] = []
    for i in range(e_shnum):
        o = e_shoff + i * e_shentsize
        sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size, sh_link, sh_info, sh_addralign, sh_entsize = struct.unpack_from(
            ">IIIIIIIIII", obj_data, o
        )
        sections.append(
            {
                "name_off": sh_name,
                "type": sh_type,
                "flags": sh_flags,
                "addr": sh_addr,
                "offset": sh_offset,
                "size": sh_size,
                "link": sh_link,
                "info": sh_info,
                "entsize": sh_entsize,
            }
        )

    shstr = sections[e_shstrndx]
    strtab = obj_data[shstr["offset"] : shstr["offset"] + shstr["size"]]
    for s in sections:
        n = s["name_off"]
        end = strtab.find(b"\x00", n)
        s["name"] = strtab[n:end].decode("ascii", "ignore") if end != -1 else ""
    return sections


def find_section(sections: list[dict], name: str) -> dict:
    for s in sections:
        if s.get("name") == name:
            return s
    raise ValueError(f"Section not found: {name}")


def read_obj_symbol_bytes(obj_path: Path, symbol: str, size: int) -> bytes:
    data = obj_path.read_bytes()
    sections = elf32_be_sections(data)
    symtab = find_section(sections, ".symtab")
    strtab = sections[symtab["link"]]
    strings = data[strtab["offset"] : strtab["offset"] + strtab["size"]]
    entsize = symtab["entsize"] or 16

    for off in range(symtab["offset"], symtab["offset"] + symtab["size"], entsize):
        st_name, st_value, st_size, st_info, st_other, st_shndx = struct.unpack_from(">IIIBBH", data, off)
        if st_name == 0:
            continue
        end = strings.find(b"\x00", st_name)
        name = strings[st_name:end].decode("ascii", "ignore") if end != -1 else ""
        if name != symbol:
            continue
        sec = sections[st_shndx]
        sym_off = sec["offset"] + st_value
        sym_size = st_size if st_size else size
        out = data[sym_off : sym_off + min(size, sym_size)]
        if len(out) < size:
            raise ValueError(f"Symbol too short for patch: {name}")
        return out

    text = find_section(sections, ".text")
    out = data[text["offset"] : text["offset"] + min(size, text["size"])]
    if len(out) < size:
        raise ValueError(f".text too short for patch: {obj_path}")
    return out


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--queue", default="config/GC_USA/leaf_queue.json", help="Queue JSON path relative to decomp/")
    parser.add_argument(
        "--ref-dol",
        default="../../crash_bandicoot/roms/gc/usa_v1.00/extracted/main.dol",
        help="Reference DOL path relative to decomp/",
    )
    parser.add_argument("--out", default="build/GC_USA/main.candidate.dol", help="Output candidate DOL path")
    parser.add_argument(
        "--statuses",
        default="matched",
        help="Comma-separated queue statuses to patch (e.g. matched,mismatch)",
    )
    parser.add_argument(
        "--objects-root",
        default=".",
        help="Root for queue object paths (relative to decomp/)",
    )
    parser.add_argument("--slices-out", default=None, help="Optional JSON output of patched slices")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    queue_path = (root / args.queue).resolve()
    ref_dol_path = (root / args.ref_dol).resolve()
    out_path = (root / args.out).resolve()
    obj_root = (root / args.objects_root).resolve()
    allowed = {x.strip() for x in args.statuses.split(",") if x.strip()}

    queue = json.loads(queue_path.read_text(encoding="utf-8"))
    dol = bytearray(ref_dol_path.read_bytes())

    patched = []
    for item in queue:
        if item.get("status") not in allowed:
            continue
        name = item["name"]
        address = int(str(item["address"]), 16)
        size = int(item["size"])
        symbol = item.get("symbol", name)
        obj_rel = item.get("last_result", {}).get("build_object", f"build/leaf_o/{name}.o")
        obj_path = (obj_root / obj_rel).resolve()
        if not obj_path.exists():
            continue
        code = read_obj_symbol_bytes(obj_path, symbol, size)
        off = find_dol_offset(dol, address, size)
        dol[off : off + size] = code
        patched.append(
            {
                "name": name,
                "address": f"0x{address:08X}",
                "size": size,
                "object": str(obj_path),
                "symbol": symbol,
            }
        )

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_bytes(bytes(dol))
    print(f"Wrote {out_path}")
    print(f"Patched slices: {len(patched)}")

    if args.slices_out:
        slices_out = (root / args.slices_out).resolve()
        slices_out.parent.mkdir(parents=True, exist_ok=True)
        slices_out.write_text(json.dumps(patched, indent=2) + "\n", encoding="utf-8")
        print(f"Wrote {slices_out}")


if __name__ == "__main__":
    main()
