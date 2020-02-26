#include "aset.hpp"

namespace alex::random_approximate_set
{
    template <
        typename X,
        class P
    >
    class RASComplement: public RAS<P,X,I>
    {
    public:
        RASComplement(RAS<P,X,I> const * const s)
            : s(s) {};
        
        P fpr() const
        {
            return s->fnr();
        };
        
        P fnr() const
        {
            return s->fpr();
        };
        
        bool contains(X const & x) const
        {
            return !s->contains(x);
        };
        
    private:
        RAS<P,X> const * const s;
    };

    template <
        typename P,
        typename X
    >
    RAS_P<P,X,I> make_ras_complement(
        RAS<P,X,I> const * const s)
    {
        return std::make_unique(RASComplement<P,X,I>(s));
    };
}