#pragma once

namespace alex::runtime::rate_distorted_set
{
    // All elements in the first-order rate-distorted set have values of
    // type X.
    template <
        typename X,
        typename R = double
    >
    struct first_order_rate_distorted_set
    {
        virtual R fpr() const = 0;
        virtual R fnr() const = 0;

        // If value x of type X is in the set being modeled, contains(x)
        // returns false with probability fnr() and true with probability
        // 1-fnr().
        //
        // Otherwise, contains(x) returns true with p robability fpr() and
        // false with probability 1-fpr().
        virtual bool contains(X const &) const = 0;
    }; 
}