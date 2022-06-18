Bernoulli set
=============

Consider the Boolean algebra
    `(array<bool,N>, +, *, ~, array<bool,N>{false}, array<bool,N>{true})`
and suppose we define
    `contains : (array<T,N>,int) -> bool`
as
    `bool contains(A,i) { return i >= 0 && i < N && A[i]; }.`

Then, if we replace `bool` with `bernoulli_bool`,
which is understood to be the a Bernoulli type over the equality
predicate for Boolean values, then, for instance,
    `P[bernoulli_bool<K>{x} == bool{x}] = E`.
Thus, we have a Bernoulli approximation of the original Boolean algebra,

```cpp
(array<random_bernoulli<K,bool>,N>,
     |, &, ~,
     array<random_bernoulli<0,bool>,N>{false},
     array<random_bernoulli<0,bool>,N>{true}).
```

Naturally, since contains is defined with respect to equality on Boolean
values, the probability that
    `contains(array<random_bernoulli<K,bool>,N>{A}, i)`
is not equal to
    `contains(array<bool,N>{A}, i)`
for any i in the {0,...,N-1} is given by the distribution of error rates
on the contains. For instance, if we let K = 1, i.e., the first-order random
Bernoulli model with the same error rate E, then the answer is
simply E, i.e.,
    contains : (array<random_bernoulli<1,bool,E>,N>,int) ->
               random_bernoulli<1,bool,E>
where we took the liberty of parameterizing the error rate into the type
signature to make the distribution on the output explicit.

If we let K = 2 with the natural partition into positive and
negative elements such that the positive elements (A) have an error rate
denoted the false negative rate and the negative elements (complement(A))
have error rate denoted the false positive rate.

The *highest* order Bernoulli set over a univese of N elements is K == N,
e.g., an array
    array<random_bernoulli<1,bool>>
where every first-order Bernoulli Boolean of the array has a unique error
rate.

We denote this mathematical structure the Bernoulli set model of order K,
    bernoulli_set<K,X>,
in which X is partitioned into K blocks such that the j-th block has
Bernoulli error Ej over the contains predicate, i.e., if x in block i, then
    P[contains(bernoulli_set<K,X>{A},x) != contains(A,x)] == Ei.

Frequently, the expected error rates E[1], E[2], ..., E[K] are unknown, but
the expected false positive rate and false negative rate may be known or
estimated (either an interval or point estimate). Since we are
frequently not interested in the particulate error rates E[1], ..., E[K],
but instead the false positive and false negative rates, uncertainty over
E[1],...,E[K] may not be particularly important.

Most naturally, the order `K=2` with the natural partition of X into positive
and negative sets. We denote this the negative-positive Bernoulli set.
Algorithms that generate Bernoulli sets, such as the Bloom
filter, generate negative-positive Bernoulli sets with a false negative rate
0 and a false positive rate `0 < E < 1`. We consider this a special case
and denote it the positive Bernoulli set. The complemnt of the positive
Bernoulli set is the negative Bernoulli set.

Given a natural order `K=1` or `K=2`, higher-order Bernoulli sets are then
the result of composing these Bernoulli sets, such as set-interection.
Moreover, as random sets, any function of these random sets is also random,
and in many cases the derived random element is another Bernoulli
approximation, e.g., the subset predicate

```
    subset : (
        array<random_bernoulli<K,bool>,N>,
        array<random_bernoulli<K,bool>,N>) -> random_bernouli<-1,bool>
```
maps to a random Bernoulli of some variable order (it depends upon the
number of )

Conversely, consider
    `random_bernoulli<K,array<bool,N>>`
over the equality predicate

```
    bernoulli<1,bool>
    operator==(
        random_bernoulli<K,array<bool,N>> lhs,
        array<bool,N> rhs).
```

since this would apply to the entire value type of the vector, i.e.,
the error rate would be on the equality predicate `==`. However, the
equality predicate also has an error rate for 
    `array<random_approximate<1,bool>,N>`,
but that is a lot more complicated.

We choose to provide a template specialization for this particular
type.
