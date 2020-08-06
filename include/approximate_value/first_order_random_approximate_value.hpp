#include "random_approximate_value.hpp"

/**
 * The first-order random approximate Boolean is a straightforward monad.
 * 
 * This is the simplest type. There are conceptually two simpler types,
 *     template <size_t N, typename T> // T models Singleton
 *     random_approximate
 * and
 *     template <size_t N>
 *     random_approximate<N,void>,
 * where void denotes the absurd type which has no values, are equivalent to
 * T and void, or alternatively, random_approximate<0,void>
 */
template <>
struct random_approximate<1,bool>
{
    using value_type = bool;
    constexpr int order = 1;

    double epsilon;
    bool given;

    auto fpr() const { return epsilon; }
    auto fnr() const { return epsilon; }
    auto error() const { return epsilon; }

    auto operator()() const { return given; }

    template <typename R>
    auto sample(R & r)
    {
        std::bernoulli_distribution ber(epsilon);
        if (given)
            return approximate<1,bool>{epsilon, 1-ber(r)};
        else
            return approximate<1,bool>{epsilon, ber(r)};
    }
};

/**
 * A realization of a value sampled from the random_approximate<1,bool>
 * random approximate value model.
 */
struct approximate<1,bool>
{
    using value_type = bool;
    auto constexpr order = 1;

    bool value;
    double err;

    auto operator==(bool x) const { return approximate<1,bool>{value==x,err}; }
    auto operator!=(bool x) const { return approximate<1,bool>{value!=x,err}; }

    double fpr() const { return err; }
    double fnr() const { return err; }

    /**
     * Suppose
     *     X ~ random_approximate<1,bool> and
     *     Y ~ random_approximate<0,bool>.
     * Then, by the first-order random approximate Boolean model,
     *     P[X != a|Y == a] == error(X). 
     * 
     * We are given an observation of X rather than Y, but we are
     * interested in
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
     */
    auto error_rate(double p = .5)
    {
        // note: error_rate(.5) = err
        return (1-p)*err / (p*(1-err) + (1-p)*err);
    }

    bool operator() const { return value; }
    auto operator()() const { return value; }
};

auto operator~(approximate<1,bool> a)
{
    return approximate<1,bool>{!a.value,a.err};
}

// at some point, for higher orders, just need to start
// using intervals to span the fpr and fnr.
auto operator&(approximate<1,bool> a, approximate<1,bool> b)
{
    if (a.value && b.value)
        return approximate<3,bool>{a.alpha+b.alpha-a.alpha*b.alpha, true};

    if (!a.value && !b.value)
        return approximate<3,bool>{a.alpha*b.alpha, false};

    if (!a.value && b.value)
        return approximate<3,bool>{a.alpha*(1.-b.alpha), false};

    // a.value && !b.value
    return approximate<3,bool>{(1.-a.alpha)*b.alpha, false};
}

