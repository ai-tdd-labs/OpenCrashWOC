#!/usr/bin/env python3
"""Auto-add extern stubs for undeclared globals in candidate C files."""

from __future__ import annotations

import argparse
import json
import re
import subprocess
from pathlib import Path


UNDECL_RE = re.compile(r"`([A-Za-z_]\w*)' undeclared")
IGNORE = {
    "NULL",
    "s32",
    "u32",
    "u16",
    "f32",
    "uint",
    "bool32",
    "undefined",
    "undefined1",
    "undefined2",
    "undefined4",
    "undefined8",
}


def compile_probe(root: Path, src_rel: str, prodg: Path) -> str:
    i_out = root / "build" / "autostub.i"
    s_out = root / "build" / "autostub.s"
    cpp = prodg / "cpp.exe"
    cc1 = prodg / "cc1.exe"
    cmds = [
        [str(cpp), src_rel.replace("\\", "/"), "-o", str(i_out.relative_to(root)).replace("\\", "/")],
        [str(cc1), "-O2", "-mps-float", "-G", "0", str(i_out.relative_to(root)).replace("\\", "/"), "-o", str(s_out.relative_to(root)).replace("\\", "/")],
    ]
    errs: list[str] = []
    for cmd in cmds:
        p = subprocess.run(cmd, cwd=root, capture_output=True, text=True)
        if p.returncode != 0:
            errs.append((p.stdout or "") + "\n" + (p.stderr or ""))
            break
    return "\n".join(errs)


def insert_externs(src: str, names: list[str]) -> str:
    if not names:
        return src
    add = [f"extern int {n};" for n in names]
    block = "\n".join(add) + "\n"
    # Insert after typedef prelude and existing extern block.
    marker = "struct nuwindgrp_s;\n"
    if marker in src:
        return src.replace(marker, marker + "\n" + block, 1)
    return block + "\n" + src


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--queue", default="config/GC_USA/project_match_queue.json", help="Queue JSON relative to decomp/")
    parser.add_argument("--statuses", default="compile_error,todo", help="Queue statuses to process")
    parser.add_argument("--limit", type=int, default=10, help="Max entries to process")
    parser.add_argument(
        "--prodg-dir",
        default=r"D:/projects/ps2/crash_bandicoot/tools/compilers/ProDG-v3.5",
        help="Path to ProDG directory",
    )
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    queue_path = (root / args.queue).resolve()
    prodg = Path(args.prodg_dir).resolve()
    allowed = {x.strip() for x in args.statuses.split(",") if x.strip()}

    queue = json.loads(queue_path.read_text(encoding="utf-8"))
    touched = 0
    for item in queue:
        if touched >= args.limit:
            break
        if item.get("status") not in allowed:
            continue
        src_rel = item.get("source")
        if not src_rel:
            continue
        src_path = (root / src_rel).resolve()
        if not src_path.exists():
            continue
        err = compile_probe(root, src_rel, prodg)
        if not err:
            continue
        names = sorted({n for n in UNDECL_RE.findall(err) if n not in IGNORE})
        if not names:
            continue
        src = src_path.read_text(encoding="utf-8", errors="ignore")
        missing = [n for n in names if f"extern int {n};" not in src]
        if not missing:
            continue
        src_path.write_text(insert_externs(src, missing), encoding="utf-8")
        touched += 1
        print(f"Stubbed {item['name']}: {len(missing)} symbols")

    print(f"Patched files: {touched}")


if __name__ == "__main__":
    main()
