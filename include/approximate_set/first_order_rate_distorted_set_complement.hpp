#pragma once

#include "first_order_rate_distorted_set.hpp"

namespace alex::runtime::rate_distorted_set
{
    template <
        typename X,
        typename R = double
    >
    class first_order_rate_distorted_set_complement: public first_order_rate_distorted_set<X,R>
    {
    public:
        first_order_rds_complement(first_order_rds<X,R> const * const rds)
            : rds_(rds) {};
        
        R fpr() const { return rds_->fnr(); }        
        R fnr() const { return rds_->fpr(); }        
        bool contains(X const & x) const { return !rds_->contains(x); }
        
    private:
        first_order_rate_distorted_set<X,R> const * const rds_;
    };
}