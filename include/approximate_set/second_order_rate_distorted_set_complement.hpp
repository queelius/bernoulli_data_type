#pragma once

#include "second_order_rate_distorted_set.hpp"

namespace alex::runtime::rate_distorted_set
{
    template <
        typename X,     // elements of the set are type X

        typename I      // I models an interval concept. Second-order
                        // rate-distorted sets have uncertain fpr and fnr, where
                        // we model the uncertainty with an interval.
    >
    class second_order_rate_distorted_set_complement: public second_order_rate_distorted_set<X,I>
    {
    public:
        second_order_rate_distorted_set_complement(
            second_order_rate_distorted_set<X,I> const * const rds)
            : rds_(rds) {};

        I fpr() const { return rds_->fnr(); }
        I tpr() const { return rds_->fpr(); }

        I fnr() const { return I(1,1) - rds_->fpr(); }
        I tnr() const { return I(1,1) - rds_->fpr(); }
        
        bool contains(X const & x) const { return rds_->contains(x); }
        
    private:
        second_order_rate_distorted_set<X,I> const * const rds_;
    };
}