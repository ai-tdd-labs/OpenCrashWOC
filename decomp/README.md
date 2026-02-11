# OpenCrashWOC Decomp Scaffold

This folder adds a MKDD/TWW-style matching scaffold without touching the existing CMake build.

## What this gives you

- `configure.py` that generates `build.ninja` and `objdiff.json`
- `check` target (via `dtk shasum -c`) using `config/<version>/build.sha1`
- `progress` target (reads `report.json`)
- regression targets: `baseline`, `changes`, `regressions`

## Layout

- `config/GC_USA/build.sha1`: expected hashes
- `config/GC_USA/objdiff_units.json`: objdiff units (fill this with your object mapping)
- `tools/changes_fmt.py`: converts `report_changes.json` into markdown

## Setup

From this directory (`decomp/`):

```powershell
python tools/build_demo_units.py
python configure.py --version GC_USA --dtk D:\projects\ps2\crash_bandicoot\tools\dtk\dtk.exe --objdiff D:\projects\ps2\crash_bandicoot\tools\objdiff\objdiff-cli.exe
ninja check
ninja demo-match
ninja candidate-dol
ninja full-dol-diff
ninja mixed-manifest
ninja mixed-dol
ninja mixed-dol-diff
ninja partial-relink-inputs
ninja partial-relink-bundle
ninja progress
ninja baseline
ninja changes
ninja regressions
```

## Next step to make it useful

Populate `config/GC_USA/objdiff_units.json` with real `target_path/base_path` unit entries. Until then, report generation has no function units to compare.

## Persistent workflow memory

Use the ops folder to keep stable context across sessions:

```powershell
python tools/update_ops_state.py --summary "added X" --next "do Y"
```

See:
- `ops/tasks/TODO.md`
- `ops/state/CURRENT_STATE.md`
- `ops/notes/SESSION_LOG.md`
- `ops/roadmap/PHASES.md`

Leaf-first queue helper:

```powershell
python tools/next_leaf_candidate.py
```

Run one candidate compile-and-compare loop:

```powershell
python tools/run_leaf_candidate.py --name FUN_80080c6c --update-queue
```

Note: `run_leaf_candidate.py` now passes `-G 0` by default to improve relinkability.

Auto-build queue from Ghidra exports and run a batch:

```powershell
python tools/build_leaf_queue_from_ghidra.py --limit 12
python tools/normalize_candidate_sources.py
python tools/run_leaf_queue.py --limit 5
```

Import project functions marked with `NGC MATCH` comments into a separate queue:

```powershell
python tools/import_project_match_queue.py --limit 30 --max-size 200
python tools/normalize_candidate_sources.py --candidates-dir candidates_project
python tools/run_leaf_queue.py --queue config/GC_USA/project_match_queue.json --limit 10

# Seed queue with proven demo bitmatches (NuVec* pilot functions)
python tools/build_demo_units.py --auto-slices --match-dol
python tools/import_demo_match_queue.py --out config/GC_USA/demo_match_queue.json
python tools/build_verified_project_queue.py --out-queue config/GC_USA/project_verified_queue.json
```

Build a candidate full DOL from matched queue slices and diff it against original:

```powershell
python tools/build_candidate_dol_from_queue.py --out build/GC_USA/main.candidate.dol
python tools/dol_diff_report.py --target build/GC_USA/main.candidate.dol --out build/GC_USA/dol_diff.txt
```

Build a full mixed manifest (C object slices + ASM bins) and reconstruct full DOL:

```powershell
python tools/build_mixed_manifest.py --out-manifest build/GC_USA/mixed_manifest.json
python tools/build_mixed_dol.py --manifest build/GC_USA/mixed_manifest.json --bundle-link --out build/GC_USA/main.mixed.dol --report-out build/GC_USA/mixed_build_report.json
python tools/dol_diff_report.py --target build/GC_USA/main.mixed.dol --out build/GC_USA/mixed_dol_diff.txt
```

`build_mixed_dol.py` now resolves external call relocations for `c_obj` entries via `ngcld` + absolute symbol addresses from `main.dol_functions.json` before patching bytes. This allows non-leaf C functions to be injected into mixed build without introducing unresolved-branch placeholder bytes.

Generate a full function/module map for project-level planning:

```powershell
python tools/generate_function_module_map.py --out-func-map config/GC_USA/function_map.json --out-module-map config/GC_USA/module_map.json
python tools/report_progress.py --version GC_USA --out build/GC_USA/progress_summary.md
```

`build_mixed_manifest.py` reads C candidates from these queues by default:
- `config/GC_USA/demo_match_queue.json`
- `config/GC_USA/project_verified_queue.json`
- `config/GC_USA/leaf_queue.json`
- `config/GC_USA/project_match_queue.json`

Export partial relink prototype inputs from mixed manifest:

```powershell
python tools/export_partial_relink_inputs.py --manifest build/GC_USA/mixed_manifest.json
```

Run partial relink bundle (prototype, currently skips objects with unresolved SDA bases):

```powershell
python tools/run_partial_relink.py --inputs build/GC_USA/partial_relink_inputs.json --out build/GC_USA/partial_relink_bundle.o
```

## Included demo unit

The scaffold can generate multiple demo units with:

```powershell
python tools/build_demo_units.py
```

Generated units:

- `demo/nuvec_add_demo`
- `demo/nuvec_sub_demo`
- `demo/nuvec_scale_demo`
- `demo/nuvec_dot_demo`
- `demo/nuvec_lensq_demo`

Intentional mismatch mode:

```powershell
python tools/build_demo_units.py --include-mismatch
```

This adds `demo/nuvec_add_mismatch` so `progress` shows a controlled non-100% case.

## First real DOL slice pilot

This repo now includes a real-address pilot slice:

- `config/GC_USA/slices_demo.json` (currently `NuVecAdd` at `0x800C173C`)

Run:

```powershell
python tools/build_demo_units.py
python tools/match_dol_slices.py
```

Expected:

- exit code `0` and `MATCH` for all configured demo slices
- mismatch when you swap one object to a broken variant (e.g. `nuvec_add_broken_demo.o`)

This is the first building block toward full split+relink+hash flow.

### Auto slice generation

You can auto-generate slice addresses from compiled objects:

```powershell
python tools/build_demo_units.py
python tools/auto_slice_from_objects.py --objects-dir build/demo_o --glob "*.o" --out config/GC_USA/slices_demo.json
python tools/match_dol_slices.py
```

One-shot equivalent:

```powershell
python tools/build_demo_units.py --auto-slices --match-dol
```

Or through ninja after `configure.py`:

```powershell
ninja demo-match
```

Notes:
- default mode only keeps objects with a unique hit in DOL text.
- use `--allow-ambiguous` to keep multi-hit objects (uses first hit).
