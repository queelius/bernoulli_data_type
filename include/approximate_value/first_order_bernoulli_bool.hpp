#pragma once

#include "bernoulli_bool.hpp"
#include <random>


/**
 * A realization of a value sampled from a first-order random Bernoulli value
 * of type Boolean.
 */
struct bernoulli<1,bool>
{
    using value_type = bool;
    static auto order() const { return 1; }

    bool value;
    double err;

    //auto operator==(bool x) const { return bernoulli<1,bool>{value==x,err}; }
    //auto operator!=(bool x) const { return bernoulli<1,bool>{value!=x,err}; }

    double fpr() const { return err; }
    double fnr() const { return err; }

    /**
     * Suppose
     *     X ~ random_bernoulli<1,bool> and
     *     Y ~ random_bernoulli<0,bool>.
     * Then, by the first-order Bernoulli model,
     *     P[X != a|Y == a] == error(X). 
     * 
     * In the above, we are given an observation of X with Y unknown, but we are
     * typically interested in
     *     alpha := P[Y == a|X == a]
     * which, by the law of probability
     *     alpha = P[Y == a,X == a] / P[X == a]
     *           = P[X == a|Y == a] P[Y == a] / P[X == a]
     *           = (1-error(X)) P[Y == a] / P[X == a].
     * 
     * By the total law of probability,
     *     P[X == a] = P[X == a,Y == a] + P[X == a,Y != a]
     *               = P[X == a|Y == a]P[Y == a] + P[X == a|Y != a]P[Y != a]
     *               = (1-error(X))P[Y == a] + error(X)(1-P[Y == a]).
     * 
     * If we denote P[Y == a] by p(a) and substitute P[X == a] with the above,
     * we get the result
     *     alpha(p) =        p(1-error(X))
     *                ---------------------------.
     *                p(1-error(X))+(1-p)error(X)
     * 
     * Note: this is just ppv (or npv),
     * 
     *     ppv(x) := p(true) tpr(x)
     *               --------------------------------
     *               p(true) tpr(x) + p(false) fnr(x)
     *                 
     *     npv(x) := p(false) tnr(x)
     *               --------------------------------
     *               p(false) tnr(x) + p(true) fpr(x)
     * 
     */
    auto error_rate(double p = .5)
    {
        // note: error_rate(.5) = err
        return (1-p)*err / (p*(1-err) + (1-p)*err);
    }

    bool operator() const { return value; }
    auto operator()() const { return value; }
};









/**
 * A random_bernoulli_bool<1,bool> models the concept of a random variable that
 * may be sampled from. Speifically, it models the concept of the first-order
 * bernoulli Boolean in which, given an objective Boolean x, the first-order
 * bernoulli Boolean realizes x with probability
 *     1-error(x)
 * and !x with probability
 *     error(x).
 */
template <>
struct random_bernoulli<1,bool>
{
    using value_type = bool;    
    static int order() { return 1; }

    double err;
    bool given;

    auto fpr() const { return err; }
    auto fnr() const { return err; }
    auto error() const { return err; }

    auto condition() const { return given; }

    // u models the concept of a uniform random bit generator.
    auto operator()(uniform_random_bit_generator auto & u)
    {
        bernoulli_distribution ber(err);
        auto x = (int)ber(u);
        if (given)
            return bernoulli<1,bool>{err, (bool)(1-x)};
        else
            return bernoulli<1,bool>{err, (bool)x};
    }
};

