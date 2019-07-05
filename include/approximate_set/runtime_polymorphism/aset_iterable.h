#include "aset.hpp"

namespace alex::set::approximate_set
{
    /*
    * ASetIterable is an ASet plus two functions,
    * begin and end, to retrieve iterators to the
    * beginning and ending of the iterable collection.
    */
    template <typename X, typename Interval>
    class ASetIterable: public ASet<X,Interval>
    {
    public:
        using typename ASet<X, Interval>::value_type;
        using typename ASet<X, Interval>::interval_type;
        using cardinality_type = size_t;
    };

    template <typename X, typename Interval>
    using ASetIterableP = std::unique_ptr<ASetIterable<X,Interval>>;
}