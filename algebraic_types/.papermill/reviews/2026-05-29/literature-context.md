# Literature Context Packet (merged scout output)

**Paper**: Random approximate values over algebraic types (Towell, algebraic_types sub-paper)
**Date**: 2026-05-29
**Scouts**: literature-scout-broad (field survey) + literature-scout-targeted (direct comparisons), merged and deduplicated.

This packet is calibrated to the paper's declared role: the **type-theoretic extension** half of the `bernoulli_data_type` two-sub-paper project, building legitimately on the foundation papers `bernoulli_sets` and `bernoulli_data_type/bernoulli_model`. Shared foundations are NOT counted against novelty here.

## 1. Field map: where this paper sits

The paper occupies the intersection of three established areas. Each has a large literature the paper does not currently engage.

### (A) Probabilistic / approximate data structures
- **Bloom filters** (Bloom 1970, cited in bib but never `\cite`d in body). The paper's `Maybe(A) = A + Unit` equals membership claim is the bridge to this literature.
- **Counting / spectral / cuckoo / quotient filters**; Count-Min sketch (Cormode and Muthukrishnan 2005). The sibling `bernoulli_model` abstract explicitly names Count-Min; this sub-paper does not.
- Survey framing: Broder and Mitzenmacher, "Network Applications of Bloom Filters: A Survey" (2004). Useful to position "approximate types" as a generalization of the per-element-error model these structures share.

### (B) Probabilistic and refinement type systems (the most important comparison class for novelty)
- **Probabilistic programming / probabilistic types**: distributions as first-class types (Church, Anglican, Hakaru; Ramsey and Pfeffer, "Stochastic lambda calculus and monads of probability distributions," POPL 2002). The *probability monad* (Giry monad) is the standard categorical object. The paper's "approximate value monad" (`approx<X>`) is closely related and must be differentiated: the paper's monad carries a **latent true value plus a confusion matrix**, not a free distribution. This is the distinguishing feature and is currently under-argued.
- **Refinement / dependent types with invariants**: Liquid Types (Rondon et al., PLDI 2008), F*, refinement subtyping. The paper's invariants section ("data type with invariant" equals a subtype carved by a predicate `P: T -> Bool`) is exactly the refinement-type setup, but here the novelty is *quantifying the probability that approximation violates the refinement*. No refinement-types work is cited; this is the single clearest "missing related work" gap.
- **Gradual / approximate typing**: not the same as "approximate values," but reviewers may conflate them; one sentence of disambiguation would help.

### (C) Information theory of channels (the mathematical backbone)
- **Shannon channel / channel matrix** (Shannon 1948, in bib, uncited in body). The confusion matrix IS a channel matrix; the Kronecker factorization of product channels and the matrix-product composition law are standard channel-coding facts.
- **Cover and Thomas** (in bib, uncited): standard reference for channel capacity, data-processing inequality. The k-fold composition `M_1...M_k` and the homogeneous `M^k` are a Markov chain of channels; the data-processing inequality is the natural information-theoretic companion the paper could cite.
- **Randomized response** (Warner 1965, in bib, uncited): the 2x2 symmetric channel is exactly Warner's mechanism; this is the historical root of "deliberately approximate Boolean with known FPR/FNR" and is a strong, citable precedent for the Bernoulli Boolean.

## 2. Direct-comparison findings (targeted scout)

| Prior thread | Overlap with this paper | Differentiation the paper should make explicit |
|---|---|---|
| Probability monad (Giry / Ramsey and Pfeffer) | `approx<X>` is a monad over values with error | approx carries (latent value, channel), supports FPR/FNR/loss queries; not a free distribution. State this. |
| Liquid/refinement types | "data type with invariant" equals subtype by predicate | Contribution is *violation probability under a channel*, not type checking. State and cite. |
| Shannon channel matrix | confusion matrix, Kronecker product, M^k | The type-constructor-indexed organization (product factors, sum breaks, exponential equals maps) is the novel lens; the channel algebra itself is classical. Cite Shannon/Cover-Thomas to anchor. |
| Algebraic data types (Pierce TAPL) | void/unit/Bool/sum/product/exponential | Standard ADT algebra; novelty is the *approximate* counterpart functor, not the ADT taxonomy. |
| Bloom filter | `Maybe(A)=A+Unit` equals membership | The sum-type derivation of FP/FN is a fresh framing; cite Bloom 1970 at that point. |

## 3. Is the headline claim novel and differentiated?

**Headline**: "error propagation through algebraic type constructors: products preserve Kronecker factorization / parametric parsimony; sums break it."

- **Within the Bernoulli family**: the per-element-independence to Kronecker-factorization result is the family's recurring theme (the CLAUDE.md "parametric parsimony" principle). For *sets* and *maps* it is already established in the sibling papers. The genuinely new content of THIS paper is (i) lifting that result to **arbitrary product types** (bit vectors, records), (ii) the **sum-type non-factorization** result with the explicit tag/payload decomposition, (iii) the **`Maybe(A)=A+Unit` implies Bernoulli membership** identification, and (iv) **invariant-violation probability** for refinement-style subtypes. Items (ii) through (iv) are the strongest novel contributions and are not subsumed by the foundation papers.
- **Against the broader literature**: the *mathematical machinery* (channel matrices, Kronecker products, monads) is classical; the *organizing contribution* is the systematic type-constructor-by-type-constructor error-propagation calculus. That framing appears novel but is currently asserted rather than positioned against probabilistic/refinement types. **The novelty is real but under-defended**: the paper will read as "re-deriving channel algebra" to a PL/IT reviewer unless it (a) cites Shannon/Cover-Thomas/Warner/refinement types and (b) states crisply what is new relative to the probability monad and to refinement types.

## 4. Recommended citations to add (all already needed; 4 are even sitting uncited in the .bib)

1. Bloom 1970, at the `Maybe = membership` / set-membership bridge. (in bib, uncited)
2. Shannon 1948, at first use of "channel matrix" / confusion matrix. (in bib, uncited)
3. Cover and Thomas 2006, at k-fold composition `M_1...M_k` (data-processing inequality). (in bib, uncited)
4. Warner 1965, at the Bernoulli Boolean / 2x2 symmetric channel as historical precedent. (in bib, uncited)
5. A refinement-types reference (Liquid Types, Rondon, Kawaguchi, and Jhala 2008, or F*), in the invariants section. (NOT in bib; should be added)
6. A probability-monad reference (Ramsey and Pfeffer 2002, or Giry 1982), in the cpp / monadic structure section. (NOT in bib; should be added)

## 5. Net assessment for the novelty-assessor

The contribution is legitimate and well-scoped for the family. The risk is **positioning, not originality**: three of the four claimed contributions (sum non-factorization, Maybe=membership, invariant violation) are defensibly new, but the paper does not yet distinguish itself from the two adjacent literatures (probabilistic types, refinement types) that a venue reviewer would immediately invoke. No evidence of overlap that would constitute a novelty *failure*; the gap is engagement, not duplication.
