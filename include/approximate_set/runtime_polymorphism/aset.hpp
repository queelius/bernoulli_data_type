#include <memory>

namespace alex::random_approximate_set::noniterable
{
    template <
        typename R,
        typename X>
    class RandomApproximateSet: public alex::set::noniterable::set<X>
    {
    public:
        // All elements in the approximate set have values of type X.
        using value_type = X;
        using rate_type = R;

        virtual R fpr() const = 0;
        virtual R fnr() const = 0;

        // If X is in the objective set, contains(X) returns false with an
        // expected probability in the interval fnr(). Otherwise, returns
        // true with an expected probability in the interval fpr().
        virtual bool contains(X const &) const = 0;
    };

    using 

    template <
        typename R,
        typename X>
    using RAS = std::unique_ptr<RandomApproximateSet<R,X>>;

    template <
        typename R,
        typename X
    >  
}