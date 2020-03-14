#pragma once

namespace alex::runtime::rate_distorted_set
{
    // I models a real interval, or a disjoint subset of intervals,
    // that is a subset of [0,1].
    template <
        typename X,
        typename I
    >
    struct second_order_rate_distorted_set
    {
        virtual I fpr() const = 0;
        virtual I fnr() const = 0;

        // Describe distribution, bit more complicated due to partition
        // of sets. This is algebra of random approximate sets.
        virtual bool contains(X const &) const = 0;
    }; 
}