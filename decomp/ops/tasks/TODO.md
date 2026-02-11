# TODO

## Now

- [x] Set up local ProDG compile pipeline and byte compare.
- [x] Add object-to-DOL slice matcher pilot.
- [x] Add auto-slice discovery from object bytes.
- [x] Add one-shot demo match flow (`demo-match`).
- [x] Add function queue JSON for batch attempts.
- [x] Add first non-demo unmatched leaf function workflow (candidate + compile loop).
- [x] Auto-build leaf queue from Ghidra exports (`functions.json` + decompiled C).
- [x] Batch-run first N leaf candidates and write result statuses.
- [ ] Improve candidate C shaping for branch-layout sensitive near-matches.
- [x] Resolve `compare_error` cases (symbol-size mismatch) in candidate runner.
- [x] Add candidate full-DOL patch flow from matched queue slices.
- [x] Add full-DOL byte/hash diff report against original.
- [x] Add full mixed-manifest pipeline (C slices + ASM bins for all functions).
- [x] Add mixed full-DOL reconstruction + diff report.

## Next (TP/TWW/MP4 style parity)

- [ ] Add stable split config coverage for larger DOL regions.
- [ ] Introduce source/object mapping conventions per module.
- [ ] Generate objdiff units from mapping data (not hand-edited JSON).
- [ ] Add relink stage for partial linked image.
- [x] Generate partial relink input artifacts (C object list + linker script skeleton).
- [x] Execute linker-based partial relink using generated script and validate section bytes.
- [ ] Resolve `_SDA_BASE_` / `_SDA2_BASE_` to relink all C objects (not only SDA-free subset).
- [ ] Add full DOL relink + end hash target.

## Later

- [x] Integrate Ghidra export helper into candidate queue.
- [ ] Add CI checks for `demo-match` and regression reports.
- [ ] Track match metrics history in CSV/JSON.
