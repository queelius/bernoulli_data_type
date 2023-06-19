Bernoulli set
=============

Let `F = array<bool,N>` be the set of all arrays of `N` Boolean values.
Consider the Boolean algebra
```
(F, +, *, ~, F{false}, F{true})
```
where `false` denotes the array of `N` `false` values and `F{true}` denotes
the array of `N` `true` values. The `+` and `*` operators are defined as
element-wise `or` and `and` respectively. The `~` operator is defined as
element-wise `not`.

Suppose the `contains` predicate function
```cpp
contains : (F,int) -> bool
```
is defined as
```cpp
auto contains(auto A, auto i) { return i >= 0 && i < N && A[i]; }
```
Likewise, suppose the equality operator
```cpp
== : (F, F) -> bool
```
is defined as
```cpp
auto operator==(auto lhs, auto rhs) {
    std::all_of(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
        [](auto x, auto y) { return x == y; });
}
```

Consider a Bernoulli Boolean value. Since there are only two possible values,
`{true, false}`, we have the follow situations to consider:

0. `Pr{bernoulli_bool<0>{x} == x} = 1` where `x` is either `true` or `false`.
This is a zeroth-order Bernoulli model, i.e., `bernoulli_bool<0>` is equivalent to `bool`.
This is like a *perfect channel* where no noise is introduced.

1. `Pr{bernoulli_bool<1>{x} == x} = p`, `0 < p < 1`. This is a first-order Bernoulli
model where the probability of an equality error is the same for both `true` and
`false`. This is like a *binary symmetric channel* where noise is introduced
independently of the input.

2. `Pr{bernoulli_bool{true} == true} = p1` and
   `Pr{bernoulli_bool{false} == false} = p2` where `p1 != p2`. This is a second-order
Bernoulli model where the probability of an equality error is different for `true`
and `false`. This is like a *binary asymmetric channel* where noise is introduced
that is dependent on the input.

For Boolean values, there are no more cases to consider. However, for other
types, such as integers, there are theoretically as many cases to consider as
there are possible values. If we extend that to composite types, such as arrays of
size `N`, then there are `|T|^N` cases to consider where `T` is the type of the
elements of the array. For instance, if `T = bool`, then there are `2^N` cases to
consider. If `T = char`, then there are `2^8^N = 256^N` cases to consider, and if
`T = array<X,N>`, then there are `|X|^N^N` cases to consider.

In our earlier example, we had `T = bool` and `N = 1`, so there were only two
cases to consider (discarding the zeroth-order case).

So, consider replacing `F = array<bool,N>` with `F* = array<bernoulli_bool<1>>`.
We have the following cases to consider:

1. `Pr{F*{x} == x} = p`, `0 < p < 1`. This is a first-order Bernoulli model
where the probability of an equality error is the same for both `true` and


replace `bool` with `bernoulli_bool<1>` and consider 
If we replace `bool` with `bernoulli_bool`,
which is understood to be the a Bernoulli type over the equality
predicate for Boolean values, then, for instance,
`Pr[bernoulli_bool<K>{i} == bool{i}] = e(i)` where `e` is some function
that maps to the probability of an equality error occuring on `x`. 
Thus, we have a Bernoulli approximation of the original Boolean algebra.
Let `F* = array<bernoulli_bool<K>,N>` be the set of all arrays of `N` Bernoulli
Boolean values. Then, we have the following approximation of the original
Boolean algebra:


```
(F*, |, &, ~, F*{false}, F*{true})
```

with the requirement that:

1. `F*{false} = F{false}` and `F*{true} = F{true}` is true with probability 1,
which means that we treat the array of `N` the error rates on the equality predicate
are the same for all elements of the array. We denote this the first-order


Naturally, since contains is defined with respect to equality on Boolean
values, the probability that
    `contains(array<random_bernoulli<K,bool>,N>{A}, i)`
is not equal to
    `contains(array<bool,N>{A}, i)`
for any i in the {0,...,N-1} is given by the distribution of error rates
on the contains. For instance, if we let K = 1, i.e., the first-order random
Bernoulli model with the same error rate E, then the answer is
simply `E`, i.e.,
```cpp
    contains : (array<random_bernoulli<1,bool,E>,N>,int) ->
               random_bernoulli<1,bool,E>
```
where we took the liberty of parameterizing the error rate into the type
signature to make the distribution on the output explicit.

If we let $K=2$ with the natural partition into positive and
negative elements such that the positive elements (A) have an error rate
denoted the false negative rate and the negative elements (complement(A))
have error rate denoted the false positive rate.

The *highest* order Bernoulli set over a univese of $N$ elements is $K == N$,
e.g., an array
    `array<random_bernoulli<1,bool>>`
where every first-order Bernoulli Boolean of the array has a unique error
rate.

We denote this mathematical structure the Bernoulli set model of order K,
    `bernoulli_set<K,X>`,
in which X is partitioned into K blocks such that the j-th block has
Bernoulli error Ej over the contains predicate, i.e., if x in block i, then
    `P[contains(bernoulli_set<K,X>{A},x) != contains(A,x)] == E[i]`.

Frequently, the expected error rates `E[1], E[2], ..., E[K]` are unknown, but
the expected false positive rate and false negative rate may be known or
estimated (either an interval or point estimate). Since we are
frequently not interested in the particulate error rates `E[1], ..., E[K]`,
but instead the false positive and false negative rates, uncertainty over
`E[1],...,E[K]` may not be particularly important.

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
approximation, e.g., the subset predicate:

~~~ cpp
template <typename K, size_t N>
auto subset : (
    array<random_bernoulli<K,bool>,N>,
    array<random_bernoulli<K,bool>,N>)
~~~

maps to a random Bernoulli of some variable order (it depends upon the
number of )

Conversely, consider
    `random_bernoulli<K,array<bool,N>>`
over the equality predicate

```cpp
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
