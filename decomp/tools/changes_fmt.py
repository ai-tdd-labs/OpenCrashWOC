#!/usr/bin/env python3
"""Format objdiff report changes JSON into a short markdown file."""

from __future__ import annotations

import json
import sys
from pathlib import Path


def find_changed_units(data):
    out = []
    if isinstance(data, dict):
        for key in ("units", "changed_units", "changes"):
            value = data.get(key)
            if isinstance(value, list):
                for item in value:
                    if isinstance(item, dict):
                        name = item.get("name") or item.get("unit") or item.get("path")
                        if name:
                            out.append(str(name))
        for value in data.values():
            out.extend(find_changed_units(value))
    elif isinstance(data, list):
        for item in data:
            out.extend(find_changed_units(item))
    return out


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit("Usage: changes_fmt.py <report_changes.json> <out.md>")

    src = Path(sys.argv[1])
    dst = Path(sys.argv[2])

    data = json.loads(src.read_text(encoding="utf-8"))
    names = sorted(set(find_changed_units(data)))

    lines = ["# Matching Regressions", ""]
    if not names:
        lines.append("No changed units detected (or schema not recognized).")
    else:
        lines.append(f"Changed units: {len(names)}")
        lines.append("")
        for n in names:
            lines.append(f"- `{n}`")

    dst.write_text("\n".join(lines) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()