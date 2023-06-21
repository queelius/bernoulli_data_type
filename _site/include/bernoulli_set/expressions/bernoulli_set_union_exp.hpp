template <bernoulli_set A, bernoulli_set B>
struct bernoulli_set_union_exp:
    public bernoulli_set_exp<bernoulli_set_union_exp<A,B>>
{
    using value_type = typename A::value_type;

    auto contains(value_type const & x) { return a.contains(x)||b.contains(x); };

    auto error(value_type const & x)
    {
        1-implicit_probability(x);
    }

    auto false_positive_rate() const
    {
        using T = decltype(a.false_positive_rate());

        return T(1)-
            (T(1)-a.false_positive_rate())*(T(1)-b.false_positive_rate());
    }

    auto false_negative_rate() const
    {
        using std::min;
        using std::max;
        using T = decltype(a.false_positive_rate())

        auto p1 = a.false_negative_rate() * b.false_negative_rate(),
             p2 = a.false_negative_rate()*(T(1)-b.false_positive_rate()),
             p3 = b.false_negative_rate()*(T(1)-a.false_positive_rate());

        return std::pair<T,T>{min(min(p1,p2),p3),max(max(p1,p2),p3)};
    }

    A const & a;
    B const & b;
};

template <bernoulli_set A, bernoulli_set B>
auto make_union_exp(A a, B b)
{
    return bernuolli_set_union_exp<A,B>{ }
}
