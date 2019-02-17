#include <memory>

namespace random_approximate_set_model
{
    template <
        typename P = double,
        typename X,    
        template <typename> class I>
    class RAS
    {
    public:
        // All elements in the approximate set have values of type X.
        using value_type = X;

        using interval_type = I;
        using probability_type = P;

        aset(fpr_type fpr, fnr_type fnr) :
            _fpr(fpr), _fnr(fnr) {};

        virtual I<P> fpr() const = 0;
        virtual I<P> fnr() const = 0;

        // If X is in the objective set, contains(X) returns false with an
        // expected probability in the interval fnr(). Otherwise, returns
        // true with an expected probability in the interval fpr().
        virtual bool contains(const X&) const = 0;
    };

    template <
        typename P = double,
        typename X,
        template <typename> class I>
    using RAS_P = std::unique_ptr<RAS<P,X,I>>;

    template <
        typename P = double,
        typename X,
        template <typename> class I
    >
    class RASEmpty: public RAS<P,X,I>
    {
    public:
        virtual I<P> fpr() const { return I<P>(0,0); };
        virtual I<P> fnr() const { return I<P>(0,0); };
        bool contains(const X&) const { return false; };
    };

    template <
        typename P = double,
        typename X,
        template <typename> class I
    >
    class RASUniversal: public RAS<P,X,I>
    {
    public:
        virtual I<P> fpr() const { return I<P>(0,0); };
        virtual I<P> fnr() const { return I<P>(0,0); };
        bool contains(const X& x) const { return true; };
    };

    template <
        typename P = double,
        typename X,
        template <typename> class I
    >
    RAS_P<P,X,I> make_ras_empty()
    {
        return std::make_unique(
            RASEmpty<P,X,I>());
    };

    template <
        typename P = double,
        typename X,
        template <typename> class I
    >
    RAS_P<P,X,I> make_ras_universal()
    {
        return std::make_unique(
            RASUniversal<P,X,I>());
    };    
}