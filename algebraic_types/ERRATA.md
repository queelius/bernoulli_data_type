# Errata: Random approximate values over algebraic types

Date: 2026-10-21. Author: Alexander Towell.

This is the final version of the preprint. It will be deposited on Zenodo as a
new version under concept DOI 10.5281/zenodo.19105395, superseding version
10.5281/zenodo.19105396 (v1.0.0). It is superseded by the monograph
*Bernoulli Types: Two-Sided Error in Approximate Membership, Retrieval, and
Computation*, Chapter 13 (The Algebra of Approximate Types), with Chapters 3
and 4 for the gate formulas and the Kronecker factorization
(<https://metafunctor.com/latex/bernoulli-monograph/book.html>).
License unchanged: CC BY 4.0.

A "Status and errata (October 2026)" box now follows the abstract
(`status_errata.tex`).

## Corrections

1. **Abstract (`main.tex`).** One sentence added: the results are elementary
   consequences of independence, presented in type-indexed form, with the
   closest predecessors named.
2. **Introduction (`sections/intro.tex`).** The "Contributions" paragraph
   (which said "we prove" that products preserve the Kronecker factorization)
   is now "Content" and states that the listed results are elementary
   consequences of per-component independence, with no novelty claimed. A
   "Prior work" paragraph credits EnerJ, Rely, Chisel, Decaf (the closest
   predecessor: probability types whose correctness probabilities compose as
   products), Uncertain<T>, the probability monad (Giry; Ramsey and Pfeffer),
   signal-probability analysis (Parker and McCluskey) and probabilistic
   transfer matrices, and randomized response. It notes that a product of
   correctness probabilities is the probability that no input or operation is
   wrong, a lower bound on correctness, not the final output's error rate.
3. **Monadic structure (`sections/approx_values.tex`).** "This gives the
   approximate type the structure of a monad ... whose error rates are
   determined by the composition of the respective confusion matrices" now
   identifies the probability (Giry) monad, and says that a confusion matrix
   on the output type alone is not closed under bind when the function is not
   injective (distinct latent inputs with the same latent output can have
   different output laws), consistent with the paper's own Theorem on error
   propagation through function application.
4. **Interval arithmetic for composed outputs (`sections/error_propagation.tex`).**
   "This interval approach ... is composable through further operations" is
   restricted to read-once circuits (monotonicity of AND/OR/NOT in each
   independent input probability); shared inputs invalidate the product
   formulas, and reusing a single corner pair of rates (such as
   `eps1 * eps2`) as the input rate of a later gate can understate the exact
   error. (Counterexample of this kind, for `(b0 AND b1) OR b2` at latent word
   010 with eps = 0.02, omega = 0.10: exact error 0.03764 versus 0.020392 from
   pair reuse; see `.research/publication-triage/candidates/quantum-readout/`.)
5. **Composition of approximate functions (`sections/error_propagation.tex`).**
   The product formula `M_A * M_f` is scoped to approximate maps whose
   per-input channel does not depend on the input; the general form
   `sum_t (M_A)_{s,t} (M_t)_{f(t),.}` is given.
6. **Boolean algebra on bit vectors (`sections/bernoulli_boolean_algebra.tex`).**
   The remark that approximate intersection has per-element FPR `eps1 eps2`
   and union has per-element FNR `omega1 omega2` is scoped to elements absent
   from both inputs (resp. present in both); elements in exactly one input
   have the mixed middle-row rates, so the composite is a higher-order model.
   The component-wise AND corollary now says the factorization is of
   per-position 4x2 transition matrices indexed by the latent input pair, not
   of 2x2 channels on the output bit. The gate formulas are credited to
   signal-probability analysis and probabilistic transfer matrices.
7. **Exponential types (`sections/composite_types.tex`).** "The composition
   theorem ... from the companion paper on Bernoulli maps apply directly" is
   qualified: composition of a realized approximate map is not closed under
   the independent-input law when two inputs share an intermediate value (an
   error frozen at the shared value is common to both outputs), as recorded
   in the Bernoulli maps errata.
8. **C++ realization (`sections/cpp.tex`).** The `amap` listing assigned the
   Booleans `f(true)` and `f(false)` to the rate fields, contradicting the
   paper's lifting equations. It now keeps the rates for the identity, swaps
   them for negation, and returns an exact value for constant functions. The
   text called the listing `fmap`; it is `amap`. The cardinalities of `int`,
   `bool`, `char` were listed as `2^32, 2^8, 2^1` against that order; now
   `2^32, 2^1, 2^8`. The claim that a program's false-positive and
   false-negative rates can be "deduced ... through function composition" now
   says the output law is computed conditional on each latent input
   assignment, and a single pair of rates summarizes it only after averaging
   over latent inputs; one stored pair per value is exact for unary functions
   but not for multi-argument operations in general. The math-mode
   `\textless`/`\textgreater` warnings on that line were removed.

## Citations added

Each DOI was checked against the Crossref REST API on 2026-10-21 (title and
authors match).

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

The previously uncited entry `warner1965` (randomized response) is now cited.

## Claims withdrawn or narrowed

- That the listed results are contributions proved here (they are elementary
  consequences of independence).
- That an output-only confusion matrix is preserved by monadic bind.
- That interval or corner-pair output rates compose through arbitrary
  circuits.
- That intersection and union of approximate bit vectors have the single
  per-element rates `eps1 eps2` and `omega1 omega2`.

## Build

`TEXINPUTS=".:./sty:" pdflatex -shell-escape main.tex && bibtex main &&
TEXINPUTS=".:./sty:" pdflatex -shell-escape main.tex &&
TEXINPUTS=".:./sty:" pdflatex -shell-escape main.tex`.
Result on 2026-10-21: 0 LaTeX errors, 0 undefined references, 0 undefined
citations, 0 BibTeX warnings; 20 pages (18 before the freeze). The status box
is on page 1, immediately after the abstract.

## Known remaining issues (not fixed in the freeze)

- The sum-type theorem assumes a uniform payload on a tag error; it is a
  modeling choice, not derived.
- Some C++ snippets in `cpp.tex` are not valid C++ (for example the struct
  literal and the trailing commas after function definitions) and are
  illustrative only.
- `sections/bernoulli_type_2.tex` is an excluded orphan and was not edited
  (the `main.tex` comment also names `bernoulli_type.tex` and
  `bernoulli_prolog.tex`, which no longer exist on disk).
- No external positioning against refinement or liquid types was added.
