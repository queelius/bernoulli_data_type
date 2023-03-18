# The concept of the Bernoulli data type

> NOTE
> Most of the documentation/papers are outdated. Some of the code in the include
> directory is more recent, but I threw a bunch of stuff in other projects
> into this repo to try to organize it. It's also related to other repos,
> like github.com/queelius/rd_ph_filter, but it's still being sorted out.
> I'll try to whip it into shape when i have time.

## Random approximate set concept

We provide two data types that model the concept of the random positive
approximate set, the well-known Bloom filter and another data type that we
call the approximate hash set.

We also provide set-theoretic functions like union, complement, power set,
Cartesian product, and disjoint union.

We offer a mechanism such that any object that models a random approximate set
over elements of type `T` may be type-erased as objects of
type `random_approximate_set<T>` and `set<T>` (if the false positive and
false negative queries are unnecessary).

Finally, we provide a Boolean algebra query interface

`satisfies : (RandomApproximateSet<T>, BooleanQuery<T>) -> {True,False}`,
e.g., `satisfies(s, not((a or b) and c))`, where `s` modes the concept of a
random approximate set over type `T`, `a,b,c` are values of type `T`, and
`not`,`or`, and `and` are Boolean queries, and the entire expression
`not((a or b) and c)` is a Boolean query expression tree.

For more on the random approximate set model, see
https://github.com/queelius/random_approximate_set_model.

## Random approximate map concept

The random approximate set over type `T` has a *characteristic function* that
is a random approximate map over a domain of type `T` and a codomain of type
`Bool`.

Another name for a *map* is a *partial function*. Thus, a random approximate
map is a generative model for partial functions whose *domain of definition* is
uncertain.

General purpose, space and time efficient generative models exist for
representing any partial function over a finite domain of definition.

Since a function, the exponential data type, can model many other data types,
like lists, matrices, and so on, the random approximate map provides a
facility for representing any random approximate data type.

For instance, the random approximate set may be considered a special case of
the random approximate map, i.e., its uncertain characteristic function as
represented by a random approximate map. (Note that we have efficient
data structures and algorithms that directly model the concept of a random
approximate set, rather than using a random approximate map to represent
its characteristic function.)

Another example is an array of elements of type `T` which may be
modeled as a function `f : (N -> maybe T)`, where `f(i)` represents the
may be `i`-th element. If `i` is greater than or equal to the list's size,
`Nothing` is returned.
