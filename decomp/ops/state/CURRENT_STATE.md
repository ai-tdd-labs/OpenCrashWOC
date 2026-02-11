# Current State

- Updated: 2026-02-11 14:02 UTC
- Branch: `main`
- Commit: `e08d051`
- Working tree dirty: `yes`
- Objdiff unit count: `5`
- Slice demo count: `5`

## Latest Summary

- Expanded `project_match_queue` to 159 imported NGC-MATCH candidates (small-size first) and batch-ran compile/compare.
- Upgraded candidate compile handling to treat ProDG warning exit-codes as success when output artifacts exist.
- Added link-resolve stage in `build_mixed_dol.py` so `c_obj` bytes are generated from relinked objects (extern addresses resolved), fixing unresolved-branch placeholder mismatches.
- Added project-level map generation: `config/GC_USA/function_map.json` (2639 functions) and `config/GC_USA/module_map.json`.
- Added stricter bundle-link relink mode in `build_mixed_dol.py` (module/object oriented C relink before patching).
- Added progress summary generator (`tools/report_progress.py`) and CI workflow (`.github/workflows/decomp-progress.yml`).
- Current mixed build now uses `7` C entries (`5` demo + `2` project matched: `NuRndrFlush`, `NuTexSetTexture`) and `2632` ASM entries, with final full DOL SHA1 identical to reference.
- Current `project_match_queue` status: `2 matched`, `157 mismatch`, `0 compile_error`.

## Next

- Promote project mismatches to matched and increase C coverage beyond current 2 project matches.
- Expand module mapping quality (reduce `asm/unmapped`) and use map data to auto-generate objdiff units.
