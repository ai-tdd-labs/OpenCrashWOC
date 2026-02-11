#!/usr/bin/env python3
"""Build demo objects with ProDG and generate objdiff unit config."""

from __future__ import annotations

import argparse
import json
import subprocess
from pathlib import Path


def run(cmd: list[str], cwd: Path) -> None:
    subprocess.run(cmd, cwd=cwd, check=True)


def rel(path: Path, start: Path) -> str:
    return str(path.relative_to(start)).replace("\\", "/")


def compile_one(cpp: Path, cc1: Path, asm: Path, src: Path, i_out: Path, s_out: Path, o_out: Path, cwd: Path) -> None:
    run([str(cpp), rel(src, cwd), "-o", rel(i_out, cwd)], cwd)
    run([str(cc1), "-O2", "-mps-float", rel(i_out, cwd), "-o", rel(s_out, cwd)], cwd)
    run([str(asm), rel(s_out, cwd), "-o", rel(o_out, cwd)], cwd)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--prodg-dir",
        default=r"D:/projects/ps2/crash_bandicoot/tools/compilers/ProDG-v3.5",
        help="Directory containing cpp.exe, cc1.exe and NgcAs.exe",
    )
    parser.add_argument(
        "--include-mismatch",
        action="store_true",
        help="Add an intentional mismatch unit (broken add vs add)",
    )
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]  # decomp/
    src_dir = root / "demo_src"
    build_i = root / "build" / "demo_i"
    build_s = root / "build" / "demo_s"
    build_o = root / "build" / "demo_o"
    for d in (build_i, build_s, build_o):
        d.mkdir(parents=True, exist_ok=True)

    prodg = Path(args.prodg_dir)
    cpp = prodg / "cpp.exe"
    cc1 = prodg / "cc1.exe"
    asm = prodg / "NgcAs.exe"

    targets = [
        "nuvec_add_demo",
        "nuvec_sub_demo",
        "nuvec_scale_demo",
        "nuvec_dot_demo",
        "nuvec_lensq_demo",
    ]

    for name in targets:
        compile_one(
            cpp,
            cc1,
            asm,
            src_dir / f"{name}.c",
            build_i / f"{name}.i",
            build_s / f"{name}.s",
            build_o / f"{name}.o",
            root,
        )

    if args.include_mismatch:
        compile_one(
            cpp,
            cc1,
            asm,
            src_dir / "nuvec_add_broken_demo.c",
            build_i / "nuvec_add_broken_demo.i",
            build_s / "nuvec_add_broken_demo.s",
            build_o / "nuvec_add_broken_demo.o",
            root,
        )

    units: list[dict] = []
    for name in targets:
        obj = build_o / f"{name}.o"
        units.append(
            {
                "name": f"demo/{name}",
                "target_path": rel(obj, root),
                "base_path": rel(obj, root),
                "metadata": {
                    "complete": True,
                    "source_path": rel(src_dir / f"{name}.c", root),
                    "progress_categories": ["dol", "engine"],
                    "auto_generated": False,
                },
            }
        )

    if args.include_mismatch:
        units.append(
            {
                "name": "demo/nuvec_add_mismatch",
                "target_path": rel(build_o / "nuvec_add_broken_demo.o", root),
                "base_path": rel(build_o / "nuvec_add_demo.o", root),
                "metadata": {
                    "complete": False,
                    "source_path": rel(src_dir / "nuvec_add_broken_demo.c", root),
                    "progress_categories": ["dol", "engine"],
                    "auto_generated": False,
                },
            }
        )

    out_path = root / "config" / "GC_USA" / "objdiff_units.json"
    out_path.write_text(json.dumps(units, indent=2) + "\n", encoding="utf-8")
    print(f"Built {len(targets)} demo objects")
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()

