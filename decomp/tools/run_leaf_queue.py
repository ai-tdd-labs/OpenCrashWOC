#!/usr/bin/env python3
"""Run leaf candidate matching in batch and summarize results."""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--queue", default="config/GC_USA/leaf_queue.json", help="Queue path relative to decomp/")
    parser.add_argument("--limit", type=int, default=5, help="Max candidates to run")
    parser.add_argument(
        "--statuses",
        default="todo,mismatch,compile_error,compare_error",
        help="Comma-separated candidate statuses to include",
    )
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    queue_path = (root / args.queue).resolve()
    items = json.loads(queue_path.read_text(encoding="utf-8"))
    allowed = {x.strip() for x in args.statuses.split(",") if x.strip()}

    candidates = [x for x in items if x.get("status") in allowed][: args.limit]
    if not candidates:
        print("No candidates selected.")
        return

    results: list[tuple[str, int]] = []
    script = root / "tools" / "run_leaf_candidate.py"
    for c in candidates:
        name = c["name"]
        print(f"=== {name} ===")
        cmd = [sys.executable, str(script), "--name", name, "--update-queue"]
        proc = subprocess.run(cmd, cwd=root)
        results.append((name, proc.returncode))

    queue = json.loads(queue_path.read_text(encoding="utf-8"))
    counts: dict[str, int] = {}
    for q in queue:
        s = q.get("status", "unknown")
        counts[s] = counts.get(s, 0) + 1

    print("Summary:")
    for name, rc in results:
        if rc == 0:
            label = "match"
        elif rc == 1:
            label = "mismatch"
        elif rc == 2:
            label = "compile_error"
        else:
            label = "compare_error"
        print(f"- {name}: {label}")
    print("Queue status counts:")
    for k in sorted(counts):
        print(f"- {k}: {counts[k]}")


if __name__ == "__main__":
    main()
