/**
 * The concept of a Bernoulli set.
 */

namespace bernoulli::set
{
    /**
     * A Bernoulli set with elements of type `T` is a set over values of type
     * `T` with a natural error rate on the membership predicate that is a
     * function of some partition of `T` such that elements in the same
     * partition have the same expected error rate.
     *
     * Regardless, we can map these error rates to an error rate on the positive
     * and negative blocks of the partition of `T` with respect to the set,
     * which are respectively the false positive and false negative rates.
     *
     * Generally, this Bernoulli type is denoted by
     *     `bernoulli<N, set<T>, is_member<T,set<T>>>`,
     * which indicates that it is an N-th order Bernoulli type that models
     * the concept of a set, where the Bernoulli error rates are with respect to
     * the `is_member` function.
     *
     * As a shorthand, we denote this Bernoulli type by `bernoulli_set<N,T>`.
     * Thus,
     * ```
     *     is_member : (T, bernoulli_set<T,N>) -> bernoulli<bool,2>
     * ```
     *
     * What is `bernoulli<bool,2>`? It is a Bernoulli type over `bool` of
     * order 2. Note that this is the highest order possible on `bool`,
     * since the partition `bool` that has the most blocks is 2,
     * i.e., `{ {true}, {false} }`.
     *
     * However, note that for each `t in T`, `is_member(t,.)`,
     * may map to a value of type `bernoulli<bool,2>` that has different
     * false positive and false negative rates.
     *
     * Often, the Bernoulli set may not know the false positive or false
     * negative rate for a praticular `t in T`, so we allow the error rates
     * to be intervals. Say we have a Bernoulli set `s` and we are interested
     * in whether `t` is a member of `s`. Then,
     *     `is_member(t,s)`
     * maps to a Bernoulli Boolean for which `fpr(is_member(t,s))` and
     * `fnr(is_member(t,s))` map to are appropriately defined.
     *
     * If the underlying implementation of the Bernoulli set does not precisely
     * what the false positive (false negative) rate is for `is_member(t,s)`,
     * then we allow the the false positive (false negative) rate to be
     * an *interval*.
     *
     * If *nothing* is known about the false positive (false negative) rate,
     * then `fpr(is_member(t,s))` is an interval `[0,1]`.
     *
     * The error on the membership predicate, `is_member`, induces Bernoulli
     * errors on functions of `is_member`, such as the subset predicate or union
     * operation.
     *
     * For example, if we take the powerset of a Bernoulli set, the error rate
     * is with respect to subsets of the original Bernoulli set. This is not
     * a canoncial example of `bernoulli_set<T,N>`, but rather models the
     * concept `bernoulli<N, set<T>, is_subset<set<T>,set<T>>>`.
     */

    template <typename T, typename N>
    concept bool bernoulli_set = requires(T x)
    {
        { x.fpr() } -> std::convertible_to<double>
	    { x.fnr() } -> std::convertible_to<double>
	    { x.contains() } -> bernoulli<N,bool>
    }
}
