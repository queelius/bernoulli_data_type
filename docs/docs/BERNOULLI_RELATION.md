# Random approximate relations

A random approximate relation is a random approximate set with additional
structure between tuples of elements in the set.

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

If $R := A x B$, and we have isomorphisms $f : A -> C$ and $ g : B -> C$
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
