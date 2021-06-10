#pragma once

#include "bernoulli.hpp"

namespace bernoulli
{
    struct bernoulli<first_order, bool, eq>
    {
        using value_type = bool;
        using op = eq;
        using partition = first_order;

        auto error(bool) const { return err; }

        operator bool() const { return value; }

        bool const value;
        double const err;
    };

    bernoulli<first_order, bool, eq>
    operator==(
        bernoulli<first_order, bool, eq> const& x,
        bernoulli<first_order, bool, eq> const& y)
    {
        /*
        false inequality case:

            P[X = 1 && Y = 1 | x = 1 && y = 1]
                = P[X = 1 | x = 1] P[Y = 1 | y = 1]
                = (1-err1) * (1-err2)
            => 1 - (1-err1)(1-err2)

            P[X = 0 && Y = 0 | x = 0 && y = 0]
                = P[X = 0 | x = 0] P[Y = 0 | y = 0]
                = (1-err1) * (1-err2)
            => 1 - (1-err1)(1-err2)

            => fnr := P[X == Y | x != y] = 1 - (1-err1)(1-err2)

        false equality case:

            P[X = 0 && Y = 0 | x = 1 && y = 0]
                = P[X = 0 | x = 1] P[Y = 0 | y = 0]
                = err1(1-err2)

            P[X = 0 && Y = 0 | x = 0 && y = 1]
                = P[X = 0 | x = 0] P[Y = 0 | y = 1]
                = (1-err1)err2

            P[X = 1 && Y = 1 | x = 0 && y = 1]
                = P[X = 1 | x = 0] P[Y = 1 | y = 1]
                = err1(1-err2)

            P[X = 1 && Y = 1 | x = 1 && y = 0]
                = P[X = 1 | x = 1] P[Y = 1 | y = 0]
                = (1-err1)err2

            => fpr := P[X == Y | x != y] = minspan(err1(1-err2),(1-err1)err2)
        */

        return bernoulli<higher_order<pos_neg>, bool, eq>{
            x.value == y.value,
            minspan(x.err*(1-y.err), (1-x.err)*y.err) // fpr
            x.err + y.err - x.err*y.err // fnr
        }
    }

    auto
    fpr(bernoulli<first_order, bool, eq> const& x)
    {
        return x.err;
    }

    auto
    fnr(bernoulli<first_order, bool, eq> const& x)
    {
        return x.err;
    }

    auto
    operator~(bernoulli<first_order, bool, eq> const& x)
    {
        return bernoulli<first_order, bool, eq>{ !x.value, x.err };
    }



    /**
     * Suppose
     *     X' ~ random_bernoulli<1,bool> and
     *     X  ~ random_bernoulli<0,bool>.
     * Then, by the first-order Bernoulli model,
     *     P[X' != a|X == a] == error(a) = error()
     * where error() is the marginalization
     *     error() = sum (p(x[i] * error(x[i])).
     * Typically, we assume p(x[i]) = 1/n where |x| = n, but in the first-order
     * model it does not make a difference.
     * 
     * In the above, we are given an observation of X' with X unknown, but we are
     * typically interested in
     *     alpha := P[X == a|X' == a]
     * which, by the law of probability
     *     alpha = P[X == a,X' == a] / P[X' == a]
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
     *     alpha(a) =        p(a)*(1-error(a))
     *                --------------------------------.
     *                p(a)*(1-error(a))+(1-p)*error(a)
     * 
     * Note: this is just ppv (or npv),
     * 
     *     ppv(x) := p(true) tpr(x)
     *               --------------------------------
     *               p(true) tpr(x) + p(false) fnr(x)
     *                 
     *     npv(x) := p(false) tnr(x)
     *               --------------------------------.
     *               p(false) tnr(x) + p(true) fpr(x)
     * 
     */

    template <typename P, typename T, typename Op>
    auto pv(bernoulli<P,T,Op> const & x, double p = 0.5)
    {
        return p*(1-x.error(x.value)) /
            (p*(1-x.error(x.value) + (1-p)*x.error(x.value)));
    }
}