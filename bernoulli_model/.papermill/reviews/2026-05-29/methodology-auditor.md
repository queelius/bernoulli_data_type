# Methodology Auditor Report

**Paper**: The Bernoulli Model: A Probabilistic Framework for Data Structures and Types
**Reviewer lens**: experimental design, statistical/derivational rigor, reproducibility
**Date**: 2026-05-29

## Summary

This is a theory paper with no experiments, so "methodology" here means the rigor of the probabilistic modeling, the well-definedness of the constructions, and whether a reader can reproduce the derivations. The modeling apparatus (confusion matrices, channel orders, Bayes prediction) is mostly handled rigorously and matches the conventions of the foundation paper. The one place where the methodology breaks down is the `hash_set` construction in `set_indicator.tex`, which is presented as a concrete algorithm but is under-specified and arrives at a space figure that is not achievable. I cross-checked the logic-checker's L1--L5 by independently reconstructing the construction; I concur with all five.

## Findings

### MAJOR (cross-verification of logic findings, methodology angle)

**M1. The `hash_set` algorithm in `set_indicator.tex` is not reproducible as written.**
- Location: `set_indicator.tex`, lines 12--66.
- Problem (methodology angle): a reader trying to *implement* the construction cannot, because (a) the acceptance predicate flips between "`<= k`" (line 18) and "less than `k`" (line 21); (b) the per-element false-positive rate is quoted as `k 2^{-n}` but used as `(k+1) 2^{-n}`; (c) `epsilon(m,n,k)` is defined as the seed-search success probability `((k+1)/2^n)^m` but then used as the false-positive rate; (d) the two-level scheme never says how many seeds are stored (one shared seed, or one per bucket), which is the crux of whether the space figure is `n` or `n/b` bits per element. I reconstructed the only internally consistent reading: predicate "`<= k`", per-element FP rate `(k+1)/2^n`, one seed *per bucket*, total space `nm` bits, i.e. `n` bits per element. Under that reading the `n/b` claim is wrong (see M2).
- Cross-verification verdict: confirms logic-checker L1, L4, L5. The defect is real and blocks reproduction.
- Suggestion: rewrite the construction as a numbered algorithm (input `A`, `n`, `k`, `b`; output the `b` seeds), define each symbol once, and state the predicate once.

**M2. The headline space result is not achievable; it would beat the stated lower bound.**
- Location: `set_indicator.tex`, lines 38--40 (lower bound) versus 56--66 (two-level claim).
- Quoted: "`n = -log epsilon` bits per element, which is the information-theoretic lower-bound" (lines 38--40); "`n/b` bits per element" and "`-log(epsilon)/b` bits per element" (lines 60, 64).
- Problem (methodology angle): the paper invokes a lower bound and then reports a construction that sits below it. From a rigor standpoint this is the most damaging issue in the paper, because the space-accuracy lower bound is one of the framework's load-bearing results (it is the subject of a sibling paper, `bernoulli_entropy`). Reporting a construction that violates it, even by a bookkeeping slip, undermines confidence in the section. The correct accounting (`b` seeds at `n m/b` bits each = `nm` bits = `n` per element) keeps the construction at the bound and reframes the two-level scheme as a *time* optimization (it lowers `E[N]` from `2^{nm}` to about `b·2^{nm/b}`).
- Cross-verification verdict: confirms logic-checker L2 and L3.
- Suggestion: present the two-level scheme as a time-space trade (faster seed search, same space), and delete the sub-lower-bound space figures.

### MINOR

**M3. Bloom filter classified as "second-order" without reconciling with the foundation paper's usage.**
- Location: `set_indicator.tex`, line 8.
- Quoted: "The Bloom filter ... is a second-order model, since false negatives occur with probability 0 and false positives occur with probability `epsilon`."
- Problem: in the foundation paper, the `fnrate = 0` case is the *positive approximate set* and is naturally described by a single rate. Calling it "second-order" is consistent with "two partition blocks (positives, negatives)", but the foundation paper would describe the Bloom filter as a positive approximate set with one free rate. The paper should state which convention it uses and note that the second block has a degenerate (zero) rate, otherwise readers will count one free parameter and wonder why it is called second-order.
- Suggestion: add a half-sentence: "second-order in the partition-block sense (positives and negatives are distinct blocks), with the positive-block error rate degenerate at 0."

**M4. "expectation of the false positive rate" claim is stated without the variance/finite-universe caveat being usable.**
- Location: `set_indicator.tex`, line 8.
- Quoted: "This is actually the *expectation* of the false positive rate, and the true false positive rate is a random variable that cannot usually be computed unless `X` is a finite set."
- Problem: this is the right instinct (the realized FP rate is a random variable, per the foundation paper's binomial error-count machinery) but it is asserted in passing with no pointer to where the distribution is characterized. For a foundational paper this is a missed opportunity to connect to the binomial/CLT results in `bernoulliSets`.
- Suggestion: cite the relevant result in `bernoulliSets` for the distribution of the realized rate.

**M5. The `Bool -> Bool` maximum-entropy confusion matrix uses `epsilon/3` off-diagonal without stating the symmetry assumption that justifies equal off-diagonals.**
- Location: `lifting.tex`, Table `tbl:max_entropy`, lines 60--74.
- Problem: the table spreads the error mass uniformly (`epsilon/3` to each of the three non-true functions). That is the maximum-entropy choice *given no preference among wrong answers*, which is a modeling assumption worth one sentence, especially since the surrounding text (lines 99--100) later argues the opposite (that near-diagonal functions should get more mass). The two passages are not contradictory, but the max-entropy table should be labeled as the no-preference baseline.
- Suggestion: one sentence: "absent any similarity structure on functions, maximum entropy spreads error mass uniformly; Section [Higher-Order] relaxes this."

## Reproducibility checklist

- Derivations reproducible: YES except `set_indicator.tex` hash_set (M1).
- Symbols defined before use: mostly; `tau, nu, eta, epsilon, varepsilon` appear with overlapping roles across `prediction.tex` (see prose report); `p` in `lifting.tex` and `p1,p2` in `logic_gates.tex` are clear.
- External results correctly invoked: Shannon BSC, Cover and Thomas asymmetric channel, channel-matrix composition all correctly attributed.
- No code/data artifacts are claimed, so none are missing.

## Confidence

High on M1 and M2 (independently reconstructed). Medium on M3--M5 (conventions and presentation rather than correctness).
