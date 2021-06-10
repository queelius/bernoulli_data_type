# Random approximate alebras

A random approximate algebra is a random approximate map with additional
structure.

A groupoid is...

A monoid

$$
    (X,+,1)
$$

is one of the simplest and useful algebraic structures.
It defines a binary operation

$$
    + : X \mapsto X \mapsto X
$$

where $+$ is associative and there exists an element $e$ such that
$a + e = a$ for any $a in X$.

If the random approximate map is *rate-distorted, for instance, then
the identity $e$ cannot be guaranteed. However, if there is a total
order on $X$, then $x (+) y$ can be defined as

$$
x + y :=
\begin{cases}
    x + y & x \leq y\\
    y + x & y < x.
\end{cases}
$$

if $x < y$, $y + x$ can be converted to $x + y$
such that the commutative law (Abelian) is in the computational basis
of random approximate maps (even if the values it maps have some non-zero loss). Alterantively, to try to minimize loss, if there is some function

$$
    avg : X \mapsto X \mapsto X
$$

then $x (+) y$ can be defined as

$$
    x (+) y := avg(x + y, y + x),
$$

which may reduce the expected loss on $(+)$ with respect to $+$,
assuming that the random approximate commutative map $+$ attempted to map both $x$ to $y$ and $y$ to $x$. Of course, if the random approximate commutative map only attempted to map $x$ to $y$, $x < y$, with the understanding that it is Abelian and will convert $y + x$ to $x + y$, then this latter approach is expected to have more loss.

However, the associative law cannot be guaranteed. For instance, given a set $X = \{x_1,\ldots,x_n$ and a total order $x_1 < x_2 \ldots < x_n$. Then, $x_2 + x_3 + x_4 = x_2 + (x_3 + x_4)$ which may be computed as following:
    1. Let $a = x_3 + x_4$.
    2. If $a < x_2$, then the result is $a + x_2$ else $x_2 + a$.

Alternatively, by the associative law, $x_2 + x_3 + x_4 = (x_2 + x_3) + x_4$, which may be computed as following:
    1. Let $b = x_2 + x_3$.
    2. If $b < x_4$, then the result is $b + x_4$ else $x_4 + b$.

In the above, the first one neither computes $x_2 + x_3$ nor $the second
one never computes $x_3 + x_4$, a







We say that two tuples $x$ and $y$ are *related* by $R$ if $(x,y) \in R$. Relations have many properties, like symmetry,
reflectivity, transitivity, and other properties like being functional
over some projection.

We may re-use a concrete model of random approximate sets to construct
models of random approximate relations. Many types or properties of
relations are representable by random approximate relations,
like symmetry and reflexivity, but many properties are not (efficiently)
representable, like transitivity. We see that the random approximate relation model has
a *computational basis* that is a subset of the relational model.

For instance, random approximate binary symmetric relations are trivially
representable by taking the *symmetric closure* of random approximate
binary relations. For instance, given a binary relation $R := A x A$ and
a total order $<= : A x A$, the symmetric closure of a binary relation
$R := A x A$ may be obtained *ordering* $x,y in A$ such that, if $x < y$,
$y R x$ is converted to $x R y$ (and $x R y$ is unchanged).

If $R := A x B$, and we have isomorphisms $f : A \mapsto C$ and $ g : B \mapsto C$
with $<= : C x C$, then if $f x < f y$, $y R x$ is converted into $x R y$.
NOPE...

As approximate sets, approximate relations have the usual set-theoretic
operations like complements and unions. They have additional operations,
however, like selection, projection, and joins which have a different
sorts of approximation errors.

In additional to a Boolean algebra, relational properties
provide additional query structure, like $a$ is between $b$ and $c$
if $(a,b,c) \in B$ or $a$ is adjacent to $b$ if $a A b$. Other types
of queries, with their own source of approximation error, may
be constructed from these, e.g., we may approximate the relation
$x_1 x_2 \ldots x_n \in S$, where $S$ represents 'is a sequence',
with the adacent relation $A$ as follows: $x_1 x_2 \ldots x_n$ is
a sequence in $S$ if $x_1 A x_2$ and $x_2 A x_3$ and $\ldots$ and
$x_{n-1} A x_n$.

For more on the random approximate set model, see
https://github.com/queelius/random_approximate_relation_model.






















template <int N, typename T>
struct retrn {};

template <typename T>
struct retrn<0>
{
    bernoulli<0,T> operator()(T x) const
    {
        return bernoulli<0,T>{x};
    }
};

template <typename T>
struct retrn<1>
{
    bernoulli<1,T> operator()(T x, double err) const
    {
        return bernoulli<1,T>{rate_interval<E>{err},x};
    }

    template <typename E>
    bernoulli<1,T> operator()(T x, rate_interval<E> err) const
    {
        return bernoulli<1,T>{err,x};
    }
};


// a Beroulli of a Bernoulli ... of a Bernoulli is a Bernoulli.
template <int N, typename T>
bernoulli<N,T> join(bernoulli<N,bernoulli<N,T>> x)
{
    return join(x.value);
}

template <int N, typename T>
bernoulli<N,T> join(bernoulli<N,T> x)
{
    return x;
}
