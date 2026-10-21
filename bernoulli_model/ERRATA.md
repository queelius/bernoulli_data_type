# Errata: The Bernoulli Model: A Probabilistic Framework for Data Structures and Types

Date: 2026-10-21. Author: Alexander Towell.

This is the final version of the preprint. It will be deposited on Zenodo as a
new version under concept DOI 10.5281/zenodo.19105398, superseding version
10.5281/zenodo.19105399 (v1.0.0, Zenodo title "The Bernoulli model: a
probabilistic framework for data structures and types"). It is superseded by
the monograph *Bernoulli Types: Two-Sided Error in Approximate Membership,
Retrieval, and Computation*, Chapters 1 to 4 (the noisy bit, binary channels
and model order, Boolean algebra under noise, the two axioms) and Chapter 13
(The Algebra of Approximate Types)
(<https://metafunctor.com/latex/bernoulli-monograph/book.html>).
License unchanged: CC BY 4.0.

A "Status and errata (October 2026)" box now follows the abstract
(`status_errata.tex`).

## Corrections

1. **Abstract (`main.tex`).** The previous abstract said the paper
   "introduces the Bernoulli Model", explores Count-Min sketches and
   "introduces" cipher data types. The model is introduced in the companion
   Bernoulli sets paper, Count-Min sketches are never analyzed, and cipher
   data types are only a motivation. The abstract now describes the note as a
   type-indexed presentation of known results.
2. **Introduction (`sections/intro.tex`).** Added a "Prior work" paragraph:
   randomized response, EnerJ, Rely, Chisel, Decaf, Uncertain<T>, the
   probability monad, signal-probability analysis and probabilistic transfer
   matrices. It states that a Decaf-style product of correctness
   probabilities is the probability that no input or operation is wrong (a
   lower bound on correctness), not the error rate of the final output. The
   unanalyzed Count-Min mention was dropped from the intro's list of
   structures.
3. **Set-indicator functions, symbol conflation (`sections/set_indicator.tex`).**
   One symbol, epsilon, denoted both the per-element false-positive rate
   `(k+1) 2^-n` and the per-seed success probability `((k+1) 2^-n)^m`. The
   success probability is now `P_seed = epsilon^m`. The prose said a
   nonmember is accepted "with probability `k 2^-n`" and that members must
   hash "less than k"; both are now `(k+1) 2^-n` and "at most k", matching the
   predicate `h(xs) <= k`. The width relation `n = log(k/epsilon)` is
   corrected to `n = log2((k+1)/epsilon)`. (Source: 2026-05-29 review, C1,
   M-B, M-C.)
4. **Set-indicator functions, two-level scheme (`sections/set_indicator.tex`).**
   The expected trial count `E[N] = b / P_seed(m/b,n,k)` was printed as
   `(2^n/(k+1))^(m/b)`, dropping the factor `b`; now `b (2^n/(k+1))^(m/b)`.
   The space claim of `n/b` (equivalently `-log epsilon / b`) bits per element
   was wrong: it lies below the `-log2 epsilon` lower bound stated a few lines
   earlier. The scheme stores `b` seeds of about `nm/b` bits each, `nm` bits
   in total, i.e. `-log2 epsilon` bits per element, the same as one level.
   Bucketing reduces construction time, not space. The text no longer calls
   the bucket count a time-space tradeoff. (Source: 2026-05-29 review, C2,
   M-A.)
5. **Seed-search lineage (`sections/set_indicator.tex`).** The brute-force
   seed search is credited to the standard minimal-perfect-hashing and
   retrieval lineage (RecSplit; Lehmann et al. ESA 2025 for the one-seed
   case); no novelty is claimed.
6. **Logic gates (`sections/logic_gates.tex`).** "AND ... induces an output
   that is a fourth-order Bernoulli Boolean" is reworded: the model is
   fourth-order over the input space, and no single pair of output rates
   describes it. Interval propagation through composed circuits is restricted
   to read-once circuits (shared inputs make gate inputs dependent), with the
   note that carrying one pair of output rates from gate to gate can
   understate the exact error. Credited Parker and McCluskey 1975 and
   probabilistic transfer matrices (2008). The conclusion no longer promises
   guarantees "throughout complex computations".
7. **Error propagation in complex data types (`sections/bernoulli_maps.tex`).**
   "If a single input to the circuit is replaced with a Bernoulli Boolean, the
   output becomes one or more Bernoulli Booleans" now says the output's error
   rates depend on the latent input assignment, so it is a higher-order model
   rather than a Bernoulli Boolean with one fixed pair of rates.
8. **Lifting (`sections/lifting.tex`).** A stray Markdown blockquote (`> It is
   very unlikely ...`), which rendered as a literal character, is now a prose
   sentence. The monadic lift is credited to the probability (Giry) monad.

## Citations added

Each DOI was checked against the Crossref REST API on 2026-10-21 (title and
authors match), unless marked otherwise.

- `boston2015decaf`: B. Boston, A. Sampson, D. Grossman, L. Ceze, *Probability
  Type Inference for Flexible Approximate Programming*, OOPSLA (2015).
  10.1145/2814270.2814301.
- `carbin2013rely`: M. Carbin, S. Misailovic, M. C. Rinard, *Verifying
  Quantitative Reliability for Programs That Execute on Unreliable Hardware*,
  OOPSLA (2013). 10.1145/2509136.2509546.
- `bornholt2014uncertain`: J. Bornholt, T. Mytkowicz, K. S. McKinley,
  *Uncertain<T>: A First-Order Type for Uncertain Data*, ASPLOS (2014).
  10.1145/2541940.2541958. Crossref lists only the short title "Uncertain< T >".
- `sampson2011enerj`: A. Sampson et al., *EnerJ: Approximate Data Types for
  Safe and General Low-Power Computation*, PLDI (2011). 10.1145/1993498.1993518.
  Crossref lists only the short title "EnerJ".
- `misailovic2014chisel`: S. Misailovic et al., *Chisel: Reliability- and
  Accuracy-Aware Optimization of Approximate Computational Kernels*, OOPSLA
  (2014). 10.1145/2660193.2660231. Crossref lists only the short title "Chisel".
- `giry1982categorical`: M. Giry, *A Categorical Approach to Probability
  Theory*, Lecture Notes in Mathematics (1982). 10.1007/BFb0092872.
- `ramsey2002stochastic`: N. Ramsey, A. Pfeffer, *Stochastic Lambda Calculus
  and Monads of Probability Distributions*, POPL (2002). 10.1145/503272.503288.
- `parker1975probabilistic`: K. P. Parker, E. J. McCluskey, *Probabilistic
  Treatment of General Combinational Networks*, IEEE Trans. Computers C-24(6)
  (1975). 10.1109/T-C.1975.224279.
- `krishnaswamy2008ptm`: S. Krishnaswamy, G. F. Viamontes, I. L. Markov,
  J. P. Hayes, *Probabilistic Transfer Matrices in Symbolic Reliability
  Analysis of Logic Circuits*, ACM TODAES 13(1) (2008). 10.1145/1297666.1297674.
- `esposito2020recsplit`: E. Esposito, T. Mueller Graf, S. Vigna, *RecSplit:
  Minimal Perfect Hashing via Recursive Splitting*, ALENEX (2020).
  10.1137/1.9781611976007.14.
- `lehmann2025seeds`: H.-P. Lehmann, P. Sanders, S. Walzer, J. Ziegler,
  *Combined Search and Encoding for Seeds, with an Application to Minimal
  Perfect Hashing*, ESA 2025, LIPIcs 351, 109. 10.4230/LIPIcs.ESA.2025.109.
  This DOI is registered with DataCite, not Crossref; verified via
  api.datacite.org (title and authors match).

The previously uncited bibliography entry `warner1965` (randomized response)
is now cited.

## Claims withdrawn or narrowed

- That this paper introduces the Bernoulli model or develops Count-Min
  sketches.
- That bucketed seed search saves space (it saves construction time only).
- That interval-valued output rates compose through arbitrary circuits (only
  read-once circuits with independent gate inputs).
- That the output of a circuit with noisy inputs is a Bernoulli Boolean with
  one pair of rates.

## Build

`pdflatex main.tex && bibtex main && pdflatex main.tex && pdflatex main.tex`
(no `TEXINPUTS` needed). Result on 2026-10-21: 0 LaTeX errors, 0 undefined
references, 0 undefined citations, 0 BibTeX warnings; 24 pages (21 before the
freeze). The status box is on page 2, immediately after the abstract (the
abstract and title fill page 1).

## Known remaining issues (not fixed in the freeze)

- Notation remains mixed (`\Ber{}` / `\Berr{}{}` versus literal `B_{...}`),
  including one-argument misuses such as `\Ber{\Bool}{1}` in `lifting.tex`.
- The "Higher-Order Bernoulli Models" passage in `lifting.tex` still reads
  "first-order composed with first-order yields second-order" by example,
  which can be confused with k-fold composition depth.
- The majority-vote MLE remark in `prediction.tex` lacks the i.i.d. and
  symmetric qualifier.
- `bernoulli_maps.tex` overlaps the companion Bernoulli maps paper.
- Miller-Rabin is described without a citation; pre-existing minor overfull
  boxes remain (one of about 51 pt in `bernoulli_maps.tex`).
