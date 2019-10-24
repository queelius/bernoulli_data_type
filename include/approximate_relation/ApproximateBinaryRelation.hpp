namespace alex::approximate_binary_relation::iterable
{
    template <
        typename R1,
        typename R2
    >
    class ApproximateUnionRelation
    {
    public:
        using left_type = typename R1::left_type;
        using right_type = typename R1::right_type;
        using value_type = typename std::pair<left_type, right_type>;

        auto fpr() const
        {
            return 0.0;
        };

        auto tpr() const
        {
            return 1.0;
        };

        bool operator()(value_type const & p) const
        {
            return _l(p.first, p.second) || _r(p.first, p.second);
        };

        bool operator()(left_type a, right_type b) const
        {
            return _l(a, b) || _r(a, b);
        };

        std::set<right_type> operator()(left_type x) const
        {
            std::set<right_type> ys;
            for (auto [a,b] : _l)
            {
                if (a == x)
                    ys.insert(b);
            }
            for (auto [a,b] : _r)
            {
                if (a == x)
                    ys.insert(b);
            }
            return ys;
        };

        auto begin() const
        {

        };

        auto end() const
        {

        };

    private:
        std::set<value_type> R;
        R1 _l;
        R2 _r;
    };
}