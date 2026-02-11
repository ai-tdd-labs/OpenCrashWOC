#!/usr/bin/env python3
"""Import project functions marked as NGC MATCH into a candidate queue."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


MATCH_COMMENT_RE = re.compile(r"//\s*(?:NGC MATCH|MATCH GCN|MATCH NGC)", re.IGNORECASE)


def find_function_after_comment(text: str, start_pos: int) -> tuple[str, str] | None:
    # Find function signature start and body with simple brace matching.
    i = start_pos
    n = len(text)
    while i < n and text[i].isspace():
        i += 1
    sig_start = i
    brace = text.find("{", sig_start)
    if brace == -1:
        return None
    sig = text[sig_start:brace].strip()
    m = re.search(r"([A-Za-z_]\w*)\s*\([^()]*\)\s*$", sig)
    if not m:
        return None
    name = m.group(1)

    depth = 0
    j = brace
    while j < n:
        c = text[j]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                body = text[sig_start : j + 1]
                return name, body
        j += 1
    return None


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--functions-json",
        default=r"D:/projects/ps2/crash_bandicoot/analysis/ghidra/raw/main.dol_functions.json",
        help="Ghidra function metadata JSON",
    )
    parser.add_argument(
        "--code-root",
        default="../code/src",
        help="Project source root relative to decomp/",
    )
    parser.add_argument(
        "--queue",
        default="config/GC_USA/project_match_queue.json",
        help="Output queue JSON relative to decomp/",
    )
    parser.add_argument(
        "--candidates-dir",
        default="candidates_project",
        help="Output candidate source directory relative to decomp/",
    )
    parser.add_argument("--max-size", type=int, default=256, help="Max function size to import")
    parser.add_argument("--limit", type=int, default=50, help="Max new functions to import")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]  # decomp/
    code_root = (root / args.code_root).resolve()
    queue_path = (root / args.queue).resolve()
    cand_root = (root / args.candidates_dir).resolve()
    funcs_path = Path(args.functions_json).resolve()

    funcs = json.loads(funcs_path.read_text(encoding="utf-8"))
    func_map = {str(f.get("name")): f for f in funcs}

    queue = json.loads(queue_path.read_text(encoding="utf-8")) if queue_path.exists() else []
    existing = {x.get("name") for x in queue}
    added = 0

    for c_path in sorted(code_root.glob("**/*.c")):
        if added >= args.limit:
            break
        text = c_path.read_text(encoding="utf-8", errors="ignore")
        for cm in MATCH_COMMENT_RE.finditer(text):
            if added >= args.limit:
                break
            found = find_function_after_comment(text, cm.end())
            if not found:
                continue
            name, block = found
            if name in existing:
                continue
            f = func_map.get(name)
            if not f:
                continue
            size = int(f.get("size") or 0)
            if size <= 0 or size > args.max_size:
                continue
            address = "0x" + str(f.get("address", "")).upper()

            out_dir = cand_root / name
            out_dir.mkdir(parents=True, exist_ok=True)
            rel_source = f"{args.candidates_dir}/{name}/{name}.c".replace("\\", "/")
            (out_dir / f"{name}.c").write_text(block + "\n", encoding="utf-8")

            queue.append(
                {
                    "name": name,
                    "address": address,
                    "size": size,
                    "kind": "project_match",
                    "status": "todo",
                    "symbol": name,
                    "source": rel_source,
                    "notes": f"Imported from {c_path.relative_to(code_root)} NGC MATCH comment.",
                }
            )
            existing.add(name)
            added += 1

    queue_path.parent.mkdir(parents=True, exist_ok=True)
    queue_path.write_text(json.dumps(queue, indent=2) + "\n", encoding="utf-8")
    print(f"Wrote {queue_path}")
    print(f"Added: {added}")
    print(f"Total: {len(queue)}")


if __name__ == "__main__":
    main()
