# Literature Context Packet

**Paper under review**: *The Bernoulli Model: A Probabilistic Framework for Data Structures and Types* (`bernoulli_model`), foundational sub-paper of the `bernoulli_data_type` project.
**Date**: 2026-05-29
**Scope of grounding**: this paper's declared role is the FOUNDATIONAL half of a two-sub-paper project; its sibling `algebraic_types` is the type-theoretic extension. Novelty is assessed RELATIVE TO that declared role and relative to the foundation paper `bernoulli_sets`.

This packet merges the broad field survey and the targeted direct-comparison scouting into a single deduplicated reference frame.

## A. Field positioning (broad survey)

The paper draws on four established literatures at once:

1. **Probabilistic / approximate data structures.** Bloom filters (Bloom 1970, cited), counting Bloom filters, Count-Min sketch (Cormode and Muthukrishnan 2005, *named but uncited*), quotient filters, cuckoo filters, HyperLogLog. The standard framing in this literature is *per-structure* analysis: each structure gets its own false-positive analysis. The paper's distinguishing move is to treat all of these as instances of one model (a "Bernoulli approximation" of an indicator or a map), which is a unifying, typed reframing rather than a new structure.

2. **Information / coding theory.** Binary symmetric and asymmetric channels (Shannon 1948, cited; Cover and Thomas 2006, cited). The paper's "first-order / second-order Bernoulli Boolean" are exactly the BSC and binary asymmetric channel. This is correctly attributed; the contribution is the *typed* repackaging (channels as types `Ber{Bool}^{(k)}`), not the channel theory.

3. **Randomized algorithms with one-sided error.** Miller and Rabin primality (discussed in `bernoulli_maps.tex`), Monte Carlo algorithms generally. The framing "a PRNG seed draws a sample function from `B_{Z->Bool}(isPrime)`" is a genuinely useful conceptual lens and is consistent with the foundation paper's Remark on membership tests as Bernoulli Booleans.

4. **Type theory / functional programming.** The monadic "lifting" language (`lifting.tex`), the data-is-code/code-is-data argument, and the "computational basis of a type" connect to the established idea of a probability/writer monad and to algebraic data types. The paper uses this language informally; the formal type-theoretic development is explicitly deferred to the sibling `algebraic_types` paper.

**Adjacent prior art the paper does not cite but a referee may raise** (framing, not priority threats, given the declared foundational role): randomized response (Warner 1965, present in `references.bib` but *uncited*; the foundation paper `bernoulli_sets` does cite it for exactly the per-element interpretation used here), probabilistic/measure monads in semantics, and differential-privacy randomized-response mechanisms (same Warner lineage).

## B. Direct comparisons (targeted)

The only truly direct comparisons are *internal to the author's own family*, which is expected and legitimate for a foundational companion paper:

| Source | Relationship | What it owns (so this paper should NOT re-claim) |
|---|---|---|
| `bernoulli_sets` (foundation, cited as `bernoulliSets`) | Parent model | The two axioms; model-order = partition-block definition; the parametric-parsimony / Kronecker remark; binomial error-count distributions; the composition theorem via channel-matrix products; the abstract-data-type formulation; the Boolean-universe example; Miller and Rabin as a positive Bernoulli Boolean. |
| `bernoulli_composition` (cited) | Companion | Set-operation error propagation; interval arithmetic over rates. |
| `bernoulli_maps` (cited) | Companion / overlap | Approximate function spaces `X->Y`, the confusion matrix for function spaces, map composition. **This is the largest overlap risk**: `bernoulli_maps.tex` in THIS paper reproduces the function-space confusion matrix and Bernoulli-map machinery that the standalone `bernoulli_maps` paper owns. |

**Key consequence for the novelty assessment.** Because the foundation paper already states (i) the model-order/parameter-count distinction with the worked "12 parameters reduces to 4" example, (ii) the composition theorem, and (iii) Miller and Rabin as a Bernoulli Boolean, this paper's incremental contribution is *narrower than its abstract advertises*. Its defensible novel contributions, relative to declared role, are:

- The **per-type framing of order 0 for void/unit**, and the observation that uncertainty first appears at `Bool` (the "first type for which we can introduce uncertainty"). This is a small but genuine type-theoretic framing not present verbatim in `bernoulli_sets`.
- The **`Bool -> Bool` lifting table and the distinction between a Bernoulli Model OF a function (`B_{Bool->Bool}`) versus lifting a function to operate ON Bernoulli Booleans (`B_Bool -> B_Bool`)**. This monad-versus-channel distinction is the paper's clearest original pedagogical contribution and is *not* developed in the foundation paper.
- The **explicit `and`-gate error-propagation table** and the observation that a two-input gate induces a fourth-order output. (Note: the foundation paper and the cross-paper notes mention this in passing; here it is worked out.)

## C. Verification anchors used downstream

The following claims were independently re-derived (see methodology / logic reports) and serve as ground truth for the specialist findings:

- The Bayes posterior simplification in `prediction.tex`, `(1-eps)/(1 - eps(1 - q/(1-q)))`, **is algebraically correct** and matches the true posterior; all three boundary evaluations (`q=0`, `q->1`, `q=1/2`) hold.
- The `and`-gate `Pr{correct}` table in `logic_gates.tex` **is correct in all four rows**.
- The `X={1,2}` set-indicator confusion matrix in `set_indicator.tex` **sums to 1 per row** and is consistent with a one-sided (Bloom-like) error model.
- The `hash_set` construction in `set_indicator.tex` contains **multiple genuine errors** (k versus k+1; conflation of seed-search success probability with the false-positive rate; `n=log(k/eps)`; and a two-level space claim of `n/b` bits/element that violates the `-log eps` lower bound the same section invokes). These are flagged as logic and methodology findings, not literature-novelty issues.

## D. Net literature verdict

No external priority threat invalidates the paper. The work is a legitimate, foundational, typed reframing of an established model, correctly attributing the classical channel theory to Shannon and to Cover and Thomas, and the parent model to `bernoulli_sets`. The principal literature-adjacent risks are (1) **internal overlap with `bernoulli_maps`** that should be demarcated explicitly, and (2) an **abstract that over-claims** ("introduces the Bernoulli Model", "Oblivious Data Types", Count-Min sketch) relative to what this sub-paper actually derives and relative to the foundation paper that introduces the model.
