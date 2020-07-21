#pragma once
#include "../approximate_value/approximate_value.hpp"

//#include <variant>
//template <typename F, typename G>
//std::variant<approximate_value<F>,approximate_value<G>> test(approximate_bool x, F f, G g)
//{
//    if (x.value)
//        return approximate_value<F>{x.alpha,f};
//    return approximate_value<F>{x.alpha,g};
//}


/**
 * Approximate Booleans are the simplest case to consider.
 */

using approximate_bool = approximate_value<bool>;

approximate_bool operator&(approximate_bool a, approximate_bool b)
{
    if (a.value && b.value)
        return approximate_bool{a.alpha+b.alpha-a.alpha*b.alpha, true};

    if (!a.value && !b.value)
        return approximate_bool{a.alpha*b.alpha, false};

    if (!a.value && b.value)
        return approximate_bool{a.alpha*(1.-b.alpha), false};

    // a.value && !b.value
    return approximate_bool{(1.-a.alpha)*b.alpha, false};
}

approximate_bool operator|(approximate_bool a, approximate_bool b)
{
    if (a.value && b.value)
        return approximate_bool{a.alpha*b.alpha, true};

    if (!a.value && !b.value)
        return approximate_bool{a.alpha+b.alpha-a.alpha*b.alpha, false};

    if (!a.value && b.value)
        return approximate_bool{(1.-a.alpha)*b.alpha, true};

    // a.value && !b.value
    return approximate_bool{a.alpha*(1.-b.alpha), true};
}

approximate_bool operator~(approximate_bool a)
{
    return approximate_bool{a.alpha, !a.value};
}

approximate_bool operator^(approximate_bool a, approximate_bool b)
{
    return (~a & b) | (a & ~b);
}

approximate_bool nand(approximate_bool a, approximate_bool b)
{
    return ~(a & b);
}

approximate_bool nor(approximate_bool a, approximate_bool b)
{
    return ~(a | b);
}

int order(approximate_bool const & a)
{
    return a.alpha == 0. ? 0 : 1;
}

double error_rate(approximate_bool a)
{
    return a.alpha;
}

double fnr(approximate_bool a)
{
    return a.value ? 0. : a.alpha;
}

double fpr(approximate_bool a)
{
    return a.value ? a.alpha : 0.;
}
