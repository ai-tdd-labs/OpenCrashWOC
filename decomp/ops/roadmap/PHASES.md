# Roadmap Phases

## Phase 1: Verified local matching loop

- ProDG compile path works.
- Object bytes can be compared against DOL bytes.
- Mismatch detection is proven.

## Phase 2: Structured per-function matching

- Candidate queue of unmatched leaf functions.
- Repeatable compile and compare loop per candidate.
- Progress tracking per function.

## Phase 3: Project-scale split and mapping

- Expand split/mapping from demo to real modules.
- Generate objdiff unit metadata from mappings.
- Add stable naming and directory conventions.

## Phase 4: Relink and hash parity

- Partial relink validation.
- Full relink DOL build.
- Hash check target against reference DOL.

## Phase 5: Automation and CI

- Continuous progress snapshots.
- Regression detection in CI.
- Contributor docs for fast onboarding.
