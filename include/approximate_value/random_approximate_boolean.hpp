/**
 * There are only two partitions of bool, {{0},{1}} and {{0,1}},
 * corresponding to first and second-order random approximate
 * Booleans.
 */

#pragma once

#include <random>
#include "approximate_boolean.hpp"

// Conditional random Boolean variable
// that models the concept of a first-order
// random approximate Boolean.
struct first_order_random_approximate_boolean
{
    using value_type = bool;

    double error_rate;
    bool given;

    template <typename R>
    approximate_value<bool> sample(R & r)
    {
        std::bernoulli_distribution ber(error_rate);
        if (given)
            return approximate_value<bool>{r, ber(1-r)};
        else
            return approximate_value<bool>{r, ber(r)};
    }
};


struct second_order_random_approximate_boolean
{
    using value_type = bool;

    double fpr, fnr;
    bool given;

    template <typename R>
    approximate_value<bool> sample(R & r)
    {

    }
};

struct first_order_random_approximate_boolean_pdf
{
    using input_type = first_order_random_approximate_boolean::value_type;
    using outputt_ype = double;

    first_order_random_approximate_boolean b;

    double operator()(bool x) const
    {
        return x == b.given ? 1 - b.error_rate : b.error_rate;
    }
};

// the pdf for the approximate value. that is, given that
// the objective value is x, what is the probability that
// the approximate value is y?
struct second_order_random_approximate_boolean_pdf
{
    using input_type = second_order_random_approximate_boolean::value_type;
    using output_type = double;

    second_order_random_approximate_boolean b;

    double operator()(bool x) const
    {
        if (b.given)
            return x ? 1 - b.fnr : b.fnr;
        return x ? b.fpr : 1 - b.fpr;
    }
};

struct second_order_random_approximate_boolean_cdf
{
    using input_type = second_order_random_approximate_boolean::value_type;
    using output_type = double;

    second_order_random_approximate_boolean b;

    double operator()(bool x) const
    {
        if (b.given)
            return x ? 1 - b.fnr : 1;
        return x ? b.fpr : 1;
    }
};
