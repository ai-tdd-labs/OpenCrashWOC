# Ghidra Workflow (Public-Safe)

This folder contains a headless Ghidra workflow that contributors can run locally.

Goal:
- apply known function names
- apply signature/local metadata
- export decompiled C/functions/strings for local analysis

Important:
- You must use your own legally dumped `main.dol`.
- Do not commit game binaries or generated raw dumps.

## What is safe to commit

- Scripts and tooling in this folder.
- Hand-written docs and configs.
- Community-maintained metadata maps that do not contain original game binary bytes.

## What is NOT safe to commit

- `main.dol`, ROM/ISO files, extracted game assets.
- Generated raw decompilation dumps (`*_decompiled.c`, `*_strings.txt`) unless your project policy explicitly allows it.
- Binary slices copied from original game code/data.

## Run (PowerShell)

From repo root:

```powershell
pwsh .\analysis\ghidra\tooling\run_gc_full_public.ps1 `
  -GhidraHome "C:\tools\ghidra_12.0.1_PUBLIC" `
  -InputDol "D:\path\to\your\main.dol"
```

Outputs are written to `analysis/ghidra/raw/`.

## Inputs expected by scripts

The Java scripts look for:

- `analysis/ghidra/maps/gc_name_map_full.json` (or `gc_name_map.json`)
- `analysis/ghidra/maps/gc_signatures_map.json`
- `analysis/ghidra/maps/gc_locals_map.json`
- `analysis/ghidra/tooling/nu2_types.h` (optional but recommended)

If maps are missing, export still runs, but you will get less naming/type info.
