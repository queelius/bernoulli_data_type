#include "aset.hpp"

namespace approximate_set_model
{
    // The true positive rate.
    template <
        typename P,
        typename X,
        template <typename> class I>
    auto tpr(RAS<P,X,I> const & s)
    {
        return I<P>(1,1) - s.fnr();
    };

    // The true negative rate.
    template <
        typename P,
        typename X,
        template <typename> class I>
    auto tnr(RAS<P,X,I> const & s)
    {
        return I<P>(1,1) - s.fpr();
    };

    // The accuracy.
    template <
        typename P,
        typename X,
        template <typename> class I>
    auto accuracy(RAS<P,X,I> const & s, const I<P>& lambda)
    {
    };
        
    // The positive predictive value.
    template <
        typename P,
        typename X,
        template <typename> class I,
        typename U>
    auto ppv(RAS<P,X,I> const & s, const I<P>& lambda, const U& u)
    {
    };

    // The positive predictive value.
    template <
        typename P,
        typename X,
        template <typename> class I,
        typename U>
    auto npv(RAS<P,X,I> const & s, const I<P>& lambda, const U& u)
    {
    };
}