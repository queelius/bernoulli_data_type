#pragma once

/**
 * The expected error rate, denoted by alpha,
 * for the equality predicate is given by
 *     Pr[approximate_value<T>{T(t)} != T(t)] = alpha,
 * where the value T(t) is uncertain (a latent
 * variable) and approximate_value<T>{T(t)} is
 * an observable quantity.
 *
 * If we are given T(t), then the probability
 * that approximate_value<T>{T(t)} takes on
 * a specific value is given by the nature
 * of the random approximate model. For instance,
 * a second-order random approximate Boolean
 * is 
 *
 *
 * There is usually much more structure
 * in the distribution of approximate values.
 * At a more fine-grained level, we may be
 * interested in
 *     Pr[T = a | T' = approximate_value<T>(x)].
 *
 * We can reduce this to the 
 * although the random approximate value type
 * model does not make any
 * even though most random approximate
 * value types have more complicated structure,
 * e.g., an entropy map generates
 * approximate maps in which each value mapped
 * to is a function of the implicit probability
 * as given by the entropy coder.
 */

template <typename T>
struct approximate_value
{
    using value_type = T;
    using rate_type = double;

    double alpha;
    T value;
};


approximate_value<bool> operator&(approximate_value<bool> a, approximate_value<bool> b)
{
    if (a.value && b.value)
        return approximate_value<bool>{a.alpha+b.alpha-a.alpha*b.alpha, true};

    if (!a.value && !b.value)
        return approximate_value<bool>{a.alpha*b.alpha, false};

    if (!a.value && b.value)
        return approximate_value<bool>{a.alpha*(1.-b.alpha), false};

    // a.value && !b.value
    return approximate_value<bool>{(1.-a.alpha)*b.alpha, false};
}


approximate_value<bool> operator|(approximate_value<bool> a, approximate_value<bool> b)
{
    if (a.value && b.value)
        return approximate_value<bool>{a.alpha*b.alpha, true};

    if (!a.value && !b.value)
        return approximate_value<bool>{a.alpha+b.alpha-a.alpha*b.alpha, false};

    if (!a.value && b.value)
        return approximate_value<bool>{(1.-a.alpha)*b.alpha, true};

    // a.value && !b.value
    return approximate_value<bool>{a.alpha*(1.-b.alpha), true};
}


approximate_value<bool> operator~(approximate_value<bool> a)
{
    return approximate_value<bool>{a.alpha, !a.value};
}

approximate_value<bool> operator^(approximate_value<bool> a, approximate_value<bool> b)
{
    return (~a & b) | (a & ~b);
}

approximate_value<bool> nand(approximate_value<bool> a, approximate_value<bool> b)
{
    return ~(a & b);
}

approximate_value<bool> nor(approximate_value<bool> a, approximate_value<bool> b)
{
    return ~(a | b);
}

