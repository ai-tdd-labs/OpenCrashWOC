#!/usr/bin/env python3
"""Rank project mismatches to prioritize likely near-matches."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def score(item: dict) -> int:
    lr = item.get("last_result", {})
    fd = int(lr.get("first_diff_word", -1))
    zero = bool(lr.get("obj_all_zero", False))
    mode = str(lr.get("compare_mode", ""))
    s = 0
    # Later first diff is usually closer to match.
    s += max(fd, 0) * 10
    # Prefer linked-text compares over raw-symbol fallback.
    if mode == "linked_text":
        s += 20
    # Penalize all-zero outputs heavily.
    if zero:
        s -= 200
    return s


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--queue", default="config/GC_USA/project_match_queue.json", help="Queue path relative to decomp/")
    parser.add_argument("--limit", type=int, default=25, help="Top N rows to print")
    parser.add_argument("--out", default="", help="Optional output JSON path (relative to decomp/)")
    args = parser.parse_args()

    decomp_root = Path(__file__).resolve().parents[1]
    queue_path = (decomp_root / args.queue).resolve()
    rows = json.loads(queue_path.read_text(encoding="utf-8"))

    mismatches = [x for x in rows if x.get("status") == "mismatch"]
    ranked = sorted(mismatches, key=score, reverse=True)
    top = ranked[: max(args.limit, 0)]

    print(f"Queue: {queue_path}")
    print(f"Mismatches: {len(mismatches)}")
    print(f"Top: {len(top)}")
    for i, x in enumerate(top, 1):
        lr = x.get("last_result", {})
        print(
            f"{i:02d}. {x.get('name')} score={score(x)} "
            f"fd={lr.get('first_diff_word')} mode={lr.get('compare_mode')} zero={lr.get('obj_all_zero')}"
        )

    if args.out:
        out_path = (decomp_root / args.out).resolve()
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(json.dumps(top, indent=2) + "\n", encoding="utf-8")
        print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
