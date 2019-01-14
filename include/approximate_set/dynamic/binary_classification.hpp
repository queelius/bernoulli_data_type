#include "aset.h"

namespace approximate_set_model
{
    // The true positive rate.
    template <typename X, typename Interval>
    auto tpr(ASet<X, Interval> const & s)
    {
        return Interval(1,1) - s.fnr();
    };

    // The true negative rate.
    template <typename X, typename Interval>
    auto tnr(ASet<X, Interval> const & s)
    {
        return Interval(1,1) - s.fpr();
    };

    // The accuracy.
    template <typename X, typename Interval>
    auto accuracy(ASet<X, Interval> const & s, Interval lambda)
    {
    };
        
    // The positive predictive value.
    template <typename X, typename Interval, typename Cardinality>
    auto ppv(ASet<X, Interval> const & s, Interval lambda, Cardinality u)
    {
    };

    // The positive predictive value.
    template <typename X, typename Interval, typename Cardinality>
    auto npv(ASet<X, Interval> const & s, Interval lambda, Cardinality u)
    {
    };
}