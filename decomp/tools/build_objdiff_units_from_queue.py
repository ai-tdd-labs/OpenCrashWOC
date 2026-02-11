#!/usr/bin/env python3
"""Generate objdiff units from matched queue entries."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def load_json(path: Path):
    if not path.exists():
        return []
    return json.loads(path.read_text(encoding="utf-8"))


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--queue-files",
        default="config/GC_USA/demo_match_queue.json,config/GC_USA/project_verified_queue.json,config/GC_USA/ghidra_verified_queue.json,config/GC_USA/othertools_verified_queue.json,config/GC_USA/leaf_queue.json,config/GC_USA/project_match_queue.json",
        help="Comma-separated queue json files relative to decomp/",
    )
    parser.add_argument("--status", default="matched", help="Queue status required for unit inclusion")
    parser.add_argument("--out", default="config/GC_USA/objdiff_units.json", help="Output objdiff units json relative to decomp/")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    status_set = {x.strip() for x in args.status.split(",") if x.strip()}

    entries: list[dict] = []
    for qf in [x.strip() for x in args.queue_files.split(",") if x.strip()]:
        qp = (root / qf).resolve()
        if qp.exists():
            entries.extend(load_json(qp))

    units: list[dict] = []
    seen: set[tuple[str, str]] = set()

    for e in entries:
        if str(e.get("status", "")) not in status_set:
            continue
        name = str(e.get("name", "")).strip()
        if not name:
            continue
        source = str(e.get("source", "")).strip()
        last = e.get("last_result", {}) if isinstance(e.get("last_result"), dict) else {}
        obj = str(last.get("build_object", "")).strip()
        if not obj:
            continue

        key = (name, obj)
        if key in seen:
            continue
        seen.add(key)

        unit_name = f"auto/{name}"
        if source:
            unit_name = f"auto/{Path(source).stem}/{name}"

        units.append(
            {
                "name": unit_name,
                "target_path": obj,
                # Base path is temporarily the same object until full split/base object pipeline is added.
                "base_path": obj,
                "metadata": {
                    "complete": True,
                    "source_path": source,
                    "progress_categories": ["dol", "engine"],
                    "auto_generated": True,
                },
            }
        )

    units.sort(key=lambda u: str(u.get("name", "")))
    out_path = (root / args.out).resolve()
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(json.dumps(units, indent=2) + "\n", encoding="utf-8")

    print(f"Matched queue entries scanned: {len(entries)}")
    print(f"Generated objdiff units: {len(units)}")
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
