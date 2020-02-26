#include "aset.hpp"
#include <memory>

namespace random_approximate_set_model
{
    template <
        typename P = double,
        typename X,
        template <typename> class I>
    class RASUnion: public RAS<P,X,I>
    {
    public:
        RASUnion(RAS<P,X,I> const * const s1,
                 RAS<P,X,I> const * const s2)
            : s1(s1), s2(s2) {};

        I<P> fpr() const
        {
            return one-(one-s1->fpr())*(one-s2->fpr());
        };

        I<P> fnr() const
        {
            return interval_span({
                s1->fnr()*(one-s2->fnr()),
                s2->fnr()*(one-s1->fnr()),
                s1->fnr()*s2->fnr()});
        };
        
        bool contains(const X& x) const
        {
            return s1->contains(x) || s2->contains(x);
        };
        
    private:
        RAS<P,X,I> const * const s1;
        RAS<P,X,I> const * const s2;

        constexpr one() { return Interval(1,1); };
    };

    template <
        typename P,
        typename X,
        template <typename> class I
    >
    RAS_P<P,X,I> make_ras_union(
        RAS<P,X,I> const * const s1,
        RAS<P,X,I> const * const s2)
    {
        return std::make_unique(
            RASUnion<P,X,I>(s1, s2));
    };
}