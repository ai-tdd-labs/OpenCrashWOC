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
- [x] Import project `NGC MATCH` functions into queue with address mapping.
- [x] Add demo-match queue integration so mixed manifest can auto-use proven bitmatch C objects.
- [x] Reduce `project_match_queue` compile errors (global/struct dependency stubs + warning-tolerant compile handling).
- [ ] Promote `project_match_queue` mismatches to matched.
- [x] Resolve mixed-build C relocation placeholders by link-resolving `c_obj` entries before DOL patching.
- [x] Add full-source verified queue generation (`project_verified_queue`) from NGC MATCH files.

## Next (TP/TWW/MP4 style parity)

- [ ] Add stable split config coverage for larger DOL regions.
- [x] Introduce source/object mapping conventions per module.
- [x] Generate project-level function/module maps (`function_map.json`, `module_map.json`).
- [ ] Generate objdiff units from mapping data (not hand-edited JSON).
- [ ] Add relink stage for partial linked image.
- [x] Generate partial relink input artifacts (C object list + linker script skeleton).
- [x] Execute linker-based partial relink using generated script and validate section bytes.
- [x] Resolve `_SDA_BASE_` / `_SDA2_BASE_` blocker by compiling queue candidates with `-G 0` for relink stage.
- [x] Add full DOL relink + end hash target (bundle-link resolve path in mixed DOL builder).
- [ ] Exit criteria: all known bitmatch C functions are compiled+linked from C artifacts, others from ASM bins, final full DOL reproducible from artifacts.

## Later

- [x] Integrate Ghidra export helper into candidate queue.
- [x] Add CI checks for tooling/progress summary generation.
- [ ] Track match metrics history in CSV/JSON.
