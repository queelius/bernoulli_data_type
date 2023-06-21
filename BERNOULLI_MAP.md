# Bernoulli maps

The bernoulli map quantifies a certain kind of approximation error that is compatible
with many algorithms, particularly data types that depend on hashing.

Suppose we have a *latent* function `p : X -> Y` and an observale approximation or
noisy version of `p`, denoted by `p* : X -> Y`, such that `p*(x) != p(x)` with
probability `e(x)`, where `{e(x) : x in X}` are apriori statistically independent
random variables.

When we observe `p*`, we know that it is a noisy model of some function `X -> Y`, but
we do not know which one with certainty. If we are given no other prior information,
then the best estimator of `p` is `p*`.

Let the functions of type `X -> Y` be named `p1, p2, ..., pn`, where `n` is the
total number of functions in `X -> Y`, which if no other information is given, is
given by the cardinality of `X -> Y`, which is just `n = |Y|^|X|`.

Let us show the confusion matrix for `p1, p2, p3, p4` (`n=4`, e.g., `bool -> bool`),
where the rows represent the latent functions, and the columns represent the observed
functions that may be generated from the latent function, say by introducing some
error or rate distortion.

latent \ observed | `p1`  | `p2`  | `p3`  | `p4` |
------------------|-------|-------|-------|------|
`p1`              | `q11` | `q12` | `q13` | `q14`|
`p2`              | `q21` | `q22` | `q23` | `q24`|
`p3`              | `q31` | `q32` | `q33` | `q34`|
`p4`              | `q41` | `q42` | `q43` | `q44`|

In the matrix above, `qij` represents the probability that function `pi` is observed
as function `pj`. Since each row must sum to 1, there are at maximum of `n * (n-1)`
degrees of freedom, which in this case is `4 * (4-1) = 12`.

We call this degree-of-freedom the *order* of the Bernoulli Model for `X -> Y`.
In many cases, the order is 1, e.g., where most of the probability is assigned to
the diagonal, and the off-diagonal elements are nearly zero (or zero) but all
equal.

Since we are only given `p*`, we do not know which `p` is the true latent function.
We say that `p*` is a *Bernoulli approximation* of `p`, and we deote this by
writing `p* ~ bernoulli<X->Y>{p}`, i.e., we observe `p*` but we know that it is
a random observation from a conditional distribution where we are conditioning
on the latent function `p` to generate `p*`.

If we observe a realization of `p* ~ bernoulli<X->Y>{p}`, where `p` is latent, and we
know that it is a Bernoulli approximation, then we say that `p*` is of the type
```cpp
X -> bernoulli<Y>
```

Normally the order of the Bernoulli Model is not that important, but it may be, e.g.,
if we are trying to estimate the latent function `p` from `p*`, and we know that
the order is 1, then we can estimate the confusion matrix more easily given an
i.i.d. sample of observations.

A more *interesting* property, that can be read off the confusion matrix, is the
entropy of the distribution `bernoulli<X->Y>`. This is given by
```cpp
H(bernoulli<X->Y>) = -sum_{i=1}^n sum_{j=1}^n qij log(qij)
```
where `qij` is the probability that `pi` is observed as `pj`.

We can also consider the conditional entropy distribution, `bernoulli<X->Y|pi>`,
where `pi` is the latent function. This is given by
```cpp
H(bernoulli<X->Y|pi>) = -sum_{j=1}^n qij log(qij)
```
where `qij` is the probability that `pi` is observed as `pj`.

Often, we apriori *know* the confusion matrix, or at least various properties of this
matrix, as a result of the distortion being the result of some known process, e.g.,
a noisy channel, or a noisy sensor, or a noisy measurement. A noisy channel also
includes things like a program that introduces some loss as a way of compressing
the data, or it may be the result of some homomorphic encryption scheme or a
homomophorism for an oblivious data structure where we represent values as 
the product of trapdoors, one-way hashes.

## Bernoulli Maps

A Bernoulli Map is just a way of generating a Bernoulli approximation of a function.
By the equivalence that data is code and code is data, any function can be represented
as a data structure, and any data structure can be represented as a function.
Therefore, theoretically, we can model any data structure as a map, and then we
can generate a Bernoulli approximation of that map, which means we have a Bernoulli
approximation of the data structure.

Often, we have more efficient and interesting ways to generate particular kinds of
Bernoulli approximations of data structures. Probably, the most popular example
are sets, like Bloom filters.

### Set-indicator function `1_A : X -> bool`

The set-indicator function for `A`, denoted by `1_A`, where `1_A(x)` is `true` if
`x` is in `A`, and `false` otherwise. When we apply a Bernoulli Model to `1_A`,
we get a function of type `X -> bernoulli<bool>`. We observe `bernoulli<X->bool>{1_A}`
but we do not know `A` with certainty. This is a *Bernoulli approximation* of `1_A`,
and common examples of this kind of approximation are *bloom filters* and *counting
bloom filters*. In this project, we introduce the Bernoulli Map, which is an algorithm
that can generate any kind of approximation of computable functions, including
set-indicator functions.

### Primality test: `is_prime : integer -> bernoulli<bool>`

We know how to exactly determine whether an integer is prime. We can, for instance,
check for divisibility by all integers less than the integer. There are many
ways to more efficiently compute this, but the point is that we know how to
compute it exactly.

However, the function is still *latent* in the sense that the time required to
compute it exactly for any input of interest is prohibitive, and so in practice we do
not know its extension. It is still, in this sense, latent.

So, instead, we can use a randomized algorithm to estimate the function and be able
to compute it for any desired input in a reasonable amount of time. 

#### The Miller-Rabin primality test
The Miller-Rabin primality test is based on the concept of Fermat's Little Theorem,
which states that if `p` is a prime number and `a` is any positive integer less than
`p`, then `a^(p-1)` is congruent to 1 modulo `p`.

The Miller-Rabin test works by randomly selecting values of `a` and checking whether
the congruence holds. If the congruence fails for a particular `a`, then `p` is
definitely not prime. However, if the congruence holds for some `a`, then `p` may
or may not be prime but we say that it is prime, which has some specifiable
probability of error (false positive rate).

In essence, a particular seed value (for the PRNG) draws a sample function, a Bernoulli
map, from `is_prime* ~ bernoulli<integer -> bool>{is_prime}`.

## Computational basis

If we have a set of functions `F = { f1, ..., fk }`, then we
can define a Bernoulli model over `F` by simply generating realizations of
`bernoulli{f1}, ... bernoulli{fk}` which we may denote as `bernoulli{F}`.

For instance, it may be desirable to support both `in` and `==` for sets.
One approach is to generate a Bernoulli aproximation for each element in `F`.
However, if we define `==` in terms of `in`, then that *induces* a Bernoulli
Model of `==` through its dependence on `in`.



## Regular types

It is interesting to note that Bernoulli Models are not in general regular types,
since it is often the case that, say, a Bernoulli set `A` can have countably
infinite representations, and it is impossible (in general) to determine if
two Bernoulli sets are the same.

This does not even entertain the discussion about which latent set is being
approximately modeled by a Bernoulli set, which can of course also vary. If we
consider this perspective, then equality on Bernoulli sets vs sets is not of
type
```cpp
(bernoulli<set>, set) -> bool
```
but of type
```cpp
(bernoulli<set>, set) -> bernoulli<bool>
```
and likewise for other variations on this pattern, i.e., we can only say what the
probability that a Bernoulli set represetns a given latent set. This is normally
a much less interesting and informative question than set-membership, but it is
still a question that can be asked.