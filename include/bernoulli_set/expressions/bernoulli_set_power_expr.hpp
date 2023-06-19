template <typename B>
class bernoulli_power_set_expr: bernoulli_set_expr<bernoulli_power_set_expr<B>>
{
public:
    bernoulli_power_set_expr(B const & bs) : bs(bs) {};

    using value_type = std::set<typename B::value_type>;

    auto contains(value_type const & s) const
    {
        bernoulli_bool<X,2> res(true,0,0)
        for (auto const & x : s)
            res = res && contains(x);
        return res;
    }

    auto fpr() const
    {
        return interval<double>(0,1);
    }

    auto fnr() const
    {
        return interval<double>(0,1);
    }

private:
    B const & bs;
};

