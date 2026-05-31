---
title: "The Bernoulli Model: A Probabilistic Framework for Data Structures and Types"
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
    findings_critical: 2
    findings_major: 7
    findings_minor: 13
    findings_suggestions: 6
    recommendation: "major-revision"
    notes: >-
      Builds clean (21 pp, no undefined refs); verifiable derivations (Bayes
      posterior, and-gate correctness, confusion-matrix rows) re-derived and
      hold. Two criticals: set_indicator.tex conflates the per-seed
      search-success probability and the per-element false-positive rate under
      one symbol epsilon; and the two-level space claim drops a factor of b, so
      it violates the family's own -log eps lower bound (it is a time
      optimization, not a space one). Abstract over-claims that this paper
      "introduces the Bernoulli Model" (bernoulli_sets does). Pervasive
      notation inconsistency (Ber vs Berr vs literal B) plus draft artifacts
      (stray Markdown blockquote, stub opener); bernoulli_maps.tex duplicates
      the sibling maps paper.
    report_path: ".papermill/reviews/2026-05-29/review.md"

related_papers:
  - path: "~/github/bernoulli/papers/bernoulli_data_type/algebraic_types"
    rel: "extended-by"
    label: "Parallel sub-paper: type-theoretic extension over algebraic types"
  - path: "~/github/bernoulli/papers/bernoulli_sets"
    rel: "companion"
    label: "Core set model; this paper reframes it as a probabilistic model of types"
---

## Notes

Initialized by papermill on 2026-05-29.

### Discovered structure
- One of TWO parallel sub-papers under `bernoulli_data_type/` (the other is
  `algebraic_types/`). No top-level main.tex exists; this is a self-contained project.
- Format: latex, build `pdflatex main.tex && pdflatex main.tex` (subfiles; bibtex if cited)
- Notation: inline macros (`\Ber`, `\Bool`, `\True`, `\False`)
- Sections (7, via `\subfile{}`): intro, order_and_channels, prediction, lifting,
  set_indicator, logic_gates, bernoulli_maps
- Bibliography: `references.bib`, 7 entries
- Built PDF: ~19 pages (`bernoulli_model.pdf`)

### KNOWN ISSUES (pre-existing, from collection state)
- Author was a placeholder in source; metadata set here to Alexander Towell.
- Trailing fragment in lifting.tex; minimal conclusion in logic_gates.tex.
- Notation inconsistency across sections. Reviewer/prose-auditor will likely flag these.

### Thesis (working, from cross-paper topic placement)
The Bernoulli model as a probabilistic framework for data structures and types: model order
and channels, prediction, the pointwise lift, set indicators, logic gates, and Bernoulli maps
as a computational basis. Foundational framing for the algebraic-types sub-paper. The thesis
YAML block is left empty for `papermill:thesis`.

### Related Work and Software
Foundational half of the `bernoulli_data_type` pair. Generalizes `bernoulli_sets/` to a
type-theoretic setting. Reference implementation: the `bernoulli-cpp/` header library overall
(`bernoulli<T>` ADT).
