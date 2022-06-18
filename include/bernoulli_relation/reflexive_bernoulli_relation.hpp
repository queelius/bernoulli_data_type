template <typename R>
struct reflexive_bernoulli_relation
{
public:
    using value_type = typename R::value_type;

    reflexive_bernoulli_relation(R r) : r_(r) {}

    // if (x1, ..., xn) in R then all permutations of (x1, ..., xn) are in R.
    bool contains(value_type const & xs) const
    {
        for (auto const & p : permutations(xs.begin(),xs.end()))
        {
            if (r.contains(p))
                return true;
        }
        return false;
    }

    R r;
};
