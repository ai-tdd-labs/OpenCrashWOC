#!/usr/bin/env python3
"""Run a partial relink bundle step using ngcld -r over C object inputs."""

from __future__ import annotations

import argparse
import json
import subprocess
import tempfile
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--inputs", default="build/GC_USA/partial_relink_inputs.json", help="Partial relink inputs JSON")
    parser.add_argument(
        "--ngcld",
        default=r"D:/projects/ps2/crash_bandicoot/tools/compilers/ProDG-v3.5/ngcld.exe",
        help="Path to ngcld.exe",
    )
    parser.add_argument("--out", default="build/GC_USA/partial_relink_bundle.o", help="Output relocatable object")
    parser.add_argument(
        "--strict",
        action="store_true",
        help="Fail if any input object is not linkable (default: skip failures and continue)",
    )
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    inputs_path = (root / args.inputs).resolve()
    out_path = (root / args.out).resolve()
    ngcld = Path(args.ngcld).resolve()

    data = json.loads(inputs_path.read_text(encoding="utf-8"))
    objs = []
    for e in data.get("entries", []):
        p = (root / e["object"]).resolve()
        if p.exists():
            objs.append(str(p))
    if not objs:
        raise SystemExit("No existing C objects to relink.")

    linkable: list[str] = []
    skipped: list[dict] = []
    tmp_dir = Path(tempfile.mkdtemp(prefix="partial_relink_", dir=str((root / "build").resolve())))
    for obj in objs:
        tmp_out = tmp_dir / "probe.o"
        probe = [str(ngcld), "-r", obj, "-o", str(tmp_out)]
        p = subprocess.run(probe, cwd=root, capture_output=True, text=True)
        if p.returncode == 0:
            linkable.append(obj)
        else:
            skipped.append({"object": obj, "error": (p.stderr or p.stdout).strip()})
            if args.strict:
                raise SystemExit(f"Object not linkable: {obj}\n{(p.stderr or p.stdout)}")

    if not linkable:
        raise SystemExit("No linkable objects after probe.")

    out_path.parent.mkdir(parents=True, exist_ok=True)
    cmd = [str(ngcld), "-r", *linkable, "-o", str(out_path)]
    subprocess.run(cmd, check=True, cwd=root)
    print(f"Wrote {out_path}")
    print(f"Input objects: {len(objs)}")
    print(f"Linked objects: {len(linkable)}")
    print(f"Skipped objects: {len(skipped)}")
    if skipped:
        print("Skipped detail:")
        for s in skipped:
            head = s["error"].splitlines()[0] if s["error"] else "unknown error"
            print(f"- {s['object']}: {head}")


if __name__ == "__main__":
    main()
