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
ninja progress
ninja baseline
ninja changes
ninja regressions
```

## Next step to make it useful

Populate `config/GC_USA/objdiff_units.json` with real `target_path/base_path` unit entries. Until then, report generation has no function units to compare.

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
