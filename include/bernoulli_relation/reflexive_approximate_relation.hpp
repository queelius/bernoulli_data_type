template <
    typename R
>
class reflexive_approximate_relation
{
public:
    using value_type = typename R::value_type;

    reflexive_approximate_relation(R r) : r_(r) {}

    // if (x1, ..., xn) in R then all permutations of (x1, ..., xn) are in R.
    bool contains(value_type const & xs) const
    {
        for (auto const & p : permutations(xs.begin(),xs.end()))
        {
            if (r_.contains(p))
                return true;
        }
        return false;
    }

    auto tnr(double n) const { return std::pow(r_.tnr(), n); }
    auto fpr(double n) const { return 1. - tnr(); }
    auto tpr(double n) const { return 1. - fnr(); }
    auto fnr(double n) const { return std::pow(r_.fnr(), n); }

private:
    R r_;
};
