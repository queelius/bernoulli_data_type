---
title: "Random approximate values over algebraic types"
stage: drafting
format: latex
authors:
  - name: "Alexander Towell"
    email: "lex@metafunctor.com"
    orcid: "0000-0001-6443-9897"

thesis:
  claim: ""
  novelty: ""
  refined: null

prior_art:
  last_survey: null
  key_references: []
  gaps: ""

experiments: []

venue:
  target: null
  candidates: []

review_history:
  - date: "2026-05-29"
    type: "multi-agent-review"
    findings_critical: 0
    findings_major: 6
    findings_minor: 9
    findings_suggestions: 5
    recommendation: "major-revision"
    notes: >-
      Theory sound and draft structurally complete (major-revision, not
      not-ready). Load-bearing math (AND/OR rate tables, product correctness,
      invariant-violation bound, sum-type normalization) re-derived and
      verifies exactly. Prioritized path: (1) fix the documented build command,
      it needs TEXINPUTS="./sty:" or it fails fatally; (2) connect the
      bibliography, 4 of 7 entries (Shannon, Cover-Thomas, Bloom, Warner) are
      uncited though their ideas are used; (3) add external positioning vs the
      probability monad and refinement/liquid types; (4) rewrite cpp.tex amap
      bind, the flagship listing contradicts the paper's own lifting equations.
      NOTE: several state-file "known issues" were stale and are corrected
      below.
    report_path: ".papermill/reviews/2026-05-29/review.md"

related_papers:
  - path: "~/github/bernoulli/papers/bernoulli_data_type/bernoulli_model"
    rel: "extends"
    label: "Parallel sub-paper: foundational Bernoulli model framework"
  - path: "~/github/bernoulli/papers/bernoulli_sets"
    rel: "companion"
    label: "Core set model generalized here to sum/product/unit/absurd types"
---

## Notes

Initialized by papermill on 2026-05-29.

### Discovered structure
- One of TWO parallel sub-papers under `bernoulli_data_type/` (the other is
  `bernoulli_model/`). Self-contained project.
- Format: latex, build `TEXINPUTS="./sty:" pdflatex -shell-escape main.tex` (x2)
  (uses `minted`, so `-shell-escape` is required; ALSO needs sty/ on TEXINPUTS or it fails
  fatally. The plain command documented in CLAUDE.md/collection state does NOT work.)
- Notation: modular `sty/` packages (copied from the retired random_approximate_sets/sty/)
- Sections (8, via `\subfile{}`): intro, approx_values, prims, composite_types,
  error_propagation, bernoulli_boolean_algebra, invariants, cpp
  (note: bernoulli_type_2.tex is commented out, subsumed by approx_values.tex)
- Bibliography: `references.bib`, 7 entries
- Built PDF: ~18 pages (verified clean 2026-05-29 with the TEXINPUTS build; the older
  "~9 pages" note was stale)

### Note on stale collection state
The collection-level `papers/.papermill/state.md` lists OLD sections (bernoulli_type_2, prims,
bernoulli_boolean_algebra, cpp) and says "bibliography does not exist". The paper has since
been expanded: 8 sections above with a 7-entry references.bib. This per-paper state reflects
the current source.

### Stale "known issues" CORRECTED by the 2026-05-29 review
The collection state described this as the earliest-stage, roughest paper. The review found
otherwise (verified by building and reading every included section):
- It builds CLEAN to ~18 pages; all cross-references resolve.
- NO TODO / placeholder markers remain in the included sections.
- There is NO large `orphaned/` directory (just one excluded section file).
- Load-bearing math re-derived and verified exact.
Real remaining issues are editorial, not completeness: see review_history notes (build-command
fix, uncited bib entries, missing external positioning, cpp.tex amap bind).

### Thesis (working, from cross-paper topic placement)
Random approximate values over algebraic types: the type-theoretic generalization of the
Bernoulli model to Bool, sum, product, unit, and absurd types; approximate AND/OR/NOT as logic
gates; error propagation through type constructors; and a C++ realization. The thesis YAML
block is left empty for `papermill:thesis`.

### Related Work and Software
Type-theoretic extension of the `bernoulli_model/` sub-paper. Generalizes all prior papers in
the family. Reference implementation: `bernoulli-cpp/` (`bernoulli<T>`, `bool_coder.hpp`).
