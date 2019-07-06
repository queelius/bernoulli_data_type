#include <set>
#include <algorithm>

namespace alex::set
{
template <typename T>
struct UniversalSet
{
    constexpr bool contains(T const &) const { return true; };
};

template <typename T>
struct EmptySet
{
    constexpr bool contains(T const &) const { return false; };
};

template <
    typename S,
    template <typename> typename IterableConainer = std::set>
class PowerSet
{
public:
    using set_type = S;
    using value_type = IterableContainer<typename S::value_type>;

    PowerSet(S const & s) : _s(s) {};

    bool contains(value_type const & x) const
    {
        return std::all_of(begin, end,
            [_s](auto const & x) { return _s.contains(x); });
    };

private:
    S const & _s;
};

template <typename L, typename R, typename I = size_t>
class CartesianProduct
{
public:
    using left_value_type = typename L::value_type;
    using right_value_type = typename R::value_type;
    using value_type = std::pair<left_value_type, right_value_type>;

    CartesianProduct(L l, R r) : l(l), r(r) {};

    bool contains(value_type const & p) const override
    {
        return l.contains(p.first) && r.contains(p.second);
    };

    bool contains(left_value_type const & a, right_value_type const & b) const
    {
        return l.contains(a) && r.contains(b);
    };

private:
    L l;
    R r;
};

}