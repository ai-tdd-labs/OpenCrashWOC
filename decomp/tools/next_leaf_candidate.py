#!/usr/bin/env python3
"""Print the next leaf-function candidate from the queue."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--queue",
        default="config/GC_USA/leaf_queue.json",
        help="Queue file path relative to decomp/",
    )
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    queue_path = (root / args.queue).resolve()
    items = json.loads(queue_path.read_text(encoding="utf-8"))

    for wanted in ("todo", "mismatch"):
        for item in items:
            if item.get("status") == wanted:
                print(json.dumps(item, indent=2))
                return

    print("No pending leaf candidates found.")


if __name__ == "__main__":
    main()
