#include "../pair_tools.hpp"
#include <memory>

namespace alex::approximate_relation
{
    // R models an approximate binary relation of a symmetric relation.
    template <typename R>
    class symmetric_approximate_relation
    {
    public:
        // value_type should be some sort of pair type
        // where each element is of the same type,
        // otherwise symmetry should not be possible.
        using value_type = typename R::value_type;

        // we enforce the invariant that if (x, y) in R
        // then (y, x) in R.
        bool contains(value_type const & p) const
        {
            return _r.contains(p) || _r.contains(snd(p), fst(p));
        };

        auto fpr() const { return 1. - std::pow(1 - _r.fpr(), 2.); };

        // if R is a random approximate set, then the
        // true positive rate with respect to the
        // modified contains function still has the
        // same true positive rate.
        auto tpr() const { return _r.tpr(); };

    private:
        R _r;
    };

    template<
        typename R // R models an approximate binary relation
    >
    auto make_symmetric_approximate_relation(R r)
    {
        return symmetric_approximate_relation<R>{std::move(r)};
    };
}