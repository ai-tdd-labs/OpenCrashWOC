#!/usr/bin/env python3
"""Build a full candidate DOL from mixed manifest entries."""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
import subprocess
import tempfile
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


def load_elf_symbols(obj_data: bytes) -> list[dict]:
    sections = elf32_be_sections(obj_data)
    symtab = find_section(sections, ".symtab")
    strtab = sections[symtab["link"]]
    strings = obj_data[strtab["offset"] : strtab["offset"] + strtab["size"]]
    entsize = symtab["entsize"] or 16
    out: list[dict] = []
    for off in range(symtab["offset"], symtab["offset"] + symtab["size"], entsize):
        st_name, st_value, st_size, st_info, st_other, st_shndx = struct.unpack_from(">IIIBBH", obj_data, off)
        name = ""
        if st_name:
            end = strings.find(b"\x00", st_name)
            name = strings[st_name:end].decode("ascii", "ignore") if end != -1 else ""
        out.append(
            {
                "name": name,
                "value": st_value,
                "size": st_size,
                "info": st_info,
                "other": st_other,
                "shndx": st_shndx,
            }
        )
    return out


def read_text_section_bytes(obj_path: Path, size: int) -> bytes:
    data = obj_path.read_bytes()
    sections = elf32_be_sections(data)
    text = find_section(sections, ".text")
    out = data[text["offset"] : text["offset"] + min(size, text["size"])]
    if len(out) < size:
        return out + (b"\x00" * (size - len(out)))
    return out


def link_object_with_abs_symbols(
    obj_path: Path,
    func_addr: int,
    addr_by_name: dict[str, int],
    ngcld_path: Path,
    root: Path,
) -> Path | None:
    obj_data = obj_path.read_bytes()
    symbols = load_elf_symbols(obj_data)
    undef_names = sorted({s["name"] for s in symbols if s["shndx"] == 0 and s["name"]})
    mapped = []
    for name in undef_names:
        if name in addr_by_name:
            mapped.append((name, addr_by_name[name]))
    if not mapped:
        return None

    tmp_dir = Path(tempfile.mkdtemp(prefix="mixed_link_", dir=str((root / "build").resolve())))
    ld_script = tmp_dir / "link.ld"
    out_obj = tmp_dir / f"{obj_path.stem}.linked.o"
    lines = [
        "SECTIONS {",
        f"  .text 0x{func_addr:08X} : {{ {obj_path.as_posix()}(.text) }}",
        "}",
    ]
    for name, addr in mapped:
        lines.append(f"{name} = 0x{addr:08X};")
    ld_script.write_text("\n".join(lines) + "\n", encoding="utf-8")

    cmd = [str(ngcld_path), str(obj_path), "-T", str(ld_script), "-o", str(out_obj)]
    proc = subprocess.run(cmd, cwd=root, capture_output=True, text=True)
    if proc.returncode != 0 or not out_obj.exists():
        return None
    return out_obj


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
        out = data[sym_off : sym_off + min(size, st_size if st_size else size)]
        if len(out) < size:
            return out + (b"\x00" * (size - len(out)))
        return out

    text = find_section(sections, ".text")
    out = data[text["offset"] : text["offset"] + min(size, text["size"])]
    if len(out) < size:
        return out + (b"\x00" * (size - len(out)))
    return out


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", default="build/GC_USA/mixed_manifest.json", help="Manifest path relative to decomp/")
    parser.add_argument(
        "--ref-dol",
        default="../../crash_bandicoot/roms/gc/usa_v1.00/extracted/main.dol",
        help="Reference DOL path relative to decomp/",
    )
    parser.add_argument("--out", default="build/GC_USA/main.mixed.dol", help="Output mixed DOL path relative to decomp/")
    parser.add_argument("--report-out", default="build/GC_USA/mixed_build_report.json", help="Output report JSON")
    parser.add_argument(
        "--functions-json",
        default=r"D:/projects/ps2/crash_bandicoot/analysis/ghidra/raw/main.dol_functions.json",
        help="Function metadata JSON used to resolve extern symbol addresses for C relink.",
    )
    parser.add_argument(
        "--ngcld",
        default=r"D:/projects/ps2/crash_bandicoot/tools/compilers/ProDG-v3.5/ngcld.exe",
        help="Path to ngcld.exe for C object resolve-link step.",
    )
    parser.add_argument(
        "--no-resolve-link",
        dest="resolve_link",
        action="store_false",
        help="Disable relink-based extern symbol resolution for c_obj entries.",
    )
    parser.set_defaults(resolve_link=True)
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    manifest_path = (root / args.manifest).resolve()
    ref_dol_path = (root / args.ref_dol).resolve()
    out_path = (root / args.out).resolve()
    report_path = (root / args.report_out).resolve()

    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    dol = bytearray(ref_dol_path.read_bytes())
    funcs = json.loads(Path(args.functions_json).resolve().read_text(encoding="utf-8"))
    addr_by_name: dict[str, int] = {}
    for f in funcs:
        name = str(f.get("name", ""))
        addr_hex = str(f.get("address", ""))
        if not name or not addr_hex:
            continue
        try:
            addr_by_name[name] = int(addr_hex, 16)
        except ValueError:
            continue

    stats = {
        "total_entries": 0,
        "c_obj_entries": 0,
        "asm_bin_entries": 0,
        "patched_bytes": 0,
        "c_obj_bytes": 0,
        "asm_bin_bytes": 0,
        "skipped_entries": 0,
    }

    for item in manifest:
        stats["total_entries"] += 1
        kind = item.get("kind")
        address = int(str(item["address"]), 16)
        size = int(item["size"])
        off = find_dol_offset(dol, address, size)
        path = (root / item["path"]).resolve()
        if not path.exists():
            stats["skipped_entries"] += 1
            continue

        if kind == "c_obj":
            code: bytes
            linked_obj = None
            if args.resolve_link:
                linked_obj = link_object_with_abs_symbols(
                    path,
                    address,
                    addr_by_name,
                    Path(args.ngcld).resolve(),
                    root,
                )
            if linked_obj is not None:
                code = read_text_section_bytes(linked_obj, size)
            else:
                code = read_obj_symbol_bytes(path, item.get("symbol", item.get("name", "")), size)
            stats["c_obj_entries"] += 1
            stats["c_obj_bytes"] += size
        else:
            code = path.read_bytes()[:size]
            if len(code) < size:
                code = code + (b"\x00" * (size - len(code)))
            stats["asm_bin_entries"] += 1
            stats["asm_bin_bytes"] += size

        dol[off : off + size] = code
        stats["patched_bytes"] += size

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_bytes(bytes(dol))
    stats["out_sha1"] = hashlib.sha1(bytes(dol)).hexdigest()
    stats["ref_sha1"] = hashlib.sha1(ref_dol_path.read_bytes()).hexdigest()
    stats["identical_to_ref"] = stats["out_sha1"] == stats["ref_sha1"]

    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(stats, indent=2) + "\n", encoding="utf-8")

    print(f"Wrote {out_path}")
    print(f"Wrote {report_path}")
    print(f"Identical to ref: {stats['identical_to_ref']}")
    print(f"C entries: {stats['c_obj_entries']} | ASM entries: {stats['asm_bin_entries']}")


if __name__ == "__main__":
    main()
