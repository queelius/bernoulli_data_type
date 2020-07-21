#include "approximate_value.hpp"
#include <variant>

/**
 * Approximate Booleans are the simplest case to consider.
 */

template <typename F, typename G>
std::variant<approximate_value<F>,approximate_value<G>> test(approximate_value<bool> x, F f, G g)
{
    if (x.value)
        return approximate_value<F>{x.alpha,f};
    return approximate_value<F>{x.alpha,g};
}

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

