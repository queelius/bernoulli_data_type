#include "aset.hpp"

namespace random_approximate_set_model
{
    template <
        typename P = double,
        typename X,
        template <typename> class I
    >
    class RASComplement: public RAS<P,X,I>
    {
    public:
        RASComplement(RAS<P,X,I> const * const s)
            : s(s) {};
        
        I<P> fpr() const
        {
            return s->fnr();
        };
        
        I<P> fnr() const
        {
            return s->fpr();
        };
        
        bool contains(X const & x) const
        {
            return !s->contains(x);
        };
        
    private:
        RAS<P,X,I> const * const s;
    };

    template <
        typename P,
        typename X,
        template <typename> class I
    >
    RAS_P<P,X,I> make_ras_complement(
        RAS<P,X,I> const * const s)
    {
        return std::make_unique(RASComplement<P,X,I>(s));
    };
}