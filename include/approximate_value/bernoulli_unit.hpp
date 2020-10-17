#include "random_approximate_value.hpp"

/**
 * Unit type
 * ---------
 *
 * The unit type is any type with only a single value in its set of possible values.
 * 
 * By an accident of history, the unit type is denoted by the keyword 'void' in C++
 * and we cannot use values of this type in an expression, e.g., suppose we have
 * a function conceptually of type f : unit -> unit must be the identity function
 * and
 *     unit x = f(unit())
 * is coherent and well-defined.
 * 
 * However, in C++,
 *     void x = f(void())
 * is not a valid expression. We can always create our own unit type, e.g., struct unit{}, 
 * and then unit x = f(unit()) is valid.
 * 
 * There is no reason to store the unit value in a named variable since unit{} already
 * singles out the only possible value any such variable can have.
 * 
 * If an approximation of unit{} is to remain a value of the unit type, it is not
 * possible to have any errors -- it must remain the value unit{}. Thus, the *only*
 * Bernoulli model for the unit type is the zero-th order model (no error).
 * 
 * Note: Conceptually, there is one simpler type, the *absurd* type, typically denoted
 * by Void. By an accident of history, in C++, the unit type is denoted by void. The absurd
 * type has values from the empty set, i.e., it is not possible to introduce a value of the
 * absurd type.
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

