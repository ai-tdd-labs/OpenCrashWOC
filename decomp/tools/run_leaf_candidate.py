#!/usr/bin/env python3
"""Compile and compare one leaf candidate against DOL bytes."""

from __future__ import annotations

import argparse
import json
import struct
import subprocess
from datetime import datetime, timezone
from pathlib import Path


def run(cmd: list[str], cwd: Path) -> None:
    subprocess.run(cmd, cwd=cwd, check=True)


def rel(path: Path, start: Path) -> str:
    return str(path.relative_to(start)).replace("\\", "/")


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
    raise ValueError(f"Address range not fully in DOL text: 0x{address:08X}+0x{size:X}")


def elf32_be_sections(obj_data: bytes) -> list[dict]:
    if obj_data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")
    if obj_data[4] != 1 or obj_data[5] != 2:
        raise ValueError("Only 32-bit big-endian ELF is supported")

    e_shoff = struct.unpack_from(">I", obj_data, 0x20)[0]
    e_shentsize = struct.unpack_from(">H", obj_data, 0x2E)[0]
    e_shnum = struct.unpack_from(">H", obj_data, 0x30)[0]
    e_shstrndx = struct.unpack_from(">H", obj_data, 0x32)[0]

    sections = []
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
            raise ValueError(f"Symbol too short for compare: {name}")
        return out

    text = find_section(sections, ".text")
    out = data[text["offset"] : text["offset"] + min(size, text["size"])]
    if len(out) < size:
        raise ValueError(f".text too short for compare: {obj_path}")
    return out


def words_hex(data: bytes) -> list[str]:
    return [f"{struct.unpack_from('>I', data, i)[0]:08x}" for i in range(0, len(data), 4)]


def load_queue(path: Path) -> list[dict]:
    return json.loads(path.read_text(encoding="utf-8"))


def save_queue(path: Path, items: list[dict]) -> None:
    path.write_text(json.dumps(items, indent=2) + "\n", encoding="utf-8")


def pick_item(items: list[dict], name: str | None) -> tuple[int, dict]:
    if name:
        for idx, item in enumerate(items):
            if item.get("name") == name:
                return idx, item
        raise SystemExit(f"Candidate not found in queue: {name}")
    for idx, item in enumerate(items):
        if item.get("status") == "todo":
            return idx, item
    raise SystemExit("No todo leaf candidate found.")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--name", default=None, help="Candidate name from queue (default: first todo)")
    parser.add_argument("--queue", default="config/GC_USA/leaf_queue.json", help="Queue JSON path (relative to decomp/)")
    parser.add_argument(
        "--dol",
        default="../../crash_bandicoot/roms/gc/usa_v1.00/extracted/main.dol",
        help="Reference DOL path (relative to decomp/)",
    )
    parser.add_argument(
        "--prodg-dir",
        default=r"D:/projects/ps2/crash_bandicoot/tools/compilers/ProDG-v3.5",
        help="Directory with cpp.exe, cc1.exe, NgcAs.exe",
    )
    parser.add_argument("--update-queue", action="store_true", help="Update queue entry status with result")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]  # decomp/
    queue_path = (root / args.queue).resolve()
    items = load_queue(queue_path)
    idx, item = pick_item(items, args.name)

    name = item["name"]
    symbol = item.get("symbol", name)
    source = (root / item.get("source", f"candidates/{name}/{name}.c")).resolve()
    address = int(str(item["address"]), 16)
    size = int(item["size"])

    build_i = root / "build" / "leaf_i"
    build_s = root / "build" / "leaf_s"
    build_o = root / "build" / "leaf_o"
    for d in (build_i, build_s, build_o):
        d.mkdir(parents=True, exist_ok=True)

    prodg = Path(args.prodg_dir)
    cpp = prodg / "cpp.exe"
    cc1 = prodg / "cc1.exe"
    asm = prodg / "NgcAs.exe"

    i_out = build_i / f"{name}.i"
    s_out = build_s / f"{name}.s"
    o_out = build_o / f"{name}.o"

    try:
        run([str(cpp), rel(source, root), "-o", rel(i_out, root)], root)
        run([str(cc1), "-O2", "-mps-float", rel(i_out, root), "-o", rel(s_out, root)], root)
        run([str(asm), rel(s_out, root), "-o", rel(o_out, root)], root)
    except subprocess.CalledProcessError as e:
        print(f"{name}: COMPILE_ERROR (exit={e.returncode})")
        if args.update_queue:
            now = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC")
            items[idx]["status"] = "compile_error"
            items[idx]["last_result"] = {
                "timestamp": now,
                "match": False,
                "error": f"compile failed (exit {e.returncode})",
            }
            save_queue(queue_path, items)
            print(f"Updated queue: {queue_path}")
        raise SystemExit(2)

    try:
        dol_data = (root / args.dol).resolve().read_bytes()
        dol_b = read_dol_bytes(dol_data, address, size)
        obj_b = read_obj_symbol_bytes(o_out, symbol, size)
    except Exception as e:
        print(f"{name}: COMPARE_ERROR ({e})")
        if args.update_queue:
            now = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC")
            items[idx]["status"] = "compare_error"
            items[idx]["last_result"] = {
                "timestamp": now,
                "match": False,
                "error": f"compare failed: {e}",
            }
            save_queue(queue_path, items)
            print(f"Updated queue: {queue_path}")
        raise SystemExit(3)

    is_match = dol_b == obj_b

    print(f"{name} @ 0x{address:08X} size={size}: {'MATCH' if is_match else 'MISMATCH'}")
    if not is_match:
        dol_w = words_hex(dol_b)
        obj_w = words_hex(obj_b)
        for i, (a, b) in enumerate(zip(dol_w, obj_w)):
            if a != b:
                print(f"  [{i:02d}] dol={a} obj={b}")

    if args.update_queue:
        now = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC")
        items[idx]["status"] = "matched" if is_match else "mismatch"
        items[idx]["last_result"] = {
            "timestamp": now,
            "match": is_match,
            "build_object": str(o_out.relative_to(root)).replace("\\", "/"),
        }
        save_queue(queue_path, items)
        print(f"Updated queue: {queue_path}")

    raise SystemExit(0 if is_match else 1)


if __name__ == "__main__":
    main()
