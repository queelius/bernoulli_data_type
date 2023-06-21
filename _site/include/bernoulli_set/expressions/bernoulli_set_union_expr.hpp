#pragma once

#include "bernoulli_set_expr.hpp"

namespace bernoulli::set::algebra
{
    template <typename L, typename R>
    struct bernoulli_set_union_expr: public bernoulli_set_expr<bernoulli_set_union_expr<L,R>>
    {
        using value_type = L::value_type;

        bernoulli_set_union_expr(
            L const & left,
            R const & right)
            : left_(left), right_(right) {}

        auto fpr() const
        {
            return interval<double>(1)-(interval<double>(1)-lfpr())*
                   (interval<double>(1)-rfpr());
        }

        auto lfpr() const { return interval<double>(left_.fpr()); }
        auto rfpr() const { return interval<double>(right_.fpr()); }
        auto lfnr() const { return interval<double>(left_.fnr()); }
        auto rfnr() const { return interval<double>(right_.fnr()); }

        auto fnr() const
        {
            return min_span(lfnr() * rfnr(),
                            lfnr() * (interval<double>(1)-rfpr()),
                            lfnr() * (interval<double>(1)-rfpr()));
        }
        
        auto contains(value_type const & x)
        {
            return left_.contains(x) || right_.contains(x);
        }

        L const & left_;
        R const & right_;
    };

    template <
        typename L, // L models the concept of a bernoulli set
        typename R  // R models the concept of a bernoulli set
    >
    auto make_union(L const & left, R const & right)
    {
        return bernoulli_union_set_expr<L,R>(left,right);
    }
}










