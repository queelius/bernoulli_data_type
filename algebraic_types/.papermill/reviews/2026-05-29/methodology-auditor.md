# Methodology Auditor Report

**Paper**: Random approximate values over algebraic types
**Date**: 2026-05-29
**Scope**: This is a theory paper with a computational-realization section and worked examples. "Methodology" here means: are the modeling assumptions stated and used consistently, are the worked examples reproducible, and does the C++ artifact support the realization claim? No empirical experiments are claimed (state file: `experiments: []`), so there is no statistics/reproducibility-of-results audit to perform.

## Verdict

The modeling methodology is **sound and consistent** where it is exercised, but two structural assumptions are used without being declared as assumptions, and the C++ realization section does not actually realize the model. Confidence: **high**.

## Modeling-assumption audit

| Assumption | Where used | Stated explicitly? | Note |
|---|---|---|---|
| Per-element / per-component independence (Axiom) | product, bit-vector, AND/OR | Used everywhere; named "Bernoulli independence assumption" | OK, but the axiom itself is only referenced, not stated in this paper. Acceptable for a family member that defers to `bernoulli_sets`/`bernoulli_model`, but a one-line restatement would make the paper self-contained. |
| Uniform spread of error mass over wrong values | `thm:sum_error` (third line), `thm:violation_bound`, rational example | Partially: `thm:sum_error` says "assumes a uniform distribution over the wrong component"; `thm:violation_bound` says "uniformly distributing error mass" | Inconsistent disclosure: the sum theorem and violation bound state it, but the surrounding prose sometimes treats results as general. Make the uniform-symmetric-channel hypothesis a named standing assumption. |
| Error rates below 1/2 | interval FPR bound (error_propagation.tex) | No | See logic-checker MINOR-1. The lower endpoint of the interval relies on it. |
| Tag error and payload error independent | `thm:sum_error` | Implicit | The factorization `(1-delta)(1-eA)` assumes tag and payload errors are independent. Reasonable, but should be stated as a modeling choice (one could imagine tag errors correlated with payload corruption). |

**Finding (MAJOR, methodology): undeclared standing assumptions.** The "uniform symmetric channel" and "rates < 1/2" assumptions are load-bearing for `thm:violation_bound`, the rational example, and the interval bound, but are introduced ad hoc. Collect them into a short "Modeling assumptions" paragraph early in the paper so each later result can cite it. Low effort, high clarity payoff.

## Reproducibility of worked examples

I re-derived every quantitative example; all are reproducible from the stated formulas:
- AND/OR conditional-rate tables: reproduced exactly (see logic-checker).
- `cor:bitvec_correct` product correctness: reproduced.
- Rational-number violation `e/(2K)`, 32-bit `= 1/(2^32-2) = 2.328e-10`: reproduced, matches the paper's "approx 2.3e-10".
The examples are concrete and checkable. This is a strength: the paper does not hand-wave its numbers.

## C++ realization audit (cross-verification of logic-checker MAJOR-1)

The logic-checker routed the `amap` listing to me to confirm whether the artifact supports the realization claim. **I confirm the finding.** Reading cpp.tex's "Approximate value monad" subsection against approx_values.tex's lifting equations:

- The realization claim (contribution 5) requires that applying `f` to an `approx<X>` produce an `approx<Y>` whose rates follow Eqs (lift_cond_0)/(lift_cond_1). Those equations make the *output* rates depend on the *input* rates and on `f`.
- The `amap` body computes `fnr = f(true)`, `fpr = f(false)` and discards `x.fpr`, `x.fnr`. The output therefore cannot depend on the input rates, so it cannot implement the stated semantics for even the simplest case `f = id` (where rates must pass through). Confirmed inconsistent.
- Additional issue beyond the logic-checker's three: the surrounding prose says `approx<X>` "is a monad type similar to the *maybe* monad ... significantly more complicated if *fully* implemented," and the general template is given as `template <typename X> struct approx<X> {}` (an empty primary template, which is also not valid as written: a primary template cannot have an empty specialization-style body with `<X>` in the name). So the realization is at the level of a sketch, not compilable code. That is acceptable for an early draft **only if** the paper labels it as illustrative; as written it is presented as the implementation.
- **Reproducibility angle**: a reader cannot compile or run the listings to confirm the realization claim. A working reference exists in the repo (`bernoulli-cpp/include/bernoulli/bernoulli_bool.hpp`, `bool_coder.hpp`); the paper should either (a) reduce the section to clearly-marked pseudocode and defer "the implementation" to that library, or (b) include a small, correct, compilable `bind` that demonstrably reproduces the identity/negation/constant lifting behavior.

**Methodology verdict on the artifact**: the realization claim (contribution 5) is currently **not supported** by the artifact in the paper. This is the same underlying issue as logic-checker MAJOR-1; counted once in the unified report, cross-verified here.

## Scope/methodology gaps appropriate to flag for an early draft

- **No discussion of when independence fails.** The whole framework rests on component independence. A short "limitations" note (correlated errors break the Kronecker story; that is exactly the sum-type lesson generalized) would pre-empt the obvious reviewer question and tie the product/sum contrast to a methodological principle.
- **No validation plan.** Even a theory paper benefits from one Monte Carlo sanity check of a non-trivial composed rate (e.g., simulate the AND-gate output FPR and compare to the closed form). The paper even mentions Monte Carlo for branching programs (cpp.tex) but provides none. Adding one small simulation table would materially raise confidence and is cheap given the formulas are verified.

## Summary
Modeling is sound; examples reproduce exactly. Two methodology actions: (1) declare the standing assumptions (uniform symmetric channel, rates < 1/2, tag/payload independence) in one place; (2) fix or relabel the C++ realization so contribution 5 is actually supported. An optional Monte Carlo validation would strengthen the paper without much cost.
