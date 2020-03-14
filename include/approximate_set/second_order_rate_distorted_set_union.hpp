#pragma once

#include "first_order_rate_distorted_set.hpp"
#include "second_order_rate_distorted_set.hpp"

namespace alex::runtime::rate_distorted_set
{
    template <
        typename X,
        typename R,
        template <typename> typename I
    >
    class second_order_rate_distorted_set_union:
        public second_order_rate_distorted_set<X,I<R>>
    {
    public:
        template <typename R>
        second_order_rate_distorted_set_union(
            first_order_rate_distorted_set<X,R> const * const rds1,
            first_order_rate_distorted_set<X,R> const * const rds2)
            : rds1_(rds1), rds2_(rds2) {};

        I<R> fpr() const
        {
            return I<R>(0,0);
        }

        I<R> fnr() const
        {
            return I<R>(0,0);
        }
        
        bool contains(X const & x) const
        {
            return rds1_->contains(x) || rds2_->contains(x);
        }
        
    private:
        first_order_rate_distorted_set<X,R> const * const rds1_,
        first_order_rate_distorted_set<X,R> const * const rds2_;
    };
}