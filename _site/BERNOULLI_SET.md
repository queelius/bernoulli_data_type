Bernoulli set
=============

Let `F = array<bool,N>` be the set of all arrays of `N` Boolean values.
Consider the Boolean algebra
```cpp
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
`{true, false}`, we have the follow situations to consider. First, let `xj` 
indicate the `j`th element of the array `x` and let `Pr{E}` denote the
probability of the event `E`.

0. `Pr{bernoulli<bool>{xj} == xj} = 1` where `xj` is either `true` or `false`.
This is a zeroth-order Bernoulli model, i.e., `bernoulli<bool,0>` is equivalent to `bool`
and an array of them is equivalent to an array of `bool`.
This is like a *perfect channel* where no noise is introduced.

1. `Pr{bernoulli<bool>{xj} == xj} = p` for all `j` and `0 < p < 1`. This is a
first-order Bernoulli model. This is equialent to a *binary symmetric channel* where
noise is i.i.d. and independent of the input. We denote each of these as
`bernoulli<bool,1>` and an array of them as `array<bernoulli<bool,1>,N>`. Note that
this is equivalent to `bernoulli<array<bool,N>>` and there is technically only
one parameter, `p`, in the confusion matrix that needs to be estimated.

2. `Pr{bernoulli<bool>{xj} == xj|xj == TRUE} = p1` and
   `Pr{bernoulli<bool>{xj} == xj|xj == FALSE} = p2` for all `j` and `p1 != p2` and
   both are non-zero and also not equal to one. This is a second-order
Bernoulli model where the probability of error is different for `true`
and `false` but the same for all indexes. This is like a *binary asymmetric channel*
where noise is introduced that is dependent on the input.

3. `Pr{bernoulli<bool>{xj} == xj|xj == TRUE} = p(xj|TRUE)` and
   `Pr{bernoulli<bool>{xj} == xj|xj == FALSE} = p(xj|FALSE)`. This is a
higher-order Bernoulli model where the probability of error may be different for
each index and for each value. The maximum order of this model is
`2^N(2^N-1)`.

Regardless, when we have these Bernoulli Booleans, this induces a Bernoulli Model
of the Boolean algebra.
We denote this mathematical structure the Bernoulli set model, `bernoulli<set<X>>`.

A natural way to construct a Bernoulli set is to allow false positives on
non-members but no false negatives, such as in the Bloom filter. However, there
is also an opportunity to allow for false negatives by introducing a rate distrotion
either due to time complexity constraints or space constraints. We consider a
Bernoulli set model, named `rd_ph_filter` and `rd_hash_set`, that allows for
false positive and false negatives, and also an entropy coder in which the
probability of a membership test is implicitly represented by prefix-free codes
such that the probability of an error varies for each element being tested.
The end result is a very compact data structure that, for the given compression
rate, has the lowest probability of error. (Expected loss is minimized.)

Even a Bernoulli set that, say, only allows for false positives, can *generate*
a Bernoulli set that allows for false negatives. For example, consider the
Bloom filter. The Bloom filter is a Bernoulli set that allows for false positives
but no false negatives. However, if we take the complement of the Bloom filter,
we get a Bernoulli set that allows for false negatives but no false positives.
In general, set-theoretic operations on Bernoulli sets can generate Bernoulli sets
of a different or higher order.
