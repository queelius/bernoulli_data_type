# Logic Checker Report

**Paper**: The Bernoulli Model: A Probabilistic Framework for Data Structures and Types
**Reviewer lens**: proof correctness, logical chain integrity, claim support
**Date**: 2026-05-29

## Summary

The paper has no formal theorem/proof environments; its logical content is a chain of derivations and confusion-matrix arguments. Most of these are correct, and I independently re-derived the central ones. The Bayes prediction derivation and the `and`-gate error table are sound. The serious logical defects are concentrated in one section, `set_indicator.tex`, where a single symbol is used for two different quantities and a space-complexity claim contradicts a lower bound stated a few lines earlier. Several smaller chain-of-reasoning gaps appear in `lifting.tex` and `prediction.tex`.

## Findings

### CRITICAL

**L1. `set_indicator.tex`: the symbol `epsilon(m,n,k)` conflates the seed-search success probability with the false-positive rate.**
- Location: `set_indicator.tex`, lines 21--34 and 62.
- Quoted: "each trial is Bernoulli distributed with probability of success given by [`epsilon(m,n,k) = ((k+1)/2^n)^m`]" (lines 21--27), then "Since the false positive rate `epsilon = (k+1)2^{-n}`" (line 34), then "Since `epsilon = 2^{-n}`, `n = -log epsilon`" (line 62).
- Problem: two distinct quantities carry the same name `epsilon`. The per-element false-positive rate of one non-member is `(k+1)/2^n` (a single channel crossover). The probability that one random seed maps *all* `m` members below the threshold is `((k+1)/2^n)^m`. The text defines `epsilon(m,n,k) = ((k+1)/2^n)^m`, calls it the "probability of success" for the geometric seed search (correct), and then in the very next lines treats `epsilon` as the false-positive rate (lines 34, 62, 73). `E[N] = 1/epsilon(m,n,k)` is the correct expected number of seed trials only under the product reading; `n = -log epsilon` is the correct bits-per-element only under the per-element reading. The two readings are silently swapped. I verified symbolically that `((k+1)/2^n)^m` and `(k+1)/2^n` are different functions and that the downstream formulas pick whichever reading they need.
- Suggestion: introduce two symbols, e.g. `epsilon` for the per-element false-positive rate `(k+1)/2^n` and `P_seed = epsilon^m` (or `epsilon_+`) for the per-seed success probability. Rewrite `E[N] = 1/P_seed` and keep `n = -log_2 epsilon` for the rate. This repair is local but mandatory; as written the section's quantitative claims are not well defined.

**L2. `set_indicator.tex`: the two-level space claim `n/b` bits per element contradicts the information-theoretic lower bound stated in the same section.**
- Location: `set_indicator.tex`, lines 38--40 versus lines 56--66.
- Quoted: at line 38--40, for `k=0`, "`epsilon = 2^{-n} => n = -log epsilon` bits per element, which is the information-theoretic lower-bound". Then at lines 57--60, the two-level scheme stores the seed in "`nm/b` bits in total, or `n/b` bits per element", and line 62--66 reparameterizes this as "`-log(epsilon)/b` bits per element".
- Problem: `-log(epsilon)/b` bits per element is strictly below the `-log epsilon` lower bound the section just invoked (for any `b > 1`). The error is in the storage accounting: the two-level scheme needs `b` independent bucket seeds, not one. Each bucket seed costs about `n(m/b)` bits, so the total is `b * n(m/b) = nm` bits, i.e. `n` bits per element, the same as single-level (I re-derived this). The text drops the factor of `b` (the same factor it drops in the `E[N]` algebra; see L3), arriving at a figure that beats the Shannon-type bound. A construction cannot represent a set in fewer than `-log epsilon` bits per element at false-positive rate `epsilon`.
- Suggestion: redo the accounting with `b` separate seeds. The two-level scheme buys *time* (lower `E[N]`), not *space*; the corrected space stays at `-log epsilon` bits per element. Remove the `-log(epsilon)/b` claim or reframe it explicitly as a time saving.

### MAJOR

**L3. `set_indicator.tex`: the two-level `E[N]` identities silently drop a leading factor of `b`.**
- Location: lines 47--53.
- Quoted: "`E[N] = b/epsilon(m/b,n,k) = (2^n/(k+1))^{m/b}`" (lines 47--49) and, for `k=0`, "`E[N] = b/epsilon(m/b,n,k) = 2^{nm/b}`" (lines 50--53).
- Problem: `b / ((k+1)/2^n)^{m/b}` equals `b * (2^n/(k+1))^{m/b}`, not `(2^n/(k+1))^{m/b}`; the right-hand sides omit the factor `b`. I verified the ratio of LHS to RHS is exactly `b` in both the general and `k=0` cases. (Whether the leading `b` even belongs depends on what `E[N]` is meant to count, per-bucket expected trials versus total, which the text does not pin down, compounding L1.)
- Suggestion: state precisely what `E[N]` counts (expected trials per bucket, or summed over buckets), then write the identity that actually holds.

**L4. `set_indicator.tex`: `n = log(k/epsilon)` is dimensionally/algebraically inconsistent with `epsilon = (k+1)2^{-n}`.**
- Location: lines 34--35.
- Quoted: "Since the false positive rate `epsilon = (k+1)2^{-n}`, `n = log(k/epsilon)`, we can reparameterize the space complexity as `log(k/epsilon)` bits per element."
- Problem: solving `epsilon = (k+1)/2^n` for `n` gives `n = log_2((k+1)/epsilon)`, not `log(k/epsilon)`. The numerator should be `k+1`, not `k` (this is the same k-versus-(k+1) slip as below), and the logarithm base should be 2 and stated. At `k=0` the text's own special case uses `n = -log epsilon`, which is `log_2((0+1)/epsilon)`, i.e. consistent with `k+1` but not with `k`.
- Suggestion: write `n = log_2((k+1)/epsilon)` and fix the base throughout.

**L5. `set_indicator.tex`: per-element false-positive probability stated as `k 2^{-n}` but used as `(k+1) 2^{-n}`.**
- Location: lines 17 and 21 versus lines 22--27 and 34.
- Quoted: line 17, "for `x notin A`, `h(x.s) <= k` with probability `k 2^{-n}`"; line 21, "the probability that all `x in A` hash to a value less than `k` occurs with probability `k 2^{-n}`"; but the displayed product (lines 22--27) uses `(k+1)/2^n` per factor and line 34 sets `epsilon = (k+1)2^{-n}`.
- Problem: the acceptance predicate is `h(x.s) <= k`, i.e. the values `{0,1,...,k}`, which is `k+1` outcomes out of `2^n`, giving probability `(k+1)/2^n`. The prose `k 2^{-n}` is off by one. Also note line 21 says "hash to a value *less than* `k`" while the predicate (lines 16, 18) is "`<= k`" (at most `k`); "less than" would be `k` outcomes, "at most" is `k+1`. The inconsistency between "less than" and "at most" is the root of the off-by-one.
- Suggestion: fix the predicate wording to "at most `k`" everywhere and use `(k+1)/2^n` uniformly (or define the predicate as `< k` and use `k/2^n` uniformly). Pick one and propagate.

### MINOR

**L6. `lifting.tex`: order arithmetic for composition is asserted without the rule that produces it.**
- Location: lines 109--113.
- Quoted: "Suppose the output type is a first-order Bernoulli Boolean ... if we input a `Ber{Bool}` value, we get a higher-order `Ber{Bool}` as output, e.g., `Ber{f}{2} : Ber{Bool}{1} -> Ber{Bool}{2}`."
- Problem: the jump from first-order input to second-order output is stated by example without a general rule for how orders add under composition. The next sentence correctly invokes the channel-matrix product `M^k` from `bernoulliSets`, but the specific claim "first-order composed with first-order yields second-order" is a statement about *order* (partition-block count), and composition of two first-order (symmetric) channels yields another symmetric channel, which is still first-order, not second-order. The intended claim is probably about *k-fold composition depth* (a `sigma^2` two-fold composition), not about the partition-block order rising to 2. This is exactly the model-order-versus-composition-depth conflation the family is careful to avoid elsewhere.
- Suggestion: state explicitly whether the superscript denotes partition-block order or composition depth `sigma^k`, and align with `bernoulliSets` Theorem on `k`-fold composition. As written it reads as a category error.

**L7. `logic_gates.tex`: "fourth-order" output is asserted, but it is a statement about input-conditioned cases, not partition-block order.**
- Location: lines 88--91.
- Quoted: "`and : (B_Bool^{(1)}, B_Bool^{(1)}) -> B_Bool^{(4)}` induces an output that is a fourth-order Bernoulli Boolean. How is this possible when there are only two possible outputs? The answer is that the output is dependent on four different combinations of inputs."
- Problem: the four `Pr{correct}` values are correct (verified). But calling the output a "fourth-order Bernoulli Boolean" overloads "order". Per the paper's own definition (`order_and_channels.tex`, Remark `rem:order_vs_dof`), order is the number of partition blocks of the *domain*. Here the domain of the *output* Boolean is still `{0,1}`; the four distinct correctness probabilities arise from four input combinations, i.e. four partition blocks of the *input* space `(Bool,Bool)`. The output Boolean per se does not have four blocks. The statement is defensible only if "order" is read as the number of distinct rows of the joint input-conditioned confusion table, which is precisely the reading the family's CLAUDE notes warn is orthogonal to output-symbol count. Internally consistent if interpreted charitably, but the phrasing invites the very confusion the family flags.
- Suggestion: say "the output correctness partitions the input space `(Bool,Bool)` into four blocks, hence a fourth-order model over that input space," rather than "fourth-order Bernoulli Boolean."

**L8. `prediction.tex`: "maximum likelihood estimate is the majority vote" needs the i.i.d. + symmetric qualifier it half-states.**
- Location: lines 23--31.
- Quoted: "the maximum likelihood estimate of `x` is the majority vote ... Assuming the error rate is not larger than 0.5" (footnote), and "where `Ber{x}_i` are independent Bernoulli variables (they do not need to be identically distributed)."
- Problem: majority vote is the MLE only for the *symmetric* (first-order) channel with a common error rate below 0.5. The text simultaneously says observations need not be identically distributed; if the per-source error rates differ, the MLE is a *weighted* vote (weights `log((1-e_i)/e_i)`), not a plain majority. The convergence claim (LLN) is fine, but the MLE = majority claim is false in the non-identically-distributed case the same paragraph allows.
- Suggestion: restrict the majority-vote MLE claim to the identically-distributed symmetric case, or replace with the weighted-vote MLE for the general case.

**L9. `prediction.tex`: notation `Ber{x}` versus `Ber{T}(x)` is used inconsistently inside the Bayes derivation.**
- Location: lines 9--21.
- Quoted: "`Pr{X = x | Ber{x} = x} ∝ Pr{Ber{x} = x | X = x} Pr{X = x}`" (line 11) where elsewhere the observable is written `Ber{T}(x)` (e.g. line 6).
- Problem: `Ber{x}` (hat over the value) and `Ber{T}(x)` (hat over the type, applied to `x`) are used for the same observable within a few lines. This is a notation defect (see prose report P-series) but it also slightly muddies the logic: `Pr{X=x | Ber{x}=x}` reads as conditioning the latent on itself. The intended meaning is clear, but the event should be written as the observation equalling the value, e.g. `Pr{X = x | \widehat{X} = x}` with a distinct symbol for the observation.
- Suggestion: fix the notation (coordinate with the prose auditor's global notation pass).

## Claims independently verified as CORRECT

- `prediction.tex` lines 134--140: the posterior simplification `(1-eps)/(1 - eps(1 - q/(1-q)))` equals the true Bayes posterior; the three boundary cases (`q=0 -> 1`, `q->1 -> 0`, `q=1/2 -> 1-eps`) all hold.
- `logic_gates.tex` Table `tbl:and_bernoulli`: all four `Pr{correct}` entries are correct.
- `prediction.tex` confusion-matrix Tables `tbl:second_order`, `tbl:first_order`, `tbl:zeroth_order`: rows sum to 1; the first-order reduction `epsilon = epsilon = eta` is consistent.
- `set_indicator.tex` Table `tbl:set_indicator`: rows sum to 1 and correctly encode a one-sided (no-false-negative) construction.
- `order_and_channels.tex` Remark `rem:order_vs_dof`: the `n(k-1)` parameter count and the order-versus-DOF distinction are stated correctly and consistently with the foundation paper.

## Confidence

High on L1--L5 (re-derived symbolically). Medium on L6--L7 (interpretation of "order" hinges on a definition the paper states but then applies loosely). High on the verified-correct list.
