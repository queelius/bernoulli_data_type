#include "aset.h"

namespace approximate_set_model
{
    template <typename X, typename Interval>
    class ASsetComplement: public ASet<X,Interval>
    {
    public:
        ASetComplement(ASet<X,Interval> const * const s)
            : s(s) {};
        
        Interval fpr() const
        {
            return s->fnr();
        };
        
        Interval fnr() const
        {
            return s->fpr();
        };
        
        bool contains(X const & x) const
        {
            return !s->contains(x);
        };
        
    private:
        ASet<X,Interval> const * const s;
    };

    template <typename X, typename Interval>
    ASetP<X,Interval> make_complement(
        ASet<X,Interval> const * const s)
    {
        return std::make_unique(ASetComplement<X,Interval>(s));
    };
}