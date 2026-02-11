#!/usr/bin/env python3
"""Normalize Ghidra-derived candidate C files for ProDG parsing."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


PRELUDE = """typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
typedef unsigned int uint;
typedef int bool32;
#ifndef NULL
#define NULL ((void*)0)
#endif

struct nuvec_s { f32 x; f32 y; f32 z; };
struct nuvec4_s { f32 x; f32 y; f32 z; f32 w; };
struct nuinstance_s;
struct nugscn_s;
struct nugobj_s;
struct creature_s;
struct creatcmd_s;
struct numtx_s;
struct nuwindgrp_s;

"""


def collect_externs(body: str) -> list[str]:
    dat_names = sorted(set(re.findall(r"\bDAT_[0-9A-Fa-f]{8}\b", body)))
    ptr_names = sorted(set(re.findall(r"\bPTR_DAT_[0-9A-Fa-f]{8}\b", body)))
    out = [f"extern int {n};" for n in dat_names]
    out.extend([f"extern int *{n};" for n in ptr_names])
    return out


def normalize_source(text: str) -> str:
    if text.lstrip().startswith("typedef unsigned char undefined;"):
        return text
    externs = collect_externs(text)
    head = PRELUDE
    if externs:
        head += "\n".join(externs) + "\n\n"
    return head + text


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--candidates-dir",
        default="candidates",
        help="Candidates directory relative to decomp/",
    )
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    cand_dir = (root / args.candidates_dir).resolve()
    files = sorted(cand_dir.glob("**/*.c"))
    changed = 0
    for p in files:
        src = p.read_text(encoding="utf-8", errors="ignore")
        new = normalize_source(src)
        if new != src:
            p.write_text(new, encoding="utf-8")
            changed += 1
    print(f"Normalized: {changed} / {len(files)}")


if __name__ == "__main__":
    main()
