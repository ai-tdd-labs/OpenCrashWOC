#!/usr/bin/env python3
"""Verify Ghidra-mapped source queue by compiling full sources and byte-matching symbols."""

from __future__ import annotations

import argparse
import json
import struct
import subprocess
import tempfile
from pathlib import Path


def run_maybe_warn(cmd: list[str], cwd: Path, expected_out: Path | None = None) -> bool:
    p = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
    if p.returncode == 0:
        return True
    if expected_out is not None and expected_out.exists() and expected_out.stat().st_size > 0:
        return True
    return False


def u32be(data: bytes, off: int) -> int:
    return struct.unpack_from(">I", data, off)[0]


def read_dol_bytes(dol_data: bytes, address: int, size: int) -> bytes:
    text_off = [u32be(dol_data, 0x00 + i * 4) for i in range(7)]
    text_addr = [u32be(dol_data, 0x48 + i * 4) for i in range(7)]
    text_size = [u32be(dol_data, 0x90 + i * 4) for i in range(7)]
    for off, addr, length in zip(text_off, text_addr, text_size):
        if length and addr <= address and address + size <= addr + length:
            s = off + (address - addr)
            return dol_data[s : s + size]
    raise ValueError(f"Address not fully in DOL text: 0x{address:08X}+0x{size:X}")


def elf32_be_sections(obj_data: bytes) -> list[dict]:
    if obj_data[:4] != b"\x7FELF":
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
        e = strtab.find(b"\x00", n)
        s["name"] = strtab[n:e].decode("ascii", "ignore") if e != -1 else ""
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
    out = []
    for off in range(symtab["offset"], symtab["offset"] + symtab["size"], entsize):
        st_name, st_value, st_size, st_info, st_other, st_shndx = struct.unpack_from(">IIIBBH", obj_data, off)
        name = ""
        if st_name:
            e = strings.find(b"\x00", st_name)
            name = strings[st_name:e].decode("ascii", "ignore") if e != -1 else ""
        out.append({"name": name, "value": st_value, "size": st_size, "info": st_info, "shndx": st_shndx})
    return out


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
        e = strings.find(b"\x00", st_name)
        name = strings[st_name:e].decode("ascii", "ignore") if e != -1 else ""
        if name != symbol:
            continue
        if st_shndx >= len(sections):
            continue
        sec = sections[st_shndx]
        local_val = st_value
        if local_val >= sec["size"] and sec["addr"] <= local_val < sec["addr"] + sec["size"]:
            local_val = local_val - sec["addr"]
        s_off = sec["offset"] + local_val
        out = data[s_off : s_off + min(size, st_size if st_size else size)]
        if len(out) < size:
            return out + (b"\x00" * (size - len(out)))
        return out
    raise ValueError(f"symbol not found: {symbol}")


def link_with_abs_symbols(obj_path: Path, func_addr: int, addr_by_name: dict[str, int], ngcld: Path, root: Path) -> Path | None:
    obj_data = obj_path.read_bytes()
    syms = load_elf_symbols(obj_data)
    undefs = sorted({s["name"] for s in syms if s["shndx"] == 0 and s["name"]})
    if not undefs:
        return None
    tdir = Path(tempfile.mkdtemp(prefix="verify_link_", dir=str((root / "build").resolve())))
    lds = tdir / "link.ld"
    out_obj = tdir / f"{obj_path.stem}.linked.o"
    lines = [
        "SECTIONS {",
        f"  .text 0x{func_addr:08X} : {{ {obj_path.as_posix()}(.text) }}",
        "}",
    ]
    for n in undefs:
        a = addr_by_name.get(n, func_addr)
        lines.append(f"{n} = 0x{a:08X};")
    lds.write_text("\n".join(lines) + "\n", encoding="utf-8")
    cmd = [str(ngcld), str(obj_path), "-T", str(lds), "-o", str(out_obj)]
    p = subprocess.run(cmd, cwd=root, capture_output=True, text=True)
    if p.returncode != 0 or not out_obj.exists():
        return None
    return out_obj


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--queue-file", default="config/GC_USA/ghidra_source_queue.json", help="Input queue relative to decomp/")
    parser.add_argument(
        "--functions-json",
        default=r"D:/projects/ps2/crash_bandicoot/analysis/ghidra/raw/main.dol_functions.json",
        help="Ghidra function metadata JSON",
    )
    parser.add_argument(
        "--dol",
        default="../../crash_bandicoot/roms/gc/usa_v1.00/extracted/main.dol",
        help="Reference DOL path relative to decomp/",
    )
    parser.add_argument(
        "--prodg-dir",
        default=r"D:/projects/ps2/crash_bandicoot/tools/compilers/ProDG-v3.5",
        help="Path to ProDG dir",
    )
    parser.add_argument(
        "--ngcld",
        default=r"D:/projects/ps2/crash_bandicoot/tools/compilers/ProDG-v3.5/ngcld.exe",
        help="Path to ngcld.exe",
    )
    parser.add_argument("--out-queue", default="config/GC_USA/ghidra_verified_queue.json", help="Output queue relative to decomp/")
    parser.add_argument("--limit", type=int, default=0, help="Optional max queue entries to verify")
    parser.add_argument(
        "--variants",
        default="g0,default",
        help="Comma-separated compile variants to try per source file: g0,default",
    )
    args = parser.parse_args()

    decomp_root = Path(__file__).resolve().parents[1]
    repo_root = decomp_root.parent
    queue_path = (decomp_root / args.queue_file).resolve()
    queue = json.loads(queue_path.read_text(encoding="utf-8"))
    if args.limit > 0:
        queue = queue[: args.limit]

    funcs = json.loads(Path(args.functions_json).resolve().read_text(encoding="utf-8"))
    info_by_name = {str(f.get("name", "")): f for f in funcs if f.get("name")}
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

    dol_data = (decomp_root / args.dol).resolve().read_bytes()
    prodg = Path(args.prodg_dir).resolve()
    cpp = prodg / "cpp.exe"
    cc1 = prodg / "cc1.exe"
    asm = prodg / "NgcAs.exe"
    ngcld = Path(args.ngcld).resolve()

    build_i = decomp_root / "build" / "ghidra_i"
    build_s = decomp_root / "build" / "ghidra_s"
    build_o = decomp_root / "build" / "ghidra_o"
    for d in (build_i, build_s, build_o):
        d.mkdir(parents=True, exist_ok=True)

    raw_variants = [x.strip().lower() for x in args.variants.split(",") if x.strip()]
    variants: list[str] = []
    for v in raw_variants:
        if v in ("g0", "default") and v not in variants:
            variants.append(v)
    if not variants:
        variants = ["g0", "default"]

    source_to_items: dict[str, list[dict]] = {}
    for item in queue:
        src = str(item.get("source", "")).strip()
        name = str(item.get("name", "")).strip()
        if not src or not name:
            continue
        source_to_items.setdefault(src, []).append(item)

    built_by_source: dict[str, list[tuple[str, Path]]] = {}
    for src_rel in sorted(source_to_items.keys()):
        src_abs = (decomp_root / src_rel).resolve()
        if not src_abs.exists():
            continue
        rel = src_abs.relative_to(repo_root).as_posix()
        stem = rel.replace("/", "__").replace(".c", "")
        built: list[tuple[str, Path]] = []
        for variant in variants:
            suffix = f"__{variant}"
            i_out = build_i / f"{stem}{suffix}.i"
            s_out = build_s / f"{stem}{suffix}.s"
            o_out = build_o / f"{stem}{suffix}.o"

            ok = run_maybe_warn(
                [str(cpp), rel, "-Icode", "-Idecomp/prodg_shim/include", "-o", str(i_out.relative_to(repo_root)).replace("\\", "/")],
                repo_root,
                i_out,
            )
            if not ok:
                continue

            cc1_cmd = [str(cc1), "-O2", "-mps-float"]
            if variant == "g0":
                cc1_cmd += ["-G", "0"]
            cc1_cmd += [str(i_out.relative_to(repo_root)).replace("\\", "/"), "-o", str(s_out.relative_to(repo_root)).replace("\\", "/")]
            ok = run_maybe_warn(cc1_cmd, repo_root, s_out)
            if not ok:
                continue

            ok = run_maybe_warn(
                [str(asm), str(s_out.relative_to(repo_root)).replace("\\", "/"), "-o", str(o_out.relative_to(repo_root)).replace("\\", "/")],
                repo_root,
                o_out,
            )
            if not ok or not o_out.exists():
                continue
            built.append((variant, o_out))
        if built:
            built_by_source[src_rel] = built

    verified: list[dict] = []
    scanned = 0
    for item in queue:
        name = str(item.get("name", "")).strip()
        source = str(item.get("source", "")).strip()
        if not name or not source:
            continue
        scanned += 1
        built_objs = built_by_source.get(source, [])
        if not built_objs:
            continue

        finfo = info_by_name.get(name)
        if not finfo:
            continue
        try:
            address = int(str(finfo.get("address", "")), 16)
        except ValueError:
            continue
        size = int(finfo.get("size") or 0)
        if size <= 0:
            continue

        matched_variant = None
        matched_obj = None
        for variant, obj_path in built_objs:
            try:
                linked = link_with_abs_symbols(obj_path, address, addr_by_name, ngcld, repo_root)
                obj_for_read = linked if linked is not None else obj_path
                obj_b = read_obj_symbol_bytes(obj_for_read, name, size)
                dol_b = read_dol_bytes(dol_data, address, size)
                if obj_b == dol_b:
                    matched_variant = variant
                    matched_obj = obj_path
                    break
            except Exception:
                continue
        if matched_obj is None:
            continue

        verified.append(
            {
                "name": name,
                "address": f"0x{address:08X}",
                "size": size,
                "kind": "ghidra_verified",
                "status": "matched",
                "symbol": name,
                "source": source,
                "last_result": {
                    "timestamp": "verified-from-ghidra-source-queue",
                    "match": True,
                    "build_object": str(matched_obj.relative_to(decomp_root)).replace("\\", "/"),
                    "build_variant": matched_variant,
                },
            }
        )

    best = {}
    for v in verified:
        best[v["name"]] = v
    out = sorted(best.values(), key=lambda x: int(x["address"], 16))
    out_path = (decomp_root / args.out_queue).resolve()
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(json.dumps(out, indent=2) + "\n", encoding="utf-8")
    print(f"Scanned queue entries: {scanned}")
    print(f"Sources compiled: {len(built_by_source)}")
    print(f"Verified matched functions: {len(out)}")
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
