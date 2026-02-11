#!/usr/bin/env python3
"""Run one full verified sweep and append progress metrics history."""

from __future__ import annotations

import argparse
import json
import subprocess
from datetime import datetime, timezone
from pathlib import Path


def run_cmd(cmd: list[str], cwd: Path) -> None:
    p = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
    if p.returncode != 0:
        raise RuntimeError(f"Command failed: {' '.join(cmd)}\nSTDOUT:\n{p.stdout}\nSTDERR:\n{p.stderr}")
    if p.stdout.strip():
        print(p.stdout.strip())


def load_json(path: Path):
    if not path.exists():
        return None
    return json.loads(path.read_text(encoding="utf-8"))


def matched_count(path: Path) -> tuple[int, int]:
    data = load_json(path) or []
    matched = sum(1 for x in data if str(x.get("status", "")) == "matched")
    return len(data), matched


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--version", default="GC_USA")
    parser.add_argument("--history-jsonl", default="build/GC_USA/metrics_history.jsonl")
    parser.add_argument("--history-csv", default="build/GC_USA/metrics_history.csv")
    parser.add_argument("--skip-source-rebuild", action="store_true")
    args = parser.parse_args()

    decomp_root = Path(__file__).resolve().parents[1]
    cfg = decomp_root / "config" / args.version
    bld = decomp_root / "build" / args.version

    if not args.skip_source_rebuild:
        run_cmd(["python", str((decomp_root / "tools" / "build_ghidra_source_queue.py").resolve())], decomp_root)
    run_cmd(["python", str((decomp_root / "tools" / "build_verified_ghidra_queue.py").resolve())], decomp_root)
    run_cmd(
        ["python", str((decomp_root / "tools" / "promote_verified_into_project_queue.py").resolve()), "--append-missing"],
        decomp_root,
    )
    run_cmd(["python", str((decomp_root / "tools" / "build_mixed_manifest.py").resolve())], decomp_root)
    run_cmd(["python", str((decomp_root / "tools" / "build_mixed_dol.py").resolve()), "--bundle-link"], decomp_root)
    run_cmd(
        [
            "python",
            str((decomp_root / "tools" / "dol_diff_report.py").resolve()),
            "--target",
            f"build/{args.version}/main.mixed.dol",
            "--out",
            f"build/{args.version}/mixed_diff_report.txt",
        ],
        decomp_root,
    )
    run_cmd(["python", str((decomp_root / "tools" / "report_progress.py").resolve()), "--version", args.version], decomp_root)

    ghidra_total, ghidra_matched = matched_count(cfg / "ghidra_verified_queue.json")
    project_total, project_matched = matched_count(cfg / "project_match_queue.json")
    mixed = load_json(bld / "mixed_build_report.json") or {}

    row = {
        "timestamp_utc": datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S UTC"),
        "ghidra_verified_total": ghidra_total,
        "ghidra_verified_matched": ghidra_matched,
        "project_queue_total": project_total,
        "project_queue_matched": project_matched,
        "c_obj_entries": int(mixed.get("c_obj_entries", 0)),
        "asm_bin_entries": int(mixed.get("asm_bin_entries", 0)),
        "identical_to_ref": bool(mixed.get("identical_to_ref", False)),
        "out_sha1": str(mixed.get("out_sha1", "")),
    }

    hist_jsonl = (decomp_root / args.history_jsonl).resolve()
    hist_jsonl.parent.mkdir(parents=True, exist_ok=True)
    with hist_jsonl.open("a", encoding="utf-8") as f:
        f.write(json.dumps(row, ensure_ascii=True) + "\n")

    hist_csv = (decomp_root / args.history_csv).resolve()
    header = (
        "timestamp_utc,ghidra_verified_total,ghidra_verified_matched,project_queue_total,"
        "project_queue_matched,c_obj_entries,asm_bin_entries,identical_to_ref,out_sha1\n"
    )
    line = (
        f"{row['timestamp_utc']},{row['ghidra_verified_total']},{row['ghidra_verified_matched']},"
        f"{row['project_queue_total']},{row['project_queue_matched']},{row['c_obj_entries']},"
        f"{row['asm_bin_entries']},{str(row['identical_to_ref']).lower()},{row['out_sha1']}\n"
    )
    if not hist_csv.exists():
        hist_csv.write_text(header, encoding="utf-8")
    with hist_csv.open("a", encoding="utf-8") as f:
        f.write(line)

    print(json.dumps(row, indent=2))
    print(f"Wrote {hist_jsonl}")
    print(f"Wrote {hist_csv}")


if __name__ == "__main__":
    main()
