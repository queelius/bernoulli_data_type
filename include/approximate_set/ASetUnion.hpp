// Define the concept of an approximate set. Paper deals with it. C++ neds
// a bit more, like an implemention T needs to define T::value_type and
// T::contains, etc.


template <
    class X,                    // X is the value type

    template <class> class A,   // A<X> and B<X> are classes that implement
    template <class> class B,   // the abstract data type of approximate sets
                                // over X. Defines A::value_type where
                                // same<A::value_type,X> is permissible for
                                // A and same for B.
    
    template <class> class I>   // Supports interval arithmetic operations and
                                // overloads
                                //     span : I<P>*I<P> -> I<P>.
class ASetUnion
{
public:
    using value_type = X;
    using interval_type = I;

    bool contains(const X& x) { return a.contains(x) || b.contains(x); };

    Interval fpr() const
    {
        return one() - (one() - a.fpr()) * (one() - b.fpr());
    };

    auto fnr() const
    {
        return span(
            a.fnr() * b.fnr(),
            span(
                a.fnr()*(one()-b.fpr()),
                b.fnr()*(one()-a.fpr())));
    };

private:
    A<X> a;
    B<X> b;

    constexpr I<P> one() { return I<P>(1,1); };
};

    template <class A, class B>
    auto make_union(A a, B b)
    {
        using X = typename A::value_type;
        return ASetUnion<X, A, B>(a, b);
    };