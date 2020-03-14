# Random variables
The *sample space* of an experiment is the set of mutually exclusive *outcomes* that may be observed. Sets of outcomes are denoted *events* where an event that contains one outcome is denoted an *elementary event* and the event containing all outcomes is the sample space.

The set of all possible events is a sigma-algebra. Suppose the sample space is `X`. A sigma-algebra on `X` is a set `A` of subsets of `X` that satisfies the following conditions:
1. `A` includes `X`, the sample space.
2. `A` includes `{}`, the empty set.
3. `A` is *closed* under countable unions, intersections, and complements.

If these conditiosn are met, the pair `(X, A)` denotes a *Borel space*. For instance, if `X` is some countable set, `A` is typically the power set of `X`.

Suppose the outcome of the experiment is *uncertain*. Any event `E` in `A` that includes the outcome has been satisfied by the experment. Let `P : A -> [0,1]` be a countably additive function that maps events to *probabilities*. The triple `(X, A, P)` defines a *probability space*. The relative frequency of any event `E` in `A` converges to probability `P(E)` as the number of times the experiment is repeated goes to infinity.

The concept `RandomVariable` is a measurable function `W : X -> Y` from outcomes `X` to a measurable space `Y`. The probability that `W` realizes some measureable subset `S` of `Y` is given by `Pr(W in S) = P({w in X | W(w) in S})`.

Normally, the measurable space `Y` are `Reals` which makes it possible to define concepts like
`ExpectedValue` and `Variance`.

A `RandomVariable` `X` is modeled by a `UnivariateDistribution`. A univariate distribution `D` should have at least one of the following:

1. A probability density (or mass) function, `pdf : D -> (W -> R)`, where `W` is the measurable sample space of the random variable.

2. A cumulative distribution function, `cdf : D -> (W -> R)`.

## Functions of random variables

We provide a generic interface to random variables that allows for various compositions of random variables, many of which are closed under some operation set.

## Limits

We also provide *limiting* distributions, i.e., if we take the limit of a sum of random variables, by the central limit theorem the result is a normal random variable.

## Random vectors: jointly distributed random variables

More complicated mathematical structures may be of interest, such as a `RandomVector`,`RandomSet`, `RandomMatrix`, or more generally `RandomElement`. In any case, we may model these by the concept of a `MultivariateDistribution`.

The most popular case is given by the concept of a `RandomVector`, which is just `n` random variables `(W_1, ..., W_n)` which may be *correlated*.
