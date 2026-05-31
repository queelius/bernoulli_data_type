# Logic Checker Report

**Paper**: Random approximate values over algebraic types
**Date**: 2026-05-29
**Scope**: Proof correctness, logical chain integrity, claim support. All numerical/symbolic claims were re-derived independently (sympy) by the reviewer.

## Verdict

The mathematical core is **sound**. Every theorem, corollary, and worked numeric I could mechanize checks out exactly. The logical defects are not in the derivations but in (a) one unstated regularity assumption, (b) a C++ realization that contradicts the paper's own lifting equations, and (c) one notation/count inconsistency in the summary table. Confidence: **high** on the verified items.

## Independently verified (all PASS)

| Claim | Location | Result |
|---|---|---|
| AND-gate P(correct) table (4 rows) | error_propagation.tex, approx_and | All 4 rows reproduced exactly |
| `thm:approx_and` conditional rates P(c=1\|a,b) (4 rows) | bernoulli_boolean_algebra.tex | All 4 reproduced exactly |
| `thm:approx_or` conditional rates (4 rows) | bernoulli_boolean_algebra.tex | All 4 reproduced exactly |
| AND output FNR `= 1-(1-w1)(1-w2)` | error_propagation.tex | Correct |
| `cor:bitvec_correct` product correctness | bernoulli_boolean_algebra.tex | Correct for x_i in {0,1} |
| `thm:error_prop` transition T = M_A·F, dims \|A\|x\|B\| | error_propagation.tex | Dimensions and entries correct; rows normalize |
| `thm:sum_error` (Left(a) case) probabilities | composite_types.tex | Sum over all outcomes = 1 (normalizes) |
| `thm:violation_bound` `= e·\|T minus T_P\|/(\|T\|-1)` | invariants.tex | Correct under stated symmetric-channel hypothesis |
| Violation corollary endpoint (\|T_P\|=1 gives e) | invariants.tex | Correct |
| Rational example `e/(2K)`, 32-bit `approx 2.3e-10` | invariants.tex | `1/(2^32-2)=2.328e-10`, matches |
| `thm:product_parsimony` proof (rows are outer products) | composite_types.tex | Logically valid |
| `thm:bitvec_kronecker` (2n params, general 2^n(2^n-1)) | bernoulli_boolean_algebra.tex | Correct |

## Findings

### MAJOR-1: C++ `amap` contradicts the paper's own lifting equations
- **Location**: cpp.tex, "Approximate value monad" subsection, the `amap` listing.
- **Quoted text**:
  > `auto amap(function<bool(bool)> f, approx<bool> x)`
  > `{`
  > `	auto fnr = f(true);`
  > `	auto fpr = f(false);`
  > `	return approx<bool> { fpr, fnr, f(value(x)) };`
  > `}`
- **Problem**: This is presented as the realization of "applying `f` to an approximate value ... as described in `\cref{sec:error_prop}`." It is inconsistent with the paper's own derivation in three ways:
  1. `f(true)` and `f(false)` are **Boolean** outputs of `f`, but they are assigned to the rate fields `fnr`/`fpr` (which approx_values.tex defines as probabilities in [0,1]). A Boolean is not a rate.
  2. The output rates **do not depend on the input rates** `x.fpr`, `x.fnr` at all. But Eqs (lift_cond_0)/(lift_cond_1) in approx_values.tex establish that the lifted rates are functions of the *input* rates (for `f = id` the rates pass through unchanged; for `f = not` they swap). The code cannot reproduce either behavior.
  3. The aggregate-initialization order `{fpr, fnr, value}` (per the `struct approx<bool>` definition) is fed `{f(false), f(true), ...}`, so even the field labelling is crossed.
- **Why it matters**: the cpp section is contribution 5's evidence ("computational realization as a C++ monad"). As written, the central code listing does not implement the monad the paper derives, so the realization claim is unsupported by its own artifact.
- **Suggestion**: Replace with a `bind`/`fmap` that maps the *input channel* through `f`. For `f: Bool->Bool` the correct lifted channel is the row-permutation/contraction of the input 2x2 matrix per `thm:error_prop`. Concretely: compute output (fpr', fnr') from (x.fpr, x.fnr) and f's truth table (identity passes through, negation swaps, constants give exact output). A reference implementation already exists at `bernoulli-cpp/include/bernoulli/bernoulli_bool.hpp` and `bool_coder.hpp`; cite or reuse it.
- **Cross-verify**: routed to methodology-auditor (does the artifact support the reproducibility/realization claim?).

### MINOR-1: Interval FPR lower bound assumes rates below 1/2 without saying so
- **Location**: error_propagation.tex, Remark "Interval arithmetic for composed outputs".
- **Quoted text**:
  > `\fprate_{\mathrm{out}} \in \bigl[\fprate_1 \fprate_2,\; \max\bigl\{(1 - \fnrate_1)\fprate_2,\; \fprate_1(1 - \fnrate_2)\bigr\}\bigr]`
- **Problem**: The three latent-conditional false-positive values are `e1 e2`, `(1-w1)e2`, `e1(1-w2)`. The claimed lower endpoint `e1 e2` is the minimum **only if** `e1 <= 1-w1` and `e2 <= 1-w2`, i.e. when error rates are below 1/2. For pathological rates above 1/2 the interval endpoints can invert. Verified symbolically.
- **Suggestion**: Add a one-line hypothesis "for error rates below 1/2 (the regime of interest)" or prove the ordering. Low effort.

### MINOR-2: Exponential-type row of the summary table, count vs. factorization notation disagree
- **Location**: composite_types.tex, type_summary table, exponential row.
- **Quoted text**: factorization `\bigotimes_{x \in A} M_B`, parameters `|\Set{A}| \cdot \mathrm{params}(B)`.
- **Problem**: The Kronecker symbol uses a single `M_B` for every input `x`, which denotes the *homogeneous* case (all per-input channels identical), whose free-parameter count would be `params(B)`, not `|A|·params(B)`. The count `|A|·params(B)` is correct only when each input has its **own** channel `M_x`. Notation and count describe two different models.
- **Suggestion**: Write the factorization as `\bigotimes_{x \in A} M_x` to match the count, or keep `M_B` and change the count to `params(B)` for the homogeneous case. The body text (approx_values, primary mechanism subsection) already says "per-input channel `M_x`", so `M_x` is the intended reading.

### MINOR-3: "Monad" asserted, laws not exhibited
- **Location**: approx_values.tex "Monadic structure" subsection and cpp.tex.
- **Quoted text**: "This gives `\AT{(\cdot)}` the structure of a *monad* ..."
- **Problem**: The text calls `AT(·)` a monad and `bind` the function-application operation, but never states unit/return, never gives the bind type `AT(A) x (A -> AT(B)) -> AT(B)`, and never checks the three monad laws (left identity, right identity, associativity). The composition law `M_1...M_k` is consistent with associativity but is not presented as a monad-law verification.
- **Suggestion**: Either (a) demote "monad" to "monadic-style composition" and keep the channel-product law as the real content, or (b) state return (a value with the identity channel) and verify the three laws (one short paragraph; associativity is matrix-product associativity). Option (a) is the lower-risk fix for an early draft.

### Logical-chain integrity (no defects found)
- The intro's four contributions each map to a stated, proven result (C1 to prims/composite definitions; C2 to `thm:product_parsimony`; C3 to `rem:sum_nonfactor` plus `thm:sum_error`; C4 to `ex:optional`/`rem:convergence`). No orphan claims.
- The "two perspectives converge at the exponential type" narrative (intro, `rem:convergence`) is internally consistent: constructive per-input independence and denotational function-space channel give the same `⊗_x M_x`. This is asserted, not proven, but it is a definitional identification, not a theorem requiring proof.
- `thm:error_prop`'s subtle point (T is `|A|x|B|`, not a confusion matrix on B, because distinct inputs collapsing under f keep distinct rows) is stated correctly and the injective re-indexing caveat is right.

## Summary
No critical logic errors. One major: the flagship C++ listing does not implement the derived semantics (and indeed contradicts the paper's own lifting equations). Three minors are easily fixed and would tighten rigor. The theory is in good shape for an early draft; the gap is the realization section, not the mathematics.
