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

If we define the characteristic function $1_C'(x_j) = b_j'$, then this models a first-order Bernoulli approximation of $1_C$. In general, the class of sets constructed in this way is the first-order Bernoulli model, which may be denoted by a pair of functions, the function being Bernoulli approximated, and the error rate function. In this case, $(\in, \epsilon)$ where $\epsilon$ is a constant function between 0 and 1.

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

If we fix $A$ in $A \subset B$, e.g., $p := \lambda B.(A \subset B)$,then replacing $\subset$ with $(\subset,\epsilon_A)$ constructs $p'$, which is a f

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














## Inducing different kinds of Bernoulli types

If we have a function `f : bool -> bool`, then the space of all possible functions
is given by Table 1.

Table 1: All possible functions `f : bool -> bool`

| f     | f(true) | f(false) |
|-------|---------|----------|
| id    | true    | false    |
| not   | false   | true     |
| true  | true    | true     |
| false | false   | false    |

It may be interesting to consider what happens when we replace the Boolean inputs
with Bernoulli boolean values and ask the question, "What is the probability that
`f(bernoulli<bool,1>{x}) == f(x)`?" Notice that `bernoulli<bool,1>{x}` is `x`
with some probability, but we don't actually know whether the origional `x` was
`true` or `false`. We only *observe* `bernoulli<bool,1>{x}`, where `x` is
a latent Boolean variable. If we know `p`, then we can compute the probability
that `f(bernoulli<bool,1>{x}) == f(x)`, which will depend on `f` and `x`.
Depending upon which function in this space we choose, we may either get the same function, or a different function.

For the constant fuctions, `true` and `false`, we get the same
function, i.e., `true(bernoulli<bool,1>{true}) == true` since `true : bool -> bool`
always outputs `true`, and similiarly for `false : bool -> bool`.

However, the `id` and `not` functions are different. For instance, suppose
`Pr{bernoulli<bool,1>{x} == x} = p`. Then, when we input `bernoulli<bool,1>{true}`
into `id`, we get the correct output `true` with probability `p` and the incorrect
output `false` with probability `1-p`. Likewise, when we input `bernoulli<bool,1>{false}` into `id`, we get the correct output `false` with probability `p` and
the incorrect output `true` with probability `1-p`.

Since we can think of these outputs as either correct or incorrect with probability
`p`, we can call them Bernoulli Boolean values too,
```cpp
    bernoulli<bool,1> : bool -> bernoulli<bool,1>
```
or even as a noisy `id` function,
```cpp
    bernoulli_id<1> : bool -> bernoulli<bool,1>
```

Notice that when we consider `bernoull_id`, we are not talking about whether the
value (in this case, a function) is Bernoulli over equality, but in this case,
it is Bernoulli over its output's equality. It is a subtle yet important
distinction that will be more important for composite typpes, like `bernoulli_pair`
or `bernoulli_set`, both of which are just special kinds of `bernoulli_map` types.

Let's fix `p` and consider the total space of functions for `id : bool -> bool`.

Table 2: `bernoulli_id<1> : bool -> bernoulli<bool,1>`

| id    | `Pr{bernoulli_id(x)} = x` | `Pr{bernoulli_id(x)} != x` |
|-------|---------------------------|----------------------------|
| true  | p                         | 1-p                        |
| false | p                         | 1-p                        | 

Let's consider `bernoulli_id<2> : bool -> bernoulli<bool,2>`.

Table 3: `bernoulli_id<2> : bool -> bernoulli<bool,2>`

| id    | `Pr{bernoulli_id(x)} = x` | `Pr{bernoulli_id(x)} != x` |
|-------|---------------------------|----------------------------|
| true  | true positive rate        | false negative rate        |
| false | true negative rate        | false positive rate        |



## Binary functions
For completeness, let's consider the set of binary functions
`f : (bool, bool) -> bool`. 

There are 2^2 = 4 possible functions `f : bool -> bool` since for each possible
input, `true` or `false`, we have two possible outputs, `true` or `false`.
More generally, if we have `f : X -> Y`, then we have `|Y|^|X|` possible functions,
where `|.|` denotes the cardinality of a set. For instance, if `X = (bool, bool)`
and `Y = bool`, then we have `2^4 = 16` possible functions, since `|X| = 4`
and `|Y| = 2`.

Each of these functions has a designated name, which we can use to refer to them,
like `and`, `xor`, etc. However, we are just going to look at `and`.

Table 4: `and : (bool, bool) -> bool`

| `x1` | `x2` | `and(x1, x2)` |
|------|------|---------------|
| true | true | true          |
| true | false| false         |
| false| true | false         |
| false| false| false         |

Now, let's consider
```
and : (bernoulli<bool,1>, bernoulli<bool,1>) -> bernoulli_bool<?>`
```

We will go row by row, and examine the probability that the output is correct for
each pair of inputs.

Firt, if `x1 = bernoulli<bool,1>{true}` (the latent variable is `true`, an but apriori
), and `x2 = bernoulli<bool,1>{true}`, then the output is

Table 5: `and : (bernoulli<bool,1>, bernoulli<bool,1>) -> bernoulli_bool<?>`

| `x1` | `x2` | `Pr{and(x1, x2)} == and(x1, x2)` |
|------|------|----------------------------------|
| true | true | p                                |
| true | false| 1-p                              |
| false| true | 1-p                              |
| false| false| 1-p                              |


