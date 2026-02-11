#!/usr/bin/env python3
"""Promote verified matches into project_match_queue."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def key_of(item: dict) -> tuple[str, str]:
    return (str(item.get("name", "")), str(item.get("address", "")).upper())


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--project-queue", default="config/GC_USA/project_match_queue.json", help="Project queue relative to decomp/")
    parser.add_argument(
        "--verified-queues",
        default="config/GC_USA/project_verified_queue.json,config/GC_USA/ghidra_verified_queue.json,config/GC_USA/othertools_verified_queue.json",
        help="Comma-separated verified queues relative to decomp/",
    )
    parser.add_argument("--append-missing", action="store_true", help="Append verified entries not present in project queue")
    args = parser.parse_args()

    decomp_root = Path(__file__).resolve().parents[1]
    project_path = (decomp_root / args.project_queue).resolve()
    project = json.loads(project_path.read_text(encoding="utf-8"))

    verified_items: list[dict] = []
    for vf in [x.strip() for x in args.verified_queues.split(",") if x.strip()]:
        vp = (decomp_root / vf).resolve()
        if vp.exists():
            verified_items.extend(json.loads(vp.read_text(encoding="utf-8")))

    verified_index = {key_of(v): v for v in verified_items if str(v.get("status", "")) == "matched"}
    project_index = {key_of(p): i for i, p in enumerate(project)}

    promoted = 0
    appended = 0
    for k, v in verified_index.items():
        if k in project_index:
            i = project_index[k]
            project[i]["status"] = "matched"
            project[i]["last_result"] = v.get("last_result", project[i].get("last_result", {}))
            if v.get("source"):
                project[i]["source"] = v["source"]
            promoted += 1
        elif args.append_missing:
            nv = dict(v)
            nv["kind"] = "project_match"
            nv.setdefault("notes", "Promoted from verified queue.")
            project.append(nv)
            appended += 1

    project.sort(key=lambda x: int(str(x.get("address", "0x0")), 16))
    project_path.write_text(json.dumps(project, indent=2) + "\n", encoding="utf-8")
    print(f"Verified matched loaded: {len(verified_index)}")
    print(f"Promoted existing entries: {promoted}")
    print(f"Appended missing entries: {appended}")
    print(f"Wrote {project_path}")


if __name__ == "__main__":
    main()
