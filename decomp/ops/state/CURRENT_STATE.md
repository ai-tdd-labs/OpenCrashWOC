# Current State

- Updated: 2026-02-11 13:26 UTC
- Branch: `main`
- Commit: `281550c`
- Working tree dirty: `yes`
- Objdiff unit count: `5`
- Slice demo count: `5`

## Latest Summary

- Expanded `project_match_queue` to 159 imported NGC-MATCH candidates (small-size first) and batch-ran compile/compare.
- Upgraded candidate compile handling to treat ProDG warning exit-codes as success when output artifacts exist.
- Added link-resolve stage in `build_mixed_dol.py` so `c_obj` bytes are generated from relinked objects (extern addresses resolved), fixing unresolved-branch placeholder mismatches.
- Current mixed build now uses `7` C entries (`5` demo + `2` project matched: `NuRndrFlush`, `NuTexSetTexture`) and `2632` ASM entries, with final full DOL SHA1 identical to reference.
- Current `project_match_queue` status: `2 matched`, `157 mismatch`, `0 compile_error`.

## Next

- Promote project mismatches to matched and increase C coverage beyond current 2 project matches.
- Complete linker-based full relink target and validate full DOL hash from artifacts.
