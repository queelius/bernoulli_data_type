#pragma once

#include "bernoulli.hpp"

namespace bernoulli
{
    /**
     * Predictive value (pv)
     * ---------------------
     * Suppose
     *     X' ~ random_bernoulli<P,T,F> and
     *     X  ~ random_bernoulli<P,T,F>.
     * Then, by the first-order Bernoulli model,
     *     P[X' != a|X == a] == error(a) = error()
     * where error() is the marginalization
     *     error() = sum (p(x[i] * error(x[i])).
     * Typically, we assume p(x[i]) = 1/n where |x| = n, but in the first-order
     * model it does not make a difference.
     * 
     * In the above, we are given an observation of X' with X unknown, but we are
     * typically interested in
     *     pv(a) := P[X == a|X' == a]
     * which, by the law of probability
     *     pv(a) = P[X == a,X' == a] / P[X' == a]
     *           = P[X' == a|X == a] P[X == a] / P[X' == a]
     *           = (1-error(a)) P[X == a] / P[X' == a].
     * 
     * By the total law of probability,
     *     P[X' == a] = P[X' == a,X == a] + P[X' == a,X != a]
     *                = P[X' == a|X == a]P[X == a] + P[X' == a|X != a]P[X != a]
     *                = (1-error(a))P[X == a] + error(a)(1-P[X == a]).
     * 
     * If we denote P[X == a] by p(a) and substitute P[X' == a] with the above,
     * we get the result
     *     pv(a) =          p(a)*(1-error(a))
     *                --------------------------------.
     *                p(a)*(1-error(a))+(1-p(a))*error(a)
     */

    template <
        typename P, // models the concept of a partition of T
        typename T, // object type
        typename F> // function, or family of functions, which naturally has
                    // the bernoulli error.
    auto pv(bernoulli<P,T,F> const & x, double p = 0.5)
    {
        return p*(1-x.error(x.value)) /
            (p*(1-x.error(x.value) + (1-p)*x.error(x.value)));
    }
}