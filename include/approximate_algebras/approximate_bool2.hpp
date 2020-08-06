#pragma once

#include "rate_span_t.hpp"

/**
 * a boolean value generated from a second-order,
 * positive-negative approximate predicate.
 *
 * for instance, it could be the result of a membership
 * test on a second-order positive-negative approximate
 * characteristic function for some set.
 */


#include "../approximate_value/approximate_value.hpp"

template <
    unsigned N,
    typename Tag>
struct approximate_bool: public approximate_value<bool>
{

};

struct pos_neg {};


template <>
struct approximate_bool<1,pos_neg>
{
   static unsigned const Order = 2;

    using tag = pos_neg;
    using value_type = bool;
    using rate_t = double;

    rate_t err;
    bool value;

    constexpr auto order() const { return Order; }

    rate_t false_negative_rate() const
    {
        return err;
    }

    rate_t false_positive_rate() const
    {
        return err;
    }

    rate_t error(rate_t f = 0.5) const
    {
        return value ?
            err*f / ((1-err)*(1-f) + err*f) :
            err*(1-f) / ((1-err)*f + err*(1-f));
    }
};

approximate_bool<2,pos_neg> operator&(approximate_bool<1,pos_neg> a, approximate_bool<1,pos_neg> b)
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




template <>
struct approximate_bool<2,pos_neg>
{
    static unsigned const Order = 2;

    using tag = pos_neg;
    using value_type = bool;
    using rate_t = double;

    rate_t fnr, fpr;
    bool value;

    constexpr auto order() const { return Order; }

    rate_t false_negative_rate() const
    {
        return fnr;
    }

    rate_t false_positive_rate() const
    {
        return fnr;
    }

    rate_t error(rate_t f = 0.5) const
    {
        return value ?
            fpr*f / ((1-fnr)*(1-f) + fpr*f) :
            fnr*(1-f) / ((1-fpr)*f + fnr*(1-f));
    }
};




approximate_bool<4,pos_neg> operator|(
    approximate_bool<1,pos_neg> a,
    approximate_bool<1,pos_neg> b)
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

approximate_bool<1,pos_neg> operator~(approximate_bool<1,pos_neg> a)
{
    return approximate_bool<1,pos_neg>{a.err, !a.value};
}

approximate_bool<2,pos_neg> operator~(approximate_bool<2,pos_neg> a)
{
    return approximate_bool<2,pos_neg>{a.fpr,a.fnr,!a.value};
}

approximate_bool<4,pos_neg> operator~(approximate_bool<4,pos_neg> a)
{
    return approximate_bool<1,pos_neg>{a.err, !a.value};
}



struct unrestricted {};

template <unsigned N, unsigned M, typename Tag1, typename Tag2>
approximate_bool<N*M,unrestricted> operator^(
    approximate_bool<N,Tag1> a,
    approximate_bool<M,Tag2> b)
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

template <unsigned N, typename Tag>
auto order(approximate_bool<N,Tag> const &)
{
    return N;
}

auto error_rate(approximate_bool<1,pos_neg> a)
{
    return a.err;
}

auto fnr(approximate_bool<1,pos_neg> a)
{
    return a.value ? decltype(a.err){0} : a.err;
}

auto fpr(approximate_bool<1,pos_neg> a)
{
    return a.value ? a.err : decltype(a.err){0};
}




template <>
struct approximate_bool<4,pos_neg>
{
    using value_type = bool;
    using rate_t = double;

    struct fpr_rate_set_t
    {
        static constexpr rate_t DELTA = 1e-50;
        rate_t fpr1, fpr2, fpr3;

        double lower() const { return min(fpr1,fpr2,fpr3); }
        double upper() const { return min(fpr1,fpr2,fpr3); }

        bool contains(double r) const
        {
            return abs(r-fpr1) < DELTA ||
                   abs(r-fpr2) < DELTA ||
                   abs(r-fpr3) < DELTA;
        }

        rate_span_t span() const
        {
            return rate_span_t(lower(), upper());
        }
    };

    rate_t fnr;
    fpr_rate_set_t fpr;
    bool value;

    rate_t false_negative_rate() const
    {
        return fnr;
    }

    // return an interval that is a weighted
    // combination of fpr1, fpr2, fpr3 that
    // maximizes length.
    fpr_rate_set_t false_positive_rate() const
    {
        return fpr;
    }

    rate_span_t error(rate_t f = 0.5) const
    {
        return value ?
            fpr*f / ((1-fnr)*(1-f) + fpr*f) :
            fnr*(1-f) / ((1-fpr)*f + fnr*(1-f));
    }
};

template <unsigned N>
struct approximate_bool<N,pos_neg>
{
    using value_type = bool;
    using rate_scalar_t = rate_span_t::value_type;
    using rate_t = rate_span_t;
    constexpr static unsigned order = N;

    rate_t fnr;
    rate_t fpr;
    bool value;

    auto false_negative_rate() const
    {
        return fnr;
    }

    auto false_positive_rate() const
    {
        return fpr;
    }

    auto error(rate_scalar_t f = 0.5) const
    {
        static const auto one = rate_scalar_t{1};
        auto result = value ?
            fpr*f / ((one-fnr)*(one-f) + fpr*f) :
            fnr*(one-f) / ((one-fpr)*f + fnr*(one-f));

        return rate_t(min(result), max(result));
    }
};

template <unsigned N>
approximate_bool<N,pos_neg> operator&(
    approximate_bool<N,pos_neg> a,
    approximate_bool<N,pos_neg> b)
{
    if (a.value && b.value)
        return approximate_boolean_2pn{a.alpha+b.alpha-a.alpha*b.alpha, true};

    if (!a.value && !b.value)
        return approximate_value<bool>{a.alpha*b.alpha, false};

    if (!a.value && b.value)
        return approximate_value<bool>{a.alpha*(1.-b.alpha), false};

    // a.value && !b.value
    return approximate_value<bool>{(1.-a.alpha)*b.alpha, false};
}

