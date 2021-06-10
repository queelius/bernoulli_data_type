#pragma once

#include "second_order_rate_distorted_set.hpp"

namespace bernoulli::set
{
    template <typename S>
    class bernoulli_set_complement_expr: public bernoulli_expr<bernoulli_set_complement_expr<S>>
    {
    public:
        bernoulli_set_complement_expr(S const & s) : s(s) {}

        auto fpr() const { return s.fnr(); }
        auto fnr() const { return s.fpr(); }
        
        auto contains(value_type const & x) const
        {
            return !s.contains(x);
        }
        
    private:
        S const & s;
    };
}