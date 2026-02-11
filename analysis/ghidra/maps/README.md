# Maps Folder

Place local metadata JSON files here for Ghidra scripts:

- `gc_name_map_full.json` (or `gc_name_map.json`)
- `gc_signatures_map.json`
- `gc_locals_map.json`
- optional: `gc_params_map.json`

These files are consumed by:
- `analysis/ghidra/tooling/ghidra_scripts/ApplyGCNames.java`
- `analysis/ghidra/tooling/ghidra_scripts/ApplyFullDebugInfo.java`

Keep only metadata that your project policy allows to publish.
