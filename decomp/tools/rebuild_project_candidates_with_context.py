#!/usr/bin/env python3
"""Rebuild project candidate sources with original include context."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


FUNC_HEAD_RE = re.compile(r"(?ms)([A-Za-z_][\w \t\*\(\),\[\]]*?\b{name}\b[ \t]*\([^;{{}}]*\)[ \t]*)\{{")
SRC_NOTE_RE = re.compile(r"Imported from\s+(.+?)\s+NGC MATCH comment\.?", re.IGNORECASE)


def extract_function(text: str, name: str) -> str | None:
    pat = re.compile(FUNC_HEAD_RE.pattern.format(name=re.escape(name)))
    m = pat.search(text)
    if not m:
        return None
    start = m.start(1)
    brace = text.find("{", m.end(1))
    if brace == -1:
        return None
    depth = 0
    i = brace
    n = len(text)
    while i < n:
        c = text[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return text[start : i + 1].strip() + "\n"
        i += 1
    return None


def extract_context_prefix(text: str) -> str:
    lines = text.splitlines()
    out: list[str] = []
    for line in lines:
        s = line.strip()
        if not s:
            out.append("")
            continue
        if s.startswith("//") or s.startswith("/*") or s.startswith("*"):
            out.append(line)
            continue
        if s.startswith("#include") or s.startswith("#define") or s.startswith("#pragma") or s.startswith("#if") or s.startswith("#elif") or s.startswith("#else") or s.startswith("#endif") or s.startswith("#undef"):
            out.append(line)
            continue
        # Stop at first non-preprocessor content.
        break
    return "\n".join(out).strip() + "\n\n" if out else ""


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--queue", default="config/GC_USA/project_match_queue.json", help="Queue path relative to decomp/")
    parser.add_argument("--code-root", default="../code/src", help="Project code/src root relative to decomp/")
    parser.add_argument("--only-status", default="mismatch,compile_error,todo", help="Comma statuses to rebuild")
    parser.add_argument("--limit", type=int, default=0, help="Optional max candidates")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    repo_root = root.parent
    queue_path = (root / args.queue).resolve()
    code_root = (root / args.code_root).resolve()
    allowed = {x.strip() for x in args.only_status.split(",") if x.strip()}

    queue = json.loads(queue_path.read_text(encoding="utf-8"))
    rebuilt = 0
    skipped = 0
    for item in queue:
        if args.limit and rebuilt >= args.limit:
            break
        if item.get("status") not in allowed:
            continue
        name = str(item.get("name", ""))
        source_rel = item.get("source")
        notes = str(item.get("notes", ""))
        if not name or not source_rel:
            skipped += 1
            continue
        m = SRC_NOTE_RE.search(notes)
        if not m:
            skipped += 1
            continue
        rel_src = m.group(1).strip().replace("\\", "/")
        src_path = (code_root / rel_src).resolve()
        if not src_path.exists():
            skipped += 1
            continue

        text = src_path.read_text(encoding="utf-8", errors="ignore")
        func = extract_function(text, name)
        if not func:
            skipped += 1
            continue
        prefix = extract_context_prefix(text)

        out_path = (root / source_rel).resolve()
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(prefix + func, encoding="utf-8")
        rebuilt += 1

    print(f"Rebuilt: {rebuilt}")
    print(f"Skipped: {skipped}")
    print(f"Queue: {queue_path}")


if __name__ == "__main__":
    main()
