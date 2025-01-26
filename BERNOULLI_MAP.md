# Bernoulli Maps

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

Let us show the confusion matrix for `p1, p2, p3, p4` (`n=4`, e.g., `bool -> bool`), where the rows represent the latent functions, and the columns represent the observed functions that may be generated from the latent function, say by introducing some error or rate distortion.

latent \ observed | `p1`  | `p2`  | `p3`  | `p4` |
------------------|-------|-------|-------|------|
`p1`              | `q11` | `q12` | `q13` | `q14`|
`p2`              | `q21` | `q22` | `q23` | `q24`|
`p3`              | `q31` | `q32` | `q33` | `q34`|
`p4`              | `q41` | `q42` | `q43` | `q44`|

In the matrix above, `qij` represents the probability that function `pi` is observed as function `pj`. Since each row must sum to 1, there are at maximum of `n * (n-1)` degrees of freedom, which in this case is `4 * (4-1) = 12`.

We call this degree-of-freedom the *order* of the Bernoulli Model for `X -> Y`.
In many cases, the order is 1, e.g., where most of the probability is assigned to the diagonal, and the off-diagonal elements are nearly zero (or zero) but all equal.

Since we are only given `p*`, we do not know which `p` is the true latent function. We say that `p*` is a *Bernoulli approximation* of `p`, and we deote this by writing `p* ~ bernoulli<X->Y>{p}`, i.e., we observe `p*` but we know that it is a random observation from a conditional distribution where we are conditioning on the latent function `p` to generate `p*`.

If we observe a realization of `p* ~ bernoulli<X->Y>{p}`, where `p` is latent, and we know that it is a Bernoulli approximation, then we say that `p*` is of the type
```cpp
X -> bernoulli<Y>
```

Normally the order of the Bernoulli Model is not that important, but it may be, e.g., if we are trying to estimate the latent function `p` from `p*`, and we know that the order is 1, then we can estimate the confusion matrix more easily given an i.i.d. sample of observations.

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

Often, we apriori *know* the confusion matrix, or at least various properties of this matrix, as a result of the distortion being the result of some known process, e.g., a noisy channel, or a noisy sensor, or a noisy measurement. A noisy channel also includes things like a program that introduces some loss as a way of compressing the data, or it may be the result of some homomorphic encryption scheme or a homomophorism for an oblivious data structure where we represent values as the product of trapdoors, one-way hashes.

## Examples of Bernoulli Maps in Practice

In what follows, we discuss some examples of Bernoulli Maps (Bernoulli Models of functions) that naturally arise in practice. We will see that many algorithms can be understood as a Bernoulli Model of some latent function, and that the Bernoulli Model is a very general model of computation.

### Set-Indicator Function

The set-indicator function for $A$, denoted by $1_A : X \mapsto \mathrm{bool}$, is given by
$$
1_A(x) = \begin{cases}
\mathrm{true} & \text{if } x \in A, \\
\mathrm{false} & \text{otherwise}.
\end{cases}
$$
When we apply a Bernoulli Model to $1_A$, we get a function of type $X \mapsto \mathrm{bernoulli}_{\mathrm{bool}}$. We observe $\mathrm{bernoulli}_{X \mapsto \mathrm{bool}}{1_A}$, but we do not know $1_A$ with certainty.

This is a *Bernoulli Model* approximation of $1_A$, and common examples of this kind of approximation are *bloom filters* and *counting bloom filters*. In another project, we introduce algorithms for constructing Bernolli Models of any function with a finite domain that includes set-indicator functions as a special case.

### Primality Test

We know how to precisely determine whether a number is prime, so we can write the function
$$
\operatorname{is\_prime} : \mathbb{N} \mapsto \mathrm{bool}.
$$
However, the function is still *latent* in the following sense: the time required to compute it exactly for some inputs is prohibitive. Thus, in practice, we do not *know* the value of the function applied to those inputs.

Instead, we can use a randomized algorithm to estimate the function and be able to compute it for any desired input in a reasonable amount of time, but with some probability of error. This is the Miller-Rabin primality test, which is a Bernoulli approximation of the primality test $\operatorname{is\_prime}$.

#### The Miller-Rabin Primality Test

The Miller-Rabin primality test is based on the concept of Fermat's Little Theorem, which states that if $p$ is a prime number and $a$ is any positive integer less than $p$, then $a^{p-1}$ is congruent to $1 \mod p$.

The Miller-Rabin test works by randomly selecting values of $a$ and checking whether the congruence holds. If the congruence fails for a particular $a$, then $p$ is definitely not prime. However, if the congruence holds for some $a$, then $p$ may or may not be prime. The probability that $p$ is not prime is given by the error rate of the test, which can be made arbitrarily small by increasing the number of random values of $a$ that are checked.

The probability that a composite number will pass the test is at most $1/4$ for each value of $a$, so the probability that a composite number will pass the test for $n$ random values of $a$ is at most $1/4^k$. Thus, if $p$ is a composite number, it will falsely test as prime with probability at most $1/4^k$:
$$
\varepsilon = \Pr\bigl\{\widehat{\operatorname{is\_prime}}(p) = \mathrm{true} \mid \operatorname{is\_prime}(p) = \mathrm{false}\bigr\} \leq \frac{1}{4^k},
$$
where $k$ is the number of random (or pseudo-random) values of $a$ that are checked and $\widehat{\operatorname{is\_prime}}$ is the Miller-Rabin approximation of $\operatorname{is\_prime}$.

This is just the definition of the Bernoulli Model applied to functions of this type, where the latent function is $\operatorname{is\_prime}$ and the observed function is $\widehat{\operatorname{is\_prime}}$. We can write this as
$$
\widehat{\operatorname{is\_prime}} \sim \mathrm{bernoulli}_{\mathbb{N} \mapsto \mathrm{bool}}\{\operatorname{is\_prime}\}.
$$

There are an infinite $(2^{|\mathbb{N}|}$) number of possible functions of type $\mathbb{N} \mapsto \mathrm{bool}$, and in theory the Miller-Rabin test can sample *any* of them. In practice, due to space and time constraints, the algorithm only samples from a finite subset.

There is a slight nuance in this construction. The Miller-Rabin test is a randomized algorithm, and so the output can vary even for the same input. We can still think of it as a Bernoulli Model of the latent function $\operatorname{is\_prime}$ (particularly if we cache the results of the test for each input).

### Function Equality

Suppose we have functions $f : X \mapsto Y$ and $g : X \mapsto Y$. We see they are of the same type, but are they **identical**?

Two functions are identical if
$$
\forall_{x \in X} f(x) = g(x).
$$
The extension of a function, $f := \{ (1, 1), (2,4), (3,9), \ldots \}$ is a functional relation, a set of tuples that relate each input to its output, that unambiguously defines the function. It is a complete description of the function, but it is often impractical to work with, both computationally and conceptually, particularly if the domain $X$ is infinite.

We usuall have shorter, finite descriptions, such as *programs*, which are more convenient to work with. For example, in the earlier example, we see that the pattern is $f : \mathbb{N} \mapsto \mathbb{N}$ with a simple definition given by $f(x) := x^2$.

Sometimes, when we have these kind of intensional descriptions of two functions, $f$ and $g$, we may be able to rewrite them to some normal form and recognize they are identical. However, there are an infinite
number of ways to describe most functions, and most of them are impractical to rewrite to some normal form. If you could always do this, then you could solve the *halting problem*.

#### Bernoulli Model of Function Equality

Alternatively, we could go back to the origional definition, its extension. Of course, the extension is often very large or infinite (and thus hard or impossible to compute as a general rule). In these cases, one technique is to sample inputs compatible with the two functions, apply the functions to sampled inputs, and compare their results:
$$
f \hat= g := \prod_{i=1}^n 1_{f(x_i) = g(x_i)},
$$
where $x_i \sim \mathrm{Uniform}(X)$.

If they differ on any inputs, then with certrainty $f \neq g$, and consitent with that, $f \hat= g$ maps to $0$ ($\mathrm{false}$). However, if $f \hat= g$ maps to $1$ ($\mathrm{true}$), then the sample is compatible with the hypothesis that they are equal. [^1]

[^1]: We can frame this as a hypothesis test, where the null hypothesis is that $f = g$ and the alternative hypothesis is that $f \neq g$. However, we will refrain from that terminology here.

Inuitively, the larger the sample size $n$, the greaer the probability they are equal, i.e.,
$$
\lim_{n \to \infty} \Pr\bigl\{f = g \mid f \hat= g\bigr\} \to 1.
$$

However, for finite $n$, we cannot say with certainty. So, we see that this algorithm,
$$
= : (X \mapsto Y) \times (X \mapsto Y) \mapsto \mathrm{bool}
$$
is *latent*. We know the name, or the concept, of the latent function we are approximating (equality), but for any $f$ and $g$, we do not know with certainty what its outputs should be.

We can also see that this is a Bernoulli Model, and we can write
$$
    \hat= \sim \mathrm{bernoulli}\bigl((X \mapsto Y) \times (X \mapsto Y) \mapsto \mathrm{bool}\bigr) \{=\}.
$$

We cannot compute a precise false positive rate, but we can compute its upper and lower bounds.

Suppose $f \neq g$ and we generate a sample of size $n$, $x_1, x_2, \ldots, x_n \sim \mathrm{Uniform}(X)$, and we observe that $f(x_i) = g(x_i)$ for all $i$.

Suppose there are $k$ values in $X$ that demonstrate $f \neq g$. When we take a sample of size $n$ with replacement, the probability that none of those $k$ values are in the sample is
$$
\varepsilon = \left(1 - \frac{k}{|X|}\right)^n.
$$

We do not know $k$, but we know that if $f \neq g$, then at least one value in $X$ will demonstrate that. So, we can say that the false positive rate $\varepsilon$ has a lower bound given by
$$
\varepsilon_L = \left(1 - \frac{1}{|X|}\right)^n,
$$
which for small $n$ and large $|X|$ may be written as
$$
\varepsilon_L \approx e^{-n/|X|}.
$$
The upper bound is given by
$$
\varepsilon_U = \left(1 - \frac{|X|-n}{|X|}\right)^n = \left(\frac{n}{|X|}\right)^n,
$$
and thus we may write
$$
\varepsilon \in [\varepsilon_L, \varepsilon_U].
$$

## Software Bugs and Noisy Channels

If we take a step back and look at the discipline of software engineering,we get a sense that almost all software is a Bernoulli approximation of some latent function. This is because software is written by humans, and humans are not perfect, and so the software is not perfect. The software is a Bernoulli approximation of the latent function that the software is supposed to compute. We cannot normally quantify, and it may in reality violate the assumptions of the Bernoulli model, but it may be a resonable approximation, and there is a case to be made that almost all software programs should be understood as Bernoulli approximations of some latent function. Indeed, even the hardware the software is ran on is a Bernoulli approximation of some latent function, and so we can see that the Bernoulli model is a very general model of computation.

We also have *noise* that can flip a bit, which is largely independent of software bugs. The noisy channel model is a Bernoulli model is a clear example of this process, but in a sense, everything is a noisy channel, and a big advantage of digital computation is that it can be made arbitrarily robust to noise, but it is never perfect.

## Appendix

### Inducing  Bernoulli Models

If we have a set of functions $F = \{ f_1, \ldots, f_k \}$, then we can define a Bernoulli model over $F$ by simply generating realizations of $\mathrm{bernoulli}\{f1\},\ldots,\mathrm{bernoulli}\{f_k\}$ which we may denote as $\mathrm{bernoulli}\{F\}$.

For instance, it may be desirable to support both $\in$ and $=$ for sets. One approach is to generate a Bernoulli aproximation for each element in $F$. However, if we define $=$ in terms of $\in$, then that *induces* a Bernoulli Model of $=$ through its dependence on $\in$.

### Regular types

Bernoulli Models are not in general regular types, since it is often the case that, say, a Bernoulli set $A$ can have countably
infinite representations, and it is impossible (in general) to determine if
two Bernoulli sets are the same.
