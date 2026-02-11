#!/usr/bin/env python3
"""Emit a compact progress summary markdown for decomp pipeline state."""

from __future__ import annotations

import argparse
import json
from collections import Counter
from pathlib import Path


def load_json(path: Path):
    if not path.exists():
        return None
    return json.loads(path.read_text(encoding="utf-8"))


def queue_stats(path: Path) -> tuple[int, dict[str, int]]:
    data = load_json(path) or []
    c = Counter(str(x.get("status", "todo")) for x in data)
    return len(data), dict(c)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--version", default="GC_USA")
    parser.add_argument("--out", default="build/GC_USA/progress_summary.md")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    cfg = root / "config" / args.version
    bld = root / "build" / args.version

    function_map = load_json(cfg / "function_map.json") or []
    module_map = load_json(cfg / "module_map.json") or []
    mixed_report = load_json(bld / "mixed_build_report.json") or {}

    demo_n, demo_s = queue_stats(cfg / "demo_match_queue.json")
    ver_n, ver_s = queue_stats(cfg / "project_verified_queue.json")
    ghidra_n, ghidra_s = queue_stats(cfg / "ghidra_verified_queue.json")
    leaf_n, leaf_s = queue_stats(cfg / "leaf_queue.json")
    proj_n, proj_s = queue_stats(cfg / "project_match_queue.json")

    lines: list[str] = []
    lines.append("# Decomp Progress Summary")
    lines.append("")
    lines.append("## Queues")
    lines.append("")
    lines.append(f"- demo queue: {demo_n} ({demo_s})")
    lines.append(f"- verified queue: {ver_n} ({ver_s})")
    lines.append(f"- ghidra verified queue: {ghidra_n} ({ghidra_s})")
    lines.append(f"- leaf queue: {leaf_n} ({leaf_s})")
    lines.append(f"- project queue: {proj_n} ({proj_s})")
    lines.append("")
    lines.append("## Mixed Build")
    lines.append("")
    if mixed_report:
        lines.append(f"- c_obj entries: {mixed_report.get('c_obj_entries', 0)}")
        lines.append(f"- asm entries: {mixed_report.get('asm_bin_entries', 0)}")
        lines.append(f"- identical_to_ref: {mixed_report.get('identical_to_ref', False)}")
        lines.append(f"- out_sha1: {mixed_report.get('out_sha1', '')}")
    else:
        lines.append("- mixed_build_report.json missing")
    lines.append("")
    lines.append("## Project Map")
    lines.append("")
    lines.append(f"- function_map entries: {len(function_map)}")
    lines.append(f"- module_map entries: {len(module_map)}")
    if function_map:
        matched = sum(1 for x in function_map if x.get("status") == "matched")
        lines.append(f"- matched C functions in map: {matched}")

    out_path = (root / args.out).resolve()
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
