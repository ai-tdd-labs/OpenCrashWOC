# Tooling

## Scripts in this folder

- `run_gc_full_public.ps1`: run full headless Ghidra pipeline on your own `main.dol`
- `ghidra_scripts/ApplyGCNames.java`: apply function names from map
- `ghidra_scripts/ApplyFullDebugInfo.java`: apply signatures + locals comments
- `ghidra_scripts/ExportAll.java`: export summary/decompiled/functions/strings
- `extract_dwarf_*.py`: build metadata maps from `code/src/dump_alphaNGCport_DWARF.txt`
- `match_gc_functions.py`, `build_full_name_map.py`: mapping helpers

## Typical flow

1. Generate/update maps:

```powershell
python .\analysis\ghidra\tooling\match_gc_functions.py
python .\analysis\ghidra\tooling\extract_dwarf_signatures.py
python .\analysis\ghidra\tooling\extract_dwarf_locals.py
```

2. Run headless export:

```powershell
pwsh .\analysis\ghidra\tooling\run_gc_full_public.ps1 `
  -GhidraHome "C:\tools\ghidra_12.0.1_PUBLIC" `
  -InputDol "D:\path\to\your\main.dol"
```

3. Use generated local files in `analysis/ghidra/raw/`.
