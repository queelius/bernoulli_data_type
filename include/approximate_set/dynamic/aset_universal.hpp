#include "aset.h"

namespace approximate_set_model
{
    template <typename X, typename Interval>
    class ASetUniversal: public ASet<X,Interval>
    {
    public:
        bool contains(const X& x) const
        {
            return false;
        };
    };

    template <typename X, typename Interval>
    ASetP<X,Interval> make_empty()
    {
        return std::make_unique(
            ASetEmpty<X,Interval>());
    };
}