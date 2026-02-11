# Session Log

## 2026-02-11

- Bootstrapped decomp toolkit scaffold and demo object diff flow.
- Added real DOL slice matching pilot for 5 NuVec functions.
- Added auto-slice generation and one-shot demo matching workflow.

## 2026-02-11 11:36 UTC

- Added persistent ops structure and session-state automation.
- NEXT: Implement first unmatched leaf-function queue and compile-compare loop.
- NEXT: Start mapping real modules for split/relink pipeline parity.

## 2026-02-11 11:37 UTC

- Added ops memory structure and first leaf-function queue helper.
- NEXT: Implement compile-compare loop for FUN_80080c6c with source scaffold.
- NEXT: Expand queue with more unmatched leaf candidates from callgraph scan.

## 2026-02-11 11:40 UTC

- Implemented first real leaf compile-compare loop for FUN_80080c6c; first attempt is near-match with only branch-offset mismatches (5 words).
- NEXT: Iterate FUN_80080c6c source shape until full 80-byte match.
- NEXT: Add more leaf candidates to queue from callgraph export.

## 2026-02-11 11:47 UTC

- Executed queue automation: imported 12 new leaf candidates from Ghidra, normalized candidate sources, and batch-ran first 5 candidates (4 mismatch, 1 compare_error).
- NEXT: Fix compare_error handling for symbol-size edge cases (FUN_80009ab0).
- NEXT: Iterate FUN_80080c6c source structure to close the final 5 branch-offset mismatches.
- NEXT: Run next batch on remaining todo candidates.

## 2026-02-11 11:53 UTC

- Added full-DOL candidate patch and diff workflow; validated identical SHA1 for matched-only patch set and mismatch report for forced mismatch patch set.
- NEXT: Drive leaf queue statuses toward matched and re-run full-dol-diff after each batch.
- NEXT: Design real relink stage (object map + linker script path) toward full DOL rebuild parity.

## 2026-02-11 11:54 UTC

- Resolved compare_error path by treating short symbol code as mismatch; queue now has 5 mismatch and 8 todo.
- NEXT: Focus on converting mismatches to matched starting with FUN_80080c6c near-match.
- NEXT: Then run candidate-dol/full-dol-diff after each matched increment.

## 2026-02-11 12:00 UTC

- Implemented mixed full-DOL pipeline (manifest + builder + diff). Verified full identity with ASM-only manifest and measurable divergence when forcing 5 C mismatch functions.
- NEXT: Convert mismatch candidates to matched (start with FUN_80080c6c) to increase C coverage in mixed build.
- NEXT: Prototype partial relink stage using mapped C objects and generated linker inputs.

## 2026-02-11 12:04 UTC

- Extended pipeline with partial relink prototype: generated relink inputs and produced ngcld -r bundle (1/5 objects linkable; 4 blocked by _SDA_BASE_/_SDA2_BASE_).
- NEXT: Add SDA base symbol strategy so ngcld can relink SDA-referencing objects.
- NEXT: Continue converting queue mismatches to matched and re-run mixed-dol-diff coverage.

## 2026-02-11 12:05 UTC

- Enabled -G 0 in leaf compile loop; ngcld partial relink now links all 5 current C objects (previously blocked by SDA base errors).
- NEXT: Iterate mismatches to matched to raise C coverage in mixed full-DOL pipeline.
- NEXT: Implement full relink target that produces a linked text image from generated relink inputs.

## 2026-02-11 12:11 UTC

- Verified full mixed build/relink is not complete yet; added project NGC-MATCH import queue, ran first project batch, and integrated both queues into mixed manifest generation. Current mixed-all-candidates build uses 7 C entries and 2632 ASM entries (not identical to reference).
- NEXT: Fix project queue compile errors via targeted extern/struct stubs for low-dependency functions.
- NEXT: Promote mismatches to matched and increase C coverage incrementally.
- NEXT: Complete linker-based full relink target and validate full DOL hash from artifacts.

## 2026-02-11 12:44 UTC

- Added `import_demo_match_queue.py` and wired demo queue into mixed-manifest defaults.
- Re-ran demo auto-slice+match and imported 5 proven C matches into `config/GC_USA/demo_match_queue.json`.
- Rebuilt full mixed DOL with active C entries: 5 C + 2634 ASM, SHA1 identical to original DOL.
- Ran `autostub_candidate_globals.py` + project queue batch: project queue now 25 compile errors, 5 mismatches (still pending).
- NEXT: improve auto-stubbing/type shims for project queue compile errors.
- NEXT: convert project mismatches to matched and increase C coverage in mixed full build.
- NEXT: finish full linker-based relink target (not only patch-based mixed DOL reconstruction).

## 2026-02-11 13:26 UTC

- Expanded `project_match_queue` import to 159 NGC-MATCH candidates and normalized/stubbed sources.
- Updated `run_leaf_candidate.py`:
  - warning-tolerant compile handling (ProDG can return non-zero with usable outputs),
  - optional extern-symbol resolve link step before compare.
- Updated `build_mixed_dol.py` to resolve C object extern relocations via `ngcld` + absolute symbol map before patching DOL bytes.
- Result: mixed full build with `7` C entries (`5` demo + `2` project matches) and `2632` ASM entries now reproduces original DOL SHA1 exactly.
- Project queue now has `2 matched`, `157 mismatch`, `0 compile_error`.
- NEXT: convert more project mismatches to matched and keep full mixed build SHA1-identical after each promotion.
