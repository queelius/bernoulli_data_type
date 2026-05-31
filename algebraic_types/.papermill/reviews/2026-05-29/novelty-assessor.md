# Novelty Assessor Report

**Paper**: Random approximate values over algebraic types
**Date**: 2026-05-29
**Calibration**: Assessed RELATIVE TO the paper's declared role (the type-theoretic extension sub-paper, building on `bernoulli_sets` and `bernoulli_model`). Shared foundations are not held against it.

## Verdict

The contribution is **legitimate and well-scoped** within the Bernoulli family, and at least three of the four claimed contributions are defensibly new. The deficiency is **positioning, not originality**: the paper does not differentiate itself from the two adjacent literatures any venue reviewer will invoke (probability monads, refinement types), and it does not yet articulate the *unifying* significance it gestures at. Confidence: **high** on the within-family assessment, **medium** on the broader-field claim (depends on related-work engagement the paper has not yet done).

## Contribution-by-contribution assessment

The intro lists four contributions (intro.tex). Mapping each to novelty:

1. **"Define `AT(T)` for each type constructor (void, unit, Bool, sum, product, exponential)."**
   - *Within family*: The Bool/exponential cases restate the foundation papers (Bool = second-order Bernoulli Boolean per the remark in approx_values.tex; exponential = Bernoulli map per the remark in composite_types.tex, explicitly attributed to `bernoulliMaps`). Void/unit are trivial. So this contribution is mostly **synthesis/restatement**, correctly attributed.
   - *Verdict*: Low standalone novelty, but it is the necessary scaffolding for 2 to 4 and is honestly framed as such. Keep, but do not oversell.

2. **"Product types preserve Kronecker factorization (`thm:product_parsimony`)."**
   - The Kronecker-factorization-from-independence result is the family's signature ("parametric parsimony", per the collection CLAUDE.md), already present for sets/maps. The new step is stating it at the level of an **arbitrary product type** and instantiating it for bit vectors (`thm:bitvec_kronecker`).
   - *Verdict*: **Incremental** within the family (the mechanism is reused), but the type-level generalization is a fair, citable increment. Medium novelty.

3. **"Sum types break the factorization (`rem:sum_nonfactor`), with the explicit tag/payload model (`thm:sum_error`)."**
   - This is the **most novel** technical content. The family's prior papers are about sets, maps, and relations, none of which surface a tagged-union structure. The tag-error vs. payload-error decomposition and the resulting non-factorization is genuinely new in this body of work and is not a standard textbook result phrased this way.
   - *Verdict*: **Genuinely novel** within the family; defensible against the broader literature. This should be foregrounded as the headline result.

4. **"`Maybe(A) = A + Unit` is the type-theoretic form of Bernoulli set membership."**
   - A clean conceptual bridge: false-positive/false-negative as tag errors on the optional type. This reframes the foundational membership model as a special case of the sum-type construction. Novel as a *framing/unification* contribution.
   - *Verdict*: **Novel framing**, modest but valuable. Strengthens the "unifies the family" thesis.

## Cross-paper differentiation (the part that is done well)

The paper is careful and honest about its relationship to siblings: exponential types are explicitly Bernoulli maps with the composition theorem "applying directly" (remark in composite_types.tex), and Bernoulli sets are named as the `B = BitSet` special case (`rem:convergence`). This is exactly the right posture for a family member and should NOT be flagged as derivative; it is correct attribution.

## The real novelty gap: external positioning

The paper has **no related-work section** and **cites only the three sibling papers** in the body. Against the broader literature (see literature-context.md), two comparison classes are unaddressed and a venue reviewer will raise both.

### MAJOR (novelty/positioning): No differentiation from probabilistic types and the probability monad
- **Location**: approx_values.tex "Monadic structure"; cpp.tex "Approximate value monad". The word "monad" is used as a selling point.
- **Problem**: The probability monad (Giry; Ramsey and Pfeffer, POPL 2002) is the canonical "values-with-uncertainty as a monad" construction. A reader will ask: how is `approx<X>` different from `Dist<X>`? The genuine answer (it carries a *latent true value* and a *confusion matrix relative to that latent value*, and exposes FPR/FNR/loss queries, rather than being a free distribution) is exactly the novelty, and it is currently never stated. Without it, the monadic framing reads as reinvention.
- **Suggestion**: Add 1 to 2 paragraphs (or a short related-work subsection) stating the distinction explicitly and cite at least one probability-monad reference.

### MAJOR (novelty/positioning): No differentiation from refinement / liquid types
- **Location**: invariants.tex (entire section), `def:data_type_invariant` ("a pair `(T, P)` where `P: T -> Bool`").
- **Problem**: "A data type with invariant ... equivalently a subtype: a subset of the carrier set selected by the predicate" is precisely the refinement-type setting (Liquid Types; F*). The paper presents it as if novel terrain. The actual novel content (the *probability that approximation violates the refinement*, `def:violation_prob`, `thm:violation_bound`) is good and new, but is buried because the section does not acknowledge the established type-theoretic framing it is extending.
- **Suggestion**: Open invariants.tex by naming refinement/subset types and citing one reference, then position the violation-probability analysis as the new contribution. This converts a potential "did the author miss 15 years of refinement-types work?" objection into a clear delta.

### MINOR (significance not articulated)
- **Location**: cpp.tex final paragraphs ("permits space-efficient representations and, moreover, concepts like *oblivious algebraic data types*").
- **Problem**: The paper hints at the payoff (space efficiency, oblivious/encrypted computation, the trapdoor-computing connection per the repo's CLAUDE.md) but never develops a motivating application that a non-family reader can latch onto. For an early draft this is acceptable, but it is why the significance currently reads as "internal to the project."
- **Suggestion**: Add one concrete end-to-end example in the intro or a discussion section (e.g., an approximate record / encrypted database row) showing why approximate *types* (not just sets) buy something.

## Is the headline story sound and differentiated from prior work? (the area chair's question)
- **Sound**: yes (the logic-checker independently verified the product/sum factorization claims).
- **Differentiated within the family**: yes, clearly (sum non-factorization and Maybe=membership are new here).
- **Differentiated from prior work on probabilistic/refinement types**: **not yet**. The ideas are distinct, but the paper has not done the positioning to demonstrate it. This is the single most impactful revision for perceived novelty.

## Summary
Novelty is real but under-defended externally. Foreground contribution 3 (sum non-factorization) as the headline, keep 4 (Maybe=membership) as the unifying hook, frame 1 to 2 honestly as scaffolding, and close the two positioning gaps (probability monad, refinement types). No originality failure; no overlap with siblings that should count against it.
