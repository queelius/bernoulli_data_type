#pragma once

#include "bernoulli_set_expr.hpp"

namespace bernoulli::set::expression
{
    template <typename L, typename R, typename Interval = math::interval<double>>
    struct bernoulli_set_union_expr: public bernoulli_set_expr<bernoulli_set_union_expr<L,R>>
    {
        using value_type = L::value_type;
        using interval_type = Interval;

        bernoulli_set_union_expr(
            L const & left,
            R const & right)
            : left(left), right(right) {}

        auto fpr() const
        {
            return Interval(1)-(Interval(1)-left_fpr())*
                (Interval(1)-right_fpr());
        }

        auto left_fpr() const { return Interval(left.fpr()); }
        auto right_fpr() const { return Interval(right.fpr()); }

        auto left_fnr() const { return Interval(left.fnr()); }
        auto right_fnr() const { return Interval(right.fnr()); }

        auto fnr() const
        {
            return min_span(left_fnr() * right_fnr(),
                            left_fnr() * (Interval(1)-right_fpr()),
                            left_fnr() * (Interval(1)-right_fpr()));
        }
        
        auto contains(value_type const & x) const
        {
            // contains : (L,value_type) -> bernoulli<P1,bool,eq>
            // contains : (R,value_type) -> bernoulli<P2,bool,eq>
            //
            // auto operator||(bernoulli<P1,bool,eq>,bernoulli<P2,bool,eq>)
            // should be defined so that it returns the appropriate
            // bernoulli boolean.
            return left.contains(x) || right.contains(x);
        }
        
        L const & left;
        R const & right;
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
