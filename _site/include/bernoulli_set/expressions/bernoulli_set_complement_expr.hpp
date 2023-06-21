#pragma once

#include "bernoulli_set_expr.hpp"

namespace bernoulli::set::algebra
{
    template <typename S>
    struct bernoulli_set_complement_expr:
        public bernoulli_set_expr<bernoulli_set_complement_expr<S>>
    {
        using value_type = S::value_type;

        bernoulli_set_complement_expr(S const & s) : s_(s) {}

        auto fpr() const { return s.fnr(); }
        auto fnr() const { return s.fpr(); }
        auto contains(value_type const & x) { return !s_.contains(x); }
        
        S const & s_;
    };

    template <typename S> // S models the concept of a bernoulli set
    auto make_complement(S const & s)
    {
        return bernoulli_set_complement_expr<S>(s);
    }
}










