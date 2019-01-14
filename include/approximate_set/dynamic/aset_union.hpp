#include "aset.h"
#include <memory>

namespace approximate_set_model
{
    template <typename X, typename Interval>
    class ASetUnion: public ASet<X,Interval>
    {
    public:
        ASetUnion(ASet<X, Interval> const * const s1,
                ASet<X, Interval> const * const s2)
            : s1(s1), s2(s2) {};

        Interval fpr() const
        {
            return one-(one-s1->fpr())*(one-s2->fpr());
        };

        Interval fnr() const
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
        ASet<X,Interval> const * const s1;
        ASet<X,Interval> const * const s2;

        constexpr one() { return Interval(1,1); };
    };

    template <typename X, typename Interval>
    ASetP<X,Interval> make_union(
        ASet<X,Interval> const * const s1,
        ASet<X,Interval> const * const s2)
    {
        return std::make_unique(
            ASetUnion<X,Interval>(s1, s2));
    };
}