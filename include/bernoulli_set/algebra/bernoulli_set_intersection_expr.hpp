#pragma once

#include "bernoulli_set_expr.hpp"

namespace bernoulli::set::algebra
{
    template <typename L, typename R>
    struct bernoulli_set_intersection_expr:
        public bernoulli_set_expr<bernoulli_set_intersection_expr<L,R>>
    {
        using value_type = L::value_type;

        constexpr interval<double> one() { return interval<double>(1) }

        bernoulli_set_intersection_expr(
            L const & left,
            R const & right)
            : left_(left), right_(right) {}

        auto fpr() const
        {
            return one() - (one() - left_.fpr()) * (one() - right_.fpr());
        }

        auto fnr() const
        {
            return span(
                left_.fnr() * right_.fnr(),
                span(left_.fnr()*(one()-right_.fpr()),
                    right_.fnr()*(one()-left_.fpr())));
        }

        
        auto contains(value_type const & x)
        {
            return left_.contains(x) && right_.contains(x);
        }
        
        L const & left_;
        R const & right_;
    };

    template <
        typename L, // L models the concept of a bernoulli set
        typename R  // R models the concept of a bernoulli set
    >
    auto make_intersection(L const & left, R const & right)
    {
        return bernoulli_intersection_set_expr<L,R>(left,right);
    }
}










