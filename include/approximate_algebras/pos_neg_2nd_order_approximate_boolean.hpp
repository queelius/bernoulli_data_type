#pragma once

#include <algorithm>
using std::abs;
using std::min;
using std::max;

struct approximate_boolean
{
    using value_type = bool;
    using fnr_rate_t = double;

    struct fpr_rate_t
    {
        static constexpr double DELTA = 1e-50;
        double fpr1, fpr2, fpr3;

        struct span_t
        {
            double lb, ub;

            bool contains(double r)
            {
                return r <= ub && r >= lb;
            }
        };

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
            return rate_span_t{lower(), upper()};
        }
    };

    fnr_rate_t fnr;
    fpr_rate_t fpr;
    bool value;

    fnr_rate_t false_negative_rate() const
    {
        return fnr;
    }

    // return an interval that is a weighted
    // combination of fpr1, fpr2, fpr3 that
    // maximizes length.
    fpr_rate_t false_positive_rate() const
    {
        return fpr;
    }

    rate_type error(rate_type f = 0.5) const
    {
        return value ?
            fpr*f / ((1-fnr)*(1-f) + fpr*f) :
            fnr*(1-f) / ((1-fpr)*f + fnr*(1-f));
    }
};


/**
 * a boolean value generated from a second-order,
 * positive-negative approximate predicate.
 *
 * for instance, it could be the result of a membership
 * test on a second-order positive-negative approximate
 * characteristic function for some set.
 */
struct approximate_boolean_2pn
{
    using value_type = bool;
    using rate_type = double;

    rate_type fnr, fpr;
    bool value;

    rate_type false_negative_rate() const
    {
        return fnr;
    }

    rate_type false_positive_rate() const
    {
        return fnr;
    }

    rate_type error(rate_type f = 0.5) const
    {
        return value ?
            fpr*f / ((1-fnr)*(1-f) + fpr*f) :
            fnr*(1-f) / ((1-fpr)*f + fnr*(1-f));
    }
};

approximate_boolean_4pn operator&(approximate_boolean_2pn a, approximate_boolean_2pn b)
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

