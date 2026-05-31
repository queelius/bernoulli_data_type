# Citation Verifier Report

**Paper**: Random approximate values over algebraic types
**Date**: 2026-05-29
**Scope**: Citation accuracy, missing references, bibliography integrity. references.bib has 7 entries.

## Verdict

No broken citations and no malformed bib entries, but the bibliography is **incoherent with the body**: 4 of 7 entries are never cited, and the body's conceptual claims (Shannon channel, Bloom filter, randomized response) point exactly at those uncited entries without ever citing them. The paper has **zero external citations in the body** (only the three sibling papers are cited). Confidence: **high** (mechanically checked against the build).

## Bibliography integrity

All 7 entries are well-formed and parse (bibtex ran clean, no warnings). Entry-level check:

| Key | Type | Cited in body? | Accuracy |
|---|---|---|---|
| `bernoulliSets` | unpublished | YES (intro, composite, approx_values) | Self/sibling; OK as `@unpublished`. |
| `bernoulliMaps` | unpublished | YES (intro, prims, composite x2) | Sibling. Title in bib is "The algebra of the random approximate map model"; matches the family's `bernoulli_maps` paper. OK. |
| `bernoulliComposition` | unpublished | YES (bernoulli_boolean_algebra) | Sibling. OK. |
| `coverThomas` | book | **NO** | Accurate (Cover & Thomas, Elements of Information Theory, 2nd ed., Wiley 2006). Uncited. |
| `shannonBSC` | article | **NO** | Accurate (Shannon 1948, BSTJ 27(3):379-423). Uncited. |
| `bloom1970` | article | **NO** | Accurate (Bloom 1970, CACM 13(7):422-426). Uncited. |
| `warner1965` | article | **NO** | Accurate (Warner 1965, JASA 60(309):63-69). Uncited. |

No fabricated or inaccurate references. The four standard references have correct metadata.

## Findings

### MAJOR (citations): four bibliography entries are uncited, and they are exactly the references the body needs
- **Location**: references.bib vs. body.
- **Problem**: `plainnat` with `\bibliography` only prints cited entries, so as built, the 4 uncited entries **do not appear** in the reference list at all. Meanwhile the body makes claims that demand precisely these citations:
  - Channel matrix / confusion matrix language pervades approx_values.tex and composite_types.tex. **First use should cite `shannonBSC`** (and optionally `coverThomas`).
  - The k-fold composition `M_1 ... M_k` and homogeneous `M^k` (error_propagation.tex) is a cascade of channels; **`coverThomas`** (data-processing inequality) is the natural anchor.
  - `Maybe(A) = A + Unit` is "precisely the Bernoulli set membership model" (composite_types.tex `ex:optional`); the membership/filter lineage should **cite `bloom1970`**.
  - The 2x2 symmetric channel / Bernoulli Boolean is exactly Warner's randomized-response mechanism; **cite `warner1965`** at the Bernoulli-Boolean introduction (approx_values.tex `sec:approx_bitset`).
- **Why it matters**: The paper currently reads as if it invented the channel-matrix formalism. Wiring up the four references it already has (zero new literature search required) fixes both the empty-looking bibliography and the "no prior art acknowledged" impression.
- **Suggestion**: Add `\cite{shannonBSC}` / `\cite{coverThomas}` at first "channel matrix" use and at the composition law; `\cite{bloom1970}` at the optional=membership remark; `\cite{warner1965}` at the Bernoulli Boolean. All four entries are accurate; only the `\cite` calls are missing.

### MAJOR (missing references): no probabilistic-monad or refinement-type citations exist at all
- **Location**: approx_values.tex / cpp.tex (monad) and invariants.tex (subtype/invariant).
- **Problem**: The paper claims a monad and builds a refinement-style "data type with invariant" without citing the canonical literatures (see novelty-assessor and literature-context). These references are **not in the .bib** and must be added.
- **Suggestion**: Add at minimum (a) a probability-monad reference (Ramsey and Pfeffer, POPL 2002, or Giry 1982) and (b) a refinement-types reference (Rondon, Kawaguchi, Jhala, "Liquid Types," PLDI 2008, or an F* reference). This overlaps with novelty-assessor's two MAJOR positioning findings; from the citation side it is specifically "these bib entries do not exist yet."

### MINOR (citation form): sibling papers as `@unpublished` with `note = {Companion paper}`
- **Location**: references.bib, three sibling entries.
- **Problem**: `@unpublished` with no `url`/`howpublished` is acceptable, but if these siblings have arXiv IDs or repo URLs they should be added so the citations resolve for an external reader. The repeated `note = {Companion paper}` is fine but uninformative.
- **Suggestion**: When the siblings reach a citable state, upgrade with `eprint`/`url`. Low priority for an early draft.

## Citation accuracy
No inaccurate citations found. No DOIs to verify (none provided). No author-name or year errors in the four standard references (spot-checked against known metadata: Bloom CACM 1970, Shannon BSTJ 1948, Warner JASA 1965, Cover-Thomas 2nd ed. 2006). All correct.

## Summary
Bibliography is clean but disconnected from the text. The fastest, highest-value citation fix requires no new search: cite the four standard references the paper already lists, at the points where the body invokes their ideas. Separately, two genuinely missing references (probability monad, refinement types) must be added to support the monad and invariant claims.
