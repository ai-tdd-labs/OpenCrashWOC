#!/usr/bin/env python3
"""Emit CI progress gate summary with C/ASM counts and verified delta."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def load_json(path: Path):
    if not path.exists():
        return None
    return json.loads(path.read_text(encoding="utf-8"))


def queue_matched(path: Path) -> tuple[int, int]:
    data = load_json(path) or []
    matched = sum(1 for x in data if str(x.get("status", "")) == "matched")
    return len(data), matched


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--version", default="GC_USA")
    parser.add_argument("--out", default="build/GC_USA/ci_progress.md")
    parser.add_argument("--history-jsonl", default="build/GC_USA/metrics_history.jsonl")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    cfg = root / "config" / args.version
    bld = root / "build" / args.version

    mixed = load_json(bld / "mixed_build_report.json") or {}
    ghidra_total, ghidra_matched = queue_matched(cfg / "ghidra_verified_queue.json")
    other_total, other_matched = queue_matched(cfg / "othertools_verified_queue.json")
    project_total, project_matched = queue_matched(cfg / "project_match_queue.json")

    prev = None
    hist_path = (root / args.history_jsonl).resolve()
    if hist_path.exists():
        lines = [x for x in hist_path.read_text(encoding="utf-8").splitlines() if x.strip()]
        if lines:
            try:
                prev = json.loads(lines[-1])
            except json.JSONDecodeError:
                prev = None

    delta_ghidra = None
    if prev is not None and "ghidra_verified_matched" in prev:
        delta_ghidra = ghidra_matched - int(prev.get("ghidra_verified_matched", 0))
    delta_other = None
    if prev is not None and "othertools_verified_matched" in prev:
        delta_other = other_matched - int(prev.get("othertools_verified_matched", 0))

    lines: list[str] = []
    lines.append("# CI Progress Gate")
    lines.append("")
    lines.append("## Mixed Build")
    lines.append("")
    lines.append(f"- c_obj entries: {int(mixed.get('c_obj_entries', 0))}")
    lines.append(f"- asm_bin entries: {int(mixed.get('asm_bin_entries', 0))}")
    lines.append(f"- identical_to_ref: {bool(mixed.get('identical_to_ref', False))}")
    lines.append("")
    lines.append("## Queue Match State")
    lines.append("")
    lines.append(f"- ghidra verified matched: {ghidra_matched} / {ghidra_total}")
    lines.append(f"- othertools verified matched: {other_matched} / {other_total}")
    lines.append(f"- project queue matched: {project_matched} / {project_total}")
    if delta_ghidra is None:
        lines.append("- ghidra verified delta vs previous run: n/a")
    else:
        lines.append(f"- ghidra verified delta vs previous run: {delta_ghidra:+d}")
    if delta_other is None:
        lines.append("- othertools verified delta vs previous run: n/a")
    else:
        lines.append(f"- othertools verified delta vs previous run: {delta_other:+d}")

    out_path = (root / args.out).resolve()
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
