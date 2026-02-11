#!/usr/bin/env python3
"""Compare configured DOL slices against symbols in compiled object files."""

from __future__ import annotations

import argparse
import json
import struct
import sys
from pathlib import Path


def u32be(data: bytes, off: int) -> int:
    return struct.unpack_from(">I", data, off)[0]


def read_dol_bytes(dol_data: bytes, address: int, size: int) -> bytes:
    text_off = [u32be(dol_data, 0x00 + i * 4) for i in range(7)]
    text_addr = [u32be(dol_data, 0x48 + i * 4) for i in range(7)]
    text_size = [u32be(dol_data, 0x90 + i * 4) for i in range(7)]

    for off, addr, length in zip(text_off, text_addr, text_size):
        if length and addr <= address and address + size <= addr + length:
            start = off + (address - addr)
            return dol_data[start : start + size]
    raise ValueError(f"Address range not fully in DOL text sections: 0x{address:08X}+0x{size:X}")


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

    best = None
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
        sec_off = sec["offset"]
        sym_off = sec_off + st_value
        sym_size = st_size if st_size else size
        best = data[sym_off : sym_off + min(size, sym_size)]
        break

    if best is None:
        text = find_section(sections, ".text")
        best = data[text["offset"] : text["offset"] + min(size, text["size"])]

    if len(best) < size:
        raise ValueError(f"Object bytes too short for {symbol}: expected {size}, got {len(best)}")
    return best[:size]


def words_hex(data: bytes) -> list[str]:
    out: list[str] = []
    for i in range(0, len(data), 4):
        out.append(f"{struct.unpack_from('>I', data, i)[0]:08x}")
    return out


def resolve_candidate(path_str: str, root: Path, config_parent: Path) -> Path:
    p = Path(path_str)
    if p.is_absolute():
        return p
    root_first = (root / p).resolve()
    if root_first.exists():
        return root_first
    return (config_parent / p).resolve()


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--dol",
        default="../../crash_bandicoot/roms/gc/usa_v1.00/extracted/main.dol",
        help="Path to reference main.dol",
    )
    parser.add_argument(
        "--slices",
        default="config/GC_USA/slices_demo.json",
        help="Slice config JSON",
    )
    parser.add_argument(
        "--json-out",
        default=None,
        help="Optional JSON result output path",
    )
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    dol_path = (root / args.dol).resolve()
    slices_path = (root / args.slices).resolve()

    dol_data = dol_path.read_bytes()
    slices = json.loads(slices_path.read_text(encoding="utf-8"))

    results = []
    all_ok = True
    for item in slices:
        name = item["name"]
        addr = int(str(item["address"]), 16)
        size = int(item["size"])
        symbol = item.get("symbol", name)
        obj_path = resolve_candidate(item["object"], root, slices_path.parent)

        dol_b = read_dol_bytes(dol_data, addr, size)
        obj_b = read_obj_symbol_bytes(obj_path, symbol, size)
        match = dol_b == obj_b
        all_ok = all_ok and match

        dol_words = words_hex(dol_b)
        obj_words = words_hex(obj_b)
        mismatches = [i for i, (a, b) in enumerate(zip(dol_words, obj_words)) if a != b]
        results.append(
            {
                "name": name,
                "address": f"0x{addr:08X}",
                "size": size,
                "object": str(obj_path),
                "symbol": symbol,
                "match": match,
                "mismatch_word_indices": mismatches,
            }
        )

    for r in results:
        state = "MATCH" if r["match"] else "MISMATCH"
        print(f"{r['name']} @ {r['address']} size={r['size']}: {state}")
        if r["mismatch_word_indices"]:
            print(f"  mismatch word indices: {r['mismatch_word_indices']}")

    if args.json_out:
        out_path = (root / args.json_out).resolve()
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(json.dumps(results, indent=2) + "\n", encoding="utf-8")
        print(f"Wrote {out_path}")

    sys.exit(0 if all_ok else 1)


if __name__ == "__main__":
    main()
