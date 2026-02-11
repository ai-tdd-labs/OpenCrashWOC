#!/usr/bin/env python3
"""Run queue candidates with multiple compile variants and keep first match."""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path


def load(path: Path) -> list[dict]:
    return json.loads(path.read_text(encoding="utf-8"))


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--queue", default="config/GC_USA/project_match_queue.json", help="Queue path relative to decomp/")
    parser.add_argument("--statuses", default="mismatch,todo,compile_error", help="Comma-separated statuses to include")
    parser.add_argument("--limit", type=int, default=30, help="Max candidates")
    args = parser.parse_args()

    decomp_root = Path(__file__).resolve().parents[1]
    queue_path = (decomp_root / args.queue).resolve()
    allowed = {x.strip() for x in args.statuses.split(",") if x.strip()}

    q = load(queue_path)
    names = [x["name"] for x in q if x.get("status") in allowed][: args.limit]
    if not names:
        print("No candidates selected.")
        return

    runner = decomp_root / "tools" / "run_leaf_candidate.py"
    matched = 0
    still_mismatch = 0
    for name in names:
        print(f"=== {name} [g0] ===")
        cmd_g0 = [sys.executable, str(runner), "--queue", args.queue, "--name", name, "--update-queue"]
        p = subprocess.run(cmd_g0, cwd=decomp_root)
        if p.returncode == 0:
            matched += 1
            continue

        print(f"=== {name} [default] ===")
        cmd_def = [sys.executable, str(runner), "--queue", args.queue, "--name", name, "--no-g0", "--update-queue"]
        p2 = subprocess.run(cmd_def, cwd=decomp_root)
        if p2.returncode == 0:
            matched += 1
        else:
            still_mismatch += 1

    q2 = load(queue_path)
    matched_total = sum(1 for x in q2 if x.get("status") == "matched")
    mismatch_total = sum(1 for x in q2 if x.get("status") == "mismatch")
    print("Summary:")
    print(f"- processed: {len(names)}")
    print(f"- newly matched in this run: {matched}")
    print(f"- still mismatch in this run: {still_mismatch}")
    print(f"- queue matched total: {matched_total}")
    print(f"- queue mismatch total: {mismatch_total}")


if __name__ == "__main__":
    main()
