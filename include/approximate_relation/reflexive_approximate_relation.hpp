#pragma once

#include "../pair_tools.hpp"
#include <memory>

namespace alex::approximate_relation
{
    // R models a random approximate set they obeys the reflexive relation.
    template <typename R> 
    class reflexive_approximate_relation
    {
    public:
        // value_type should be some sort of pair type where each element is
        // of the same type, otherwise symmetry should not be possible.
        using value_type = typename R::value_type;

        // we enforce the invariant that for any x in the universal set,
        // (x, x) in R.
        bool contains(value_type const & x) const
        {
            if (fst(x) == snd(x))
                return true;
            return _r.contains(x);
        };

        auto fpr() const { return _r.fpr(); };

        auto tpr() const { return _r.tpr(); };

    private:
        R _r;
    };

    // R models an approximate binary relation
    template <typename R>
    auto make_reflexive_approximate_relation(R r)
    {
        return reflexive_approximate_relation<R>{std::move(r)};
    };
}