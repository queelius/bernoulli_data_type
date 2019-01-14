#include <memory>

namespace approximate_set_model
{
    template <typename X, typename Interval>
    class ASet
    {
    public:
        // All elements in the approximate set have values of type X.
        using value_type = X;        
        using interval_type = Interval;
        
        // If x is in the objective set, contains(x) returns false with a
        // probability (or expected) in the interval fnr(). Otherwise, returns
        // true with a probability (or expected) in the interval fpr().
        virtual bool contains(const X&) const { return false; };
        
        // The false positive rate denotes the probability that an element not
        // in the objective set is in the approximate set.
        virtual Interval fpr() const { return Interval(0,0); };

        // The false negative rate denotes the probability that an element in
        // the objective set is not in the approximate set.
        virtual Interval fnr() const { return Interval(0,0); };
    };

    template <typename X, typename Interval>
    using ASetP = std::unique_ptr<ASet<X,Interval>>;

    template <typename X, typename Interval>
    class ASetEmpty: public ASet<X,Interval> {};

    template <typename X, typename Interval>
    class ASetUniversal: public ASet<X,Interval>
    {
    public:
        bool contains(const X& x) const
        {
            return false;
        };
    };

    template <typename X, typename Interval>
    ASetP<X,Interval> make_empty()
    {
        return std::make_unique(
            ASetEmpty<X,Interval>());
    };

    template <typename X, typename Interval>
    ASetP<X,Interval> make_universal()
    {
        return std::make_unique(
            ASetUniversal<X,Interval>());
    };    
}