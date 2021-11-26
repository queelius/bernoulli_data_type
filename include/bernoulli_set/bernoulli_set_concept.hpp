/**
 * The concept of a Bernoulli set.
 */

namespace alex::concepts::set
{
    /**
     * A bernoulli set with elements of type T is a set over values of type T
     * with a natural error rate on the membership predicate that is a function
     * of some partition of T such that elements in the same partition have the
     * same expected error rate.
     *
     * Regardless, we can map these error rates to an error rate on the positive
     * and negative blocks of the partition of T with respect to the set.
     *
     *     using bernoulli_set<O,set_indicator<T>,is_member<T>>
     *     using bernoulli_bool<O,bool,equal_to<T>>
     *
     *     contains : (bernoulli_set<O1>,T) -> bernoulli_bool<O2>
     *
     * The error on the membership predicate, contains, induces errors on other
     * functions and relations that can be defined as a function of the
     * membership predicate, such as subset or union.
     *
     * If the error rate is naturally with respect to some function that is
     * different than the membership predicate, then we do not have a canonical
     * bernoulli set. However, it is worth pointing out that if we take
     * the powerset of a bernoulli set, the error rate is naturally with
     * respect to subsets of the original bernoulli set.
     */

    template <typename T, typename O>
    concept bool bernoulli_set = requires(T x)
    {
        { x.fpr() } -> std::convertible_to<double>
	{ x.fnr() } -> std::convertible_to<double>
	{ x.contains() } -> std::convertible_to<bernoulli<O,bool,std::equal_to>>
    }
}
