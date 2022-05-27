# Bernoulli maps

The bernoulli map quantifies a certain kind of approximation error that is compatible with many algorithms, particularly data types that depend on hashing.

Suppose we have a function $p : X \mapsto Y$.
and a function $p' : X \mapsto Y$ such that $p'(x) \neq p(x)$ with probability $\epsilon(x)$.
We name this concept the Bernoulli approximation model.

- A first-order Bernoulli approximation is given by a constant $\epsilon(x)$, i.e., $\epsilon(x) = \epsilon$ for all $x \in X$.
- A second order approximation partitions $X$ into two sets, say $A$ and $A'$, such that $p'(a) \neq p(a)$ with probability $\epsilon(a) = \epsilon(A)$ for all $a \in A$ and over $p'(b) \neq p(b)$ with probability $\epsilon(b) = \epsilon(B)$ for all $b \in B$.
- More generally, an $n$-th order Bernoulli approximation model partitions $X$ into $n$ sets, $A_1,\ldots,A_n$, such that $p'(a_j) \neq p(a_j)$ with probability $\epsilon(a_j) = \epsilon(A_j)$ for all $a_j \in A_j$ for $j=1,\ldots,n$.

## Set-indicator function $1_A : X \mapsto \{0,1\}$

The set-indicator function for $A$, denoted by $1_A$, is defined as $1_A(x) = 1$ if $x \in A$ and $1_A(x) = 0$ if $x \notin A$.

If $1_A'$ is a first-order Bernoulli approximation, then $\epsilon(x) = \epsilon$ for any $x \in X$.

This is related to the noisy *binary symmetric channel* in the following way.
Suppose we have a set $C$ and a universal set $U = \{x_1,\ldots,x_n\}$. We may model $C$ using $n$ bits, $b_1 \cdots b_n$ where $b_j := 1_C(x_j)$ for $j=1,\ldots,n$. Then, $1_C(x) := b_j$.

If we transmit $b_1 \cdots b_n$ over a noisy binary symmetric channel with error rate $\epsilon$, then each bit may be flipped with probability $\epsilon$. Conceptually, this is a random process that maps $b_1 \cdots b_n$ to $b_1' \cdots b_n'$ where $\Pr\{b_j \neq b_j'\} = \epsilon$.

If we define the characteristic function $1_C'(x_j) = b_j'$, then this models a first-order Bernoulli approximation of $1_C$. In general, the class of sets constructed in this way is the first-order Bernoulli model, which may be denoted by a pair of functions, the function being Bernoulli approximated, and the error rate function. In this case, $(\in, \epsilon)$ where $\epsilon$ is a constant function between $0$ and $1$.

### Induced errors

Observe that other set-theoretic predicates, like subset, may be defined with respect to $\in$, e.g.,
$$
  A \subset B := x \in A \implies x \in B
$$
for all $x \in A$.

If we replace $\in$ with $(\in,\epsilon)$, then $\subset$ is mapped to $(\subset,\epsilon_A)$ where $\epsilon_A = 1-(1-\epsilon)^{|A^c|}$.
Thus, we see that $(\in,\epsilon)$ may _induce_ other Bernoulli approximations.

Note that $\subset$ is of type $2^U \times 2^U \mapsto \{0,1\}$ and $\epsilon_A$ partitions $2^U$ into $n=|U|$ sets where there are $\binom{n}{k}$ sets of size $k$ and $\sum_k \binom{n}{k} = 2^n$.
Therefore, $(\subset,\epsilon_A)$ is an $n$-th order Bernoulli approximation over $2^U$, where the larger the set $A$ the smaller the error rate.
At one extreme, $\epsilon(\emptyset) = 1-(1-\epsilon)^n \approx 1$, and at the other extreme, $\epsilon(U) = 1-(1-\epsilon)^0 = 0$.

Let $p := \lambda D.(C \subset D)$.

## Second-order model

Suppose that for any given set $A \subset U$, we partition $U$ into $A$ and $A^c$ such that for any $x \in A$, the error rate is $\omega$ and for any $x \in A^c$, the error rate is $\epsilon$.
This represents a second-order Bernoulli model, denoted by

$$
  (\in, \epsilon(x))
$$
where $x \in A \implies \epsilon(x) = \omega$ and $x \in A^c \implies \epsilon(x) = \epsilon$.
If $\omega = 0$, then we denote this by a special name, the positive Bernoulli set.
There are many examples in computer science for this type of Bernoulli model, e.g., Bloom filters.

## Extended computational basis
There is nothing to prevent having a Bernoulli model with a computational basis over multiple functions, rather than just one like $\in$.

For instance, it may be desirable to support both $\in$ and $=$, where $\in$ may be a second-order and $=$ may be first-order.
We denote this situation with the notation

$$
  \left(\{\epsilon_{\in}(x,A),\epsilon_{=}(A,B)\}\right)
$$
where $\epsilon_{\in(x,A)} = 



If the computational basis is $\{f_1,\ldots,f_k\}$, where $f_j$ is some function of any type