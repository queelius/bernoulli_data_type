template <typename B>
class bernoulli_power_set_expr: bernoulli_set_expr<bernoulli_power_set_expr<B>>
{
public:
    bernoulli_power_set_expr(B const & bs) : bs(bs) {};

    using value_type = std::set<typename B::value_type>;

    bool contains(value_type const & s) const
    {
        for (const auto& x : s)
            if (!a.contains(x)) return false;
        return true;
    }

    auto fpr() const
    {
        return 0;
    }

    auto fnr() const
    {
        return 1;
    }

private:
    B const & bs;
};

template <typename B>
bool operator==(
    bernoulli_power_set_expr<B> const & lhs,
    bernoulli_power_set_expr<B> const & rhs)
{
    return a == ps.a;
}

