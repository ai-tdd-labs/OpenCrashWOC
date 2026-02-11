#!/usr/bin/env python3
"""Minimal decomp pipeline scaffold for OpenCrashWOC.

Modes:
  configure (default): write build.ninja + objdiff.json
  progress: print progress from a report.json file
"""

from __future__ import annotations

import argparse
import json
import os
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent


def unix(p: Path | str) -> str:
    return str(p).replace("\\", "/")


def load_units(path: Path) -> list[dict]:
    if not path.is_file():
        return []
    with path.open("r", encoding="utf-8") as f:
        data = json.load(f)
    if not isinstance(data, list):
        raise SystemExit(f"Expected list in {path}")
    return data


def load_version_config(version: str) -> dict[str, str]:
    path = Path("config") / version / "config.yml"
    out: dict[str, str] = {}
    if not path.is_file():
        return out
    for raw in path.read_text(encoding="utf-8").splitlines():
        line = raw.strip()
        if not line or line.startswith("#") or ":" not in line:
            continue
        k, v = line.split(":", 1)
        out[k.strip()] = v.strip().strip("'\"")
    return out


def write_objdiff_json(version: str) -> None:
    units_path = Path("config") / version / "objdiff_units.json"
    units = load_units(units_path)

    objdiff = {
        "min_version": "2.0.0-beta.5",
        "custom_make": "ninja",
        "build_target": False,
        "watch_patterns": [
            "*.c",
            "*.h",
            "*.cpp",
            "*.hpp",
            "*.s",
            "*.py",
            "*.yml",
            "*.json",
            "*.txt",
        ],
        "units": units,
        "progress_categories": [
            {"id": "dol", "name": "DOL"},
            {"id": "game", "name": "Game"},
            {"id": "engine", "name": "Engine"},
        ],
    }

    with Path("objdiff.json").open("w", encoding="utf-8") as f:
        json.dump(objdiff, f, indent=2)



def write_ninja(version: str, build_dir: Path, dtk: str, objdiff: str, enable_progress: bool) -> None:
    out_dir = build_dir / version
    out_dir.mkdir(parents=True, exist_ok=True)

    sha_path = Path("config") / version / "build.sha1"
    report = out_dir / "report.json"
    ok = out_dir / "ok"
    baseline = out_dir / "baseline.json"
    changes = out_dir / "report_changes.json"
    regressions = out_dir / "regressions.md"
    demo_match_ok = out_dir / "demo_match.ok"
    candidate_dol = out_dir / "main.candidate.dol"
    candidate_slices = out_dir / "candidate_slices.json"
    dol_diff_report = out_dir / "dol_diff.txt"
    mixed_manifest = out_dir / "mixed_manifest.json"
    mixed_dol = out_dir / "main.mixed.dol"
    mixed_build_report = out_dir / "mixed_build_report.json"
    mixed_dol_diff_report = out_dir / "mixed_dol_diff.txt"
    demo_match_queue = Path("config") / version / "demo_match_queue.json"
    partial_relink_json = out_dir / "partial_relink_inputs.json"
    partial_relink_ld = out_dir / "partial_relink.ld"
    partial_relink_bundle = out_dir / "partial_relink_bundle.o"
    version_cfg = load_version_config(version)
    ref_dol = version_cfg.get("object", "../../crash_bandicoot/roms/gc/usa_v1.00/extracted/main.dol")

    progress_cmd = f"$python configure.py progress --version {version} --build-dir {unix(build_dir)} --report $in"

    lines = [
        "ninja_required_version = 1.3",
        f"python = {unix(Path(sys.executable))}",
        "",
        "rule check",
        f"  command = {dtk} shasum -c $in -o $out",
        "  description = CHECK $in",
        "",
        "rule report",
        f"  command = {objdiff} report generate -o $out",
        "  description = REPORT $out",
        "",
        "rule progress",
        f"  command = {progress_cmd}",
        "  description = PROGRESS",
        "",
        "rule report_changes",
        f"  command = {objdiff} report changes --format json-pretty {unix(baseline)} $in -o $out",
        "  description = CHANGES",
        "",
        "rule changes_md",
        "  command = $python tools/changes_fmt.py $in $out",
        "  description = FORMAT $out",
        "",
        "rule demo_match",
        "  command = $python tools/build_demo_units.py --auto-slices --match-dol --stamp $out",
        "  description = DEMO_MATCH",
        "",
        "rule candidate_dol",
        f"  command = $python tools/build_candidate_dol_from_queue.py --ref-dol {ref_dol} --out $out --slices-out {unix(candidate_slices)}",
        "  description = CANDIDATE_DOL",
        "",
        "rule dol_diff",
        f"  command = $python tools/dol_diff_report.py --base {ref_dol} --target $in --out $out",
        "  description = DOL_DIFF",
        "",
        "rule mixed_manifest",
        f"  command = $python tools/build_mixed_manifest.py --ref-dol {ref_dol} --out-manifest $out --asm-dir {unix(out_dir / 'asm_bins')}",
        "  description = MIXED_MANIFEST",
        "",
        "rule demo_match_queue",
        "  command = $python tools/import_demo_match_queue.py --out $out",
        "  description = DEMO_MATCH_QUEUE",
        "",
        "rule mixed_dol",
        f"  command = $python tools/build_mixed_dol.py --manifest $in --ref-dol {ref_dol} --out {unix(mixed_dol)} --report-out {unix(mixed_build_report)}",
        "  description = MIXED_DOL",
        "",
        "rule partial_relink",
        f"  command = $python tools/export_partial_relink_inputs.py --manifest $in --out-json {unix(partial_relink_json)} --out-ld {unix(partial_relink_ld)}",
        "  description = PARTIAL_RELINK_INPUTS",
        "",
        "rule partial_relink_bundle",
        f"  command = $python tools/run_partial_relink.py --inputs {unix(partial_relink_json)} --out $out",
        "  description = PARTIAL_RELINK_BUNDLE",
        "",
        f"build {unix(ok)}: check {unix(sha_path)}",
        f"build check: phony {unix(ok)}",
        f"build {unix(report)}: report objdiff.json",
        f"build progress: progress {unix(report)} | {unix(ok)}",
        f"build {unix(baseline)}: report objdiff.json",
        f"build {unix(changes)}: report_changes {unix(report)} | {unix(baseline)}",
        f"build {unix(regressions)}: changes_md {unix(changes)}",
        f"build {unix(demo_match_ok)}: demo_match | {unix(ok)}",
        f"build {unix(demo_match_queue)}: demo_match_queue | {unix(demo_match_ok)}",
        f"build {unix(candidate_dol)}: candidate_dol | {unix(ok)}",
        f"build {unix(dol_diff_report)}: dol_diff {unix(candidate_dol)}",
        f"build {unix(mixed_manifest)}: mixed_manifest | {unix(ok)} {unix(demo_match_queue)}",
        f"build {unix(mixed_dol)} {unix(mixed_build_report)}: mixed_dol {unix(mixed_manifest)}",
        f"build {unix(mixed_dol_diff_report)}: dol_diff {unix(mixed_dol)}",
        f"build {unix(partial_relink_json)} {unix(partial_relink_ld)}: partial_relink {unix(mixed_manifest)}",
        f"build {unix(partial_relink_bundle)}: partial_relink_bundle | {unix(partial_relink_json)}",
        f"build baseline: phony {unix(baseline)}",
        f"build changes: phony {unix(changes)}",
        f"build regressions: phony {unix(regressions)}",
        f"build demo-match: phony {unix(demo_match_ok)}",
        f"build candidate-dol: phony {unix(candidate_dol)}",
        f"build full-dol-diff: phony {unix(dol_diff_report)}",
        f"build demo-match-queue: phony {unix(demo_match_queue)}",
        f"build mixed-manifest: phony {unix(mixed_manifest)}",
        f"build mixed-dol: phony {unix(mixed_dol)}",
        f"build mixed-dol-diff: phony {unix(mixed_dol_diff_report)}",
        f"build partial-relink-inputs: phony {unix(partial_relink_json)} {unix(partial_relink_ld)}",
        f"build partial-relink-bundle: phony {unix(partial_relink_bundle)}",
    ]

    lines.append("default progress" if enable_progress else f"default {unix(ok)}")

    with Path("build.ninja").open("w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines) + "\n")



def print_progress(report_path: Path) -> None:
    if not report_path.is_file():
        raise SystemExit(f"Report not found: {report_path}")

    with report_path.open("r", encoding="utf-8") as f:
        data = json.load(f)

    measures = data.get("measures", {})

    def n(key: str) -> int:
        value = measures.get(key, 0)
        if isinstance(value, str) and value.isdigit():
            return int(value)
        if isinstance(value, (int, float)):
            return int(value)
        return 0

    total_code = n("total_code")
    matched_code = n("matched_code")
    total_fn = n("total_functions")
    matched_fn = n("matched_functions")

    code_pct = (matched_code * 100.0 / total_code) if total_code else 0.0
    fn_pct = (matched_fn * 100.0 / total_fn) if total_fn else 0.0

    print("Progress:")
    print(f"  Code: {matched_code} / {total_code} bytes ({code_pct:.2f}%)")
    print(f"  Func: {matched_fn} / {total_fn} ({fn_pct:.2f}%)")



def main() -> None:
    os.chdir(ROOT)

    parser = argparse.ArgumentParser()
    parser.add_argument("mode", nargs="?", default="configure", choices=["configure", "progress"])
    parser.add_argument("--version", default="GC_USA")
    parser.add_argument("--build-dir", default="build")
    parser.add_argument("--dtk", default="dtk")
    parser.add_argument("--objdiff", default="objdiff-cli")
    parser.add_argument("--no-progress", dest="progress", action="store_false")
    parser.add_argument("--report", default=None)
    args = parser.parse_args()

    build_dir = Path(args.build_dir)

    if args.mode == "configure":
        write_objdiff_json(args.version)
        write_ninja(args.version, build_dir, args.dtk, args.objdiff, args.progress)
        print("Wrote build.ninja and objdiff.json")
    else:
        report = Path(args.report) if args.report else (build_dir / args.version / "report.json")
        print_progress(report)


if __name__ == "__main__":
    main()
