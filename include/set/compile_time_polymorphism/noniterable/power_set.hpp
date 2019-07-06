#include <set>

namespace alex::set::noniterable
{
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
}