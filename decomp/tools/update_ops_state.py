#!/usr/bin/env python3
"""Update ops state snapshot and append a concise session log entry."""

from __future__ import annotations

import argparse
import json
import subprocess
from datetime import datetime, timezone
from pathlib import Path


def run(cmd: list[str], cwd: Path) -> str:
    return subprocess.check_output(cmd, cwd=cwd, text=True).strip()


def safe_run(cmd: list[str], cwd: Path, default: str = "unknown") -> str:
    try:
        return run(cmd, cwd)
    except Exception:
        return default


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--summary", required=True, help="What changed in this session")
    parser.add_argument("--next", action="append", default=[], help="Next task (can be passed multiple times)")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]  # decomp/
    repo = root.parent
    ops = root / "ops"
    state_path = ops / "state" / "CURRENT_STATE.md"
    log_path = ops / "notes" / "SESSION_LOG.md"
    units_path = root / "config" / "GC_USA" / "objdiff_units.json"
    slices_path = root / "config" / "GC_USA" / "slices_demo.json"

    timestamp = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC")
    branch = safe_run(["git", "rev-parse", "--abbrev-ref", "HEAD"], repo)
    commit = safe_run(["git", "rev-parse", "--short", "HEAD"], repo)
    status = safe_run(["git", "status", "--short"], repo, default="")
    dirty = "yes" if status else "no"

    units_count = 0
    slices_count = 0
    if units_path.is_file():
        units_count = len(json.loads(units_path.read_text(encoding="utf-8")))
    if slices_path.is_file():
        slices_count = len(json.loads(slices_path.read_text(encoding="utf-8")))

    next_items = args.next if args.next else ["Continue with first unmatched leaf-function candidate flow."]

    state_lines = [
        "# Current State",
        "",
        f"- Updated: {timestamp}",
        f"- Branch: `{branch}`",
        f"- Commit: `{commit}`",
        f"- Working tree dirty: `{dirty}`",
        f"- Objdiff unit count: `{units_count}`",
        f"- Slice demo count: `{slices_count}`",
        "",
        "## Latest Summary",
        "",
        f"- {args.summary}",
        "",
        "## Next",
        "",
    ]
    state_lines.extend([f"- {item}" for item in next_items])
    state_path.write_text("\n".join(state_lines) + "\n", encoding="utf-8")

    if not log_path.exists():
        log_path.write_text("# Session Log\n", encoding="utf-8")
    with log_path.open("a", encoding="utf-8") as f:
        f.write(f"\n## {timestamp}\n\n")
        f.write(f"- {args.summary}\n")
        for item in next_items:
            f.write(f"- NEXT: {item}\n")

    print(f"Wrote {state_path}")
    print(f"Appended {log_path}")


if __name__ == "__main__":
    main()
