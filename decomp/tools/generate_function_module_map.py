#!/usr/bin/env python3
"""Generate project-level function and module maps for mixed build tracking."""

from __future__ import annotations

import argparse
import json
import re
from collections import defaultdict
from pathlib import Path


DEF_RE = re.compile(
    r"(?ms)^[ \t]*(?:[A-Za-z_][\w \t\*\(\),\[\]]*?[ \t]+)?([A-Za-z_]\w*)[ \t]*\([^;{}]*\)[ \t]*\{"
)


def source_index(code_root: Path) -> dict[str, str]:
    out: dict[str, str] = {}
    for cfile in sorted(code_root.rglob("*.c")):
        text = cfile.read_text(encoding="utf-8", errors="ignore")
        for m in DEF_RE.finditer(text):
            name = m.group(1)
            if name not in out:
                out[name] = str(cfile).replace("\\", "/")
    return out


def load_queue(path: Path) -> list[dict]:
    if not path.exists():
        return []
    return json.loads(path.read_text(encoding="utf-8"))


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--functions-json",
        default=r"D:/projects/ps2/crash_bandicoot/analysis/ghidra/raw/main.dol_functions.json",
        help="Ghidra function metadata JSON",
    )
    parser.add_argument("--code-root", default="../code/src", help="Project source root relative to decomp/")
    parser.add_argument("--demo-queue", default="config/GC_USA/demo_match_queue.json", help="Demo queue JSON")
    parser.add_argument("--verified-queue", default="config/GC_USA/project_verified_queue.json", help="Verified queue JSON")
    parser.add_argument("--leaf-queue", default="config/GC_USA/leaf_queue.json", help="Leaf queue JSON")
    parser.add_argument("--project-queue", default="config/GC_USA/project_match_queue.json", help="Project queue JSON")
    parser.add_argument("--out-func-map", default="config/GC_USA/function_map.json", help="Output function map JSON")
    parser.add_argument("--out-module-map", default="config/GC_USA/module_map.json", help="Output module map JSON")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    funcs = json.loads(Path(args.functions_json).resolve().read_text(encoding="utf-8"))
    src_idx = source_index((root / args.code_root).resolve())

    queues: dict[str, dict] = {}
    for qname, rel in (
        ("demo", args.demo_queue),
        ("verified", args.verified_queue),
        ("leaf", args.leaf_queue),
        ("project", args.project_queue),
    ):
        for entry in load_queue((root / rel).resolve()):
            name = str(entry.get("name", ""))
            if not name:
                continue
            prev = queues.get(name)
            if prev is None:
                queues[name] = {"queue": qname, "entry": entry}
                continue
            prev_status = str(prev["entry"].get("status", ""))
            cur_status = str(entry.get("status", ""))
            # Prefer matched entries over anything else.
            if prev_status != "matched" and cur_status == "matched":
                queues[name] = {"queue": qname, "entry": entry}

    function_map: list[dict] = []
    module_summary: dict[str, dict] = defaultdict(
        lambda: {
            "total_functions": 0,
            "mapped_source": 0,
            "matched_c": 0,
            "queue_hits": 0,
        }
    )

    for f in funcs:
        if f.get("external") or f.get("thunk"):
            continue
        size = int(f.get("size") or 0)
        if size <= 0:
            continue

        name = str(f.get("name", ""))
        addr = str(f.get("address", "")).upper()
        src = src_idx.get(name, "")
        module = "asm/unmapped"
        if src:
            rel = src.replace(str((root / args.code_root).resolve()).replace("\\", "/"), "").lstrip("/")
            module = str(Path(rel).parent).replace("\\", "/") or "."

        qhit = queues.get(name)
        queue_name = qhit["queue"] if qhit else ""
        qentry = qhit["entry"] if qhit else {}
        status = str(qentry.get("status", "")) if qhit else ""
        kind = "c_obj" if status == "matched" else "asm_bin"

        rec = {
            "name": name,
            "address": "0x" + addr,
            "size": size,
            "module": module,
            "source_file": src,
            "queue": queue_name,
            "status": status,
            "kind": kind,
        }
        function_map.append(rec)

        mod = module_summary[module]
        mod["total_functions"] += 1
        if src:
            mod["mapped_source"] += 1
        if qhit:
            mod["queue_hits"] += 1
        if status == "matched":
            mod["matched_c"] += 1

    out_func = (root / args.out_func_map).resolve()
    out_mod = (root / args.out_module_map).resolve()
    out_func.parent.mkdir(parents=True, exist_ok=True)
    out_mod.parent.mkdir(parents=True, exist_ok=True)

    function_map.sort(key=lambda x: int(x["address"], 16))
    module_map = [{"module": m, **v} for m, v in sorted(module_summary.items(), key=lambda kv: kv[0])]

    out_func.write_text(json.dumps(function_map, indent=2) + "\n", encoding="utf-8")
    out_mod.write_text(json.dumps(module_map, indent=2) + "\n", encoding="utf-8")

    matched_total = sum(1 for x in function_map if x["status"] == "matched")
    print(f"Wrote {out_func}")
    print(f"Wrote {out_mod}")
    print(f"Functions: {len(function_map)}")
    print(f"Matched C functions: {matched_total}")


if __name__ == "__main__":
    main()
