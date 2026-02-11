# TODO

## Now

- [x] Set up local ProDG compile pipeline and byte compare.
- [x] Add object-to-DOL slice matcher pilot.
- [x] Add auto-slice discovery from object bytes.
- [x] Add one-shot demo match flow (`demo-match`).
- [x] Add function queue JSON for batch attempts.
- [ ] Add first non-demo unmatched leaf function workflow (candidate + compile loop).

## Next (TP/TWW/MP4 style parity)

- [ ] Add stable split config coverage for larger DOL regions.
- [ ] Introduce source/object mapping conventions per module.
- [ ] Generate objdiff units from mapping data (not hand-edited JSON).
- [ ] Add relink stage for partial linked image.
- [ ] Add full DOL relink + end hash target.

## Later

- [ ] Integrate Ghidra export helper into candidate queue.
- [ ] Add CI checks for `demo-match` and regression reports.
- [ ] Track match metrics history in CSV/JSON.
