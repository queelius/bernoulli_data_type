# Novelty Assessor Report

**Paper**: The Bernoulli Model: A Probabilistic Framework for Data Structures and Types
**Reviewer lens**: contribution clarity, differentiation, significance
**Date**: 2026-05-29
**Calibration note**: this paper is the FOUNDATIONAL half of the `bernoulli_data_type` project and a companion to `bernoulli_sets`. Shared foundations with `bernoulli_sets` are NOT counted against novelty. I assess novelty relative to (a) the declared foundational/reframing role, and (b) what the sibling `bernoulli_maps` and parent `bernoulli_sets` already own.

## Summary

Judged as a foundational reframing paper, the contribution is real but modest, and it is currently obscured by an abstract that over-claims and by heavy overlap with the sibling `bernoulli_maps` content. The genuinely original material, the type-indexed view (`Ber{T}` for any type, order 0 for void/unit, uncertainty first appearing at `Bool`), and especially the *lifting* distinction between approximating a function versus lifting a function onto Bernoulli arguments, is valuable and not stated in the foundation paper. But the paper does not foreground these; it spends as much space re-deriving function-space confusion matrices that `bernoulli_maps` owns. The net effect is a paper whose stated contribution ("introduces the Bernoulli Model") is larger than its actual contribution, while its actual contribution is under-advertised.

## What is genuinely novel here (relative to declared role)

1. **Type-indexed framing and the degenerate-type boundary.** `intro.tex` lines 22--27: `Ber{void} ≡ void`, `Ber{()} ≡ ()`, and "`Bool` is the first type for which we can introduce uncertainty," with the sum/product observations (`Ber{Bool} = Ber{True} ⊕ Ber{False}`, product type Bernoulli only if both factors have >1 value). This per-type ordering of where uncertainty can live is a clean foundational framing that the set-centric `bernoulli_sets` does not state. **Keep and foreground.**

2. **The lifting distinction (the paper's best idea).** `lifting.tex` lines 30--36: the explicit contrast between (i) a Bernoulli Model *of* a function, `B_{Bool->Bool}`, with its 4x4 confusion matrix, and (ii) *lifting* an exact function to act on Bernoulli arguments, `B_Bool -> B_Bool` (the monadic lift). The observation that the constant functions `True`/`False` collapse under lifting while `id`/`not` propagate input noise is genuinely illuminating and is not in `bernoulli_sets`. This is the paper's strongest claim to originality and should be the centerpiece.

3. **Worked logic-gate error propagation as a typed operation.** `logic_gates.tex`: the `and` table and the "two inputs induce a fourth-order output" observation. This is partly anticipated by the family's cross-paper notes, but the explicit per-output-case derivation is a useful concrete artifact.

## Where novelty is overstated or duplicated

### MAJOR

**N1. The abstract claims to "introduce the Bernoulli Model"; the foundation paper introduces it.**
- Location: abstract (`main.tex` lines 39--41), and `intro.tex` line 6 ("The Bernoulli Model is a general framework...").
- Quoted: "This paper introduces the Bernoulli Model, a probabilistic framework designed to handle uncertainty in data types."
- Problem: `bernoulli_sets` is the paper that introduces the model (axioms, distributions, composition theorem, ADT). This paper *reframes* the model over types. Claiming to "introduce" it both overstates this paper's contribution and steps on the foundation paper's claim, which is exactly the kind of intra-family double-counting the collection is trying to avoid. The cross-reference exists (`intro.tex` line 12 cites `bernoulliSets` as "the Bernoulli set framework"), but the abstract does not position this paper as a reframing.
- Suggestion: rewrite the abstract opening to "Building on the Bernoulli set model [bernoulliSets], we recast it as a probabilistic model indexed by data types, ...". State the specific deltas (type indexing, lifting, logic gates) as the contribution.

**N2. `bernoulli_maps.tex` largely duplicates the sibling `bernoulli_maps` paper's territory.**
- Location: `bernoulli_maps.tex`, entire section, especially the function-space confusion matrix (Table `tbl:bernoulli_maps_confusion`), the `n(n-1)` DOF count, the conditional-entropy formula (lines 49--56), and the Miller and Rabin example (lines 75--83).
- Problem: the section opens by citing `bernoulliMaps` as the source of the Bernoulli Map formalism, then re-derives that formalism. The function-space confusion matrix here is the same object as in `lifting.tex` (Table `tbl:confusion_matrix`) and as in the standalone `bernoulli_maps` paper. The Miller and Rabin "PRNG seed draws a sample function" framing is the same observation made in `bernoulli_sets` Remark `rem:membership_boolean`. For a foundational sub-paper, this is redundant: it neither adds to `bernoulli_maps` nor is needed to set up `algebraic_types`.
- Suggestion: cut `bernoulli_maps.tex` down to a short bridging subsection that states the type signature `B_{X->Y}(p) : X -> B_Y` and defers everything else to the `bernoulli_maps` paper with a citation. Keep only the "computational basis" subsection (lines 85--90) and the "non-regular types" subsection (lines 91--97), which are the parts that actually motivate `algebraic_types`.

### MINOR

**N3. "Oblivious Data Types" is advertised as a motivation but never developed.**
- Location: abstract ("introduce the concept of Oblivious Data Types"); `intro.tex` line 6 ("a foundation for creating Oblivious Data Types"); `logic_gates.tex` line 116 ("enables the development of Oblivious Data Types").
- Problem: the term is named three times as a headline motivation and a "concept [we] introduce," but the paper never defines an oblivious data type or shows the connection. (The monorepo context places obliviousness in the trapdoor-computing tree.) Advertising it as an introduced concept and not delivering is a novelty over-claim.
- Suggestion: either define it in one paragraph and cite the trapdoor-computing work, or remove it from the abstract and demote it to a "future work / motivation" mention.

**N4. "Count-Min sketch" is claimed as a derived structure but never analyzed.**
- Location: abstract; `intro.tex` line 8; `logic_gates.tex` line 116.
- Quoted: "such as Bloom filters and Count-Min sketches."
- Problem: Count-Min is named three times as something the framework covers, but the paper only works the set-indicator/Bloom case. Count-Min is a *counting* sketch (frequency estimation with one-sided additive error), which is not obviously an instance of the Boolean-valued model developed here; claiming it without analysis is a stretch.
- Suggestion: drop Count-Min unless a paragraph shows it as a Bernoulli model of a multiset-indicator / frequency map. Otherwise it reads as scope inflation.

## Significance

For its intended audience (readers of the Bernoulli family proceeding from sets to the type-theoretic generalization), the lifting distinction and the type-indexed framing are a worthwhile foundation. Outside the family, the paper does not stand alone as a novel contribution: the channel theory is classical and the set model is the foundation paper's. The right framing is "foundational/expository bridge with one sharp original idea (lifting)," and the paper should be honest about that rather than claiming to introduce the model and an entire class of oblivious data types.

## Confidence

High that the abstract over-claims relative to `bernoulli_sets` (verified by reading the foundation paper's `bernoulli_model.tex`, which contains the axioms, composition theorem, ADT, and Miller and Rabin Bernoulli-Boolean remark). High on the `bernoulli_maps` overlap (read both). Medium on the significance calibration.
