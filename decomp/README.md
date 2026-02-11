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
python configure.py --version GC_USA --dtk D:\projects\ps2\crash_bandicoot\tools\dtk\dtk.exe --objdiff D:\projects\ps2\crash_bandicoot\tools\objdiff\objdiff-cli.exe
ninja check
ninja progress
ninja baseline
ninja changes
ninja regressions
```

## Next step to make it useful

Populate `config/GC_USA/objdiff_units.json` with real `target_path/base_path` unit entries. Until then, report generation has no function units to compare.

## Included demo unit

This scaffold includes one working demo unit:

- `numath/NuVecAdd_demo`

It compares `test_nuvecadd.o` from `D:\projects\ps2\crash_bandicoot\decomp_test`.
To intentionally produce a mismatch report, temporarily change `target_path` in `config/GC_USA/objdiff_units.json` to:

`../../crash_bandicoot/decomp_test/test_broken.o`
