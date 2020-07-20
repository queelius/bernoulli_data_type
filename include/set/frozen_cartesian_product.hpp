#pragma once

#include <utility>

namespace alex::set
{
template <typename L, typename R>
class frozen_cartesian_product
{
public:
    using left_value_type = typename L::value_type;
    using right_value_type = typename R::value_type;
    using value_type = std::pair<left_value_type, right_value_type>;

    frozen_cartesian_product(L l, R r) : l(l), r(r){};

    bool contains(value_type const &p) const override
    {
        return l.contains(p.first) && r.contains(p.second);
    };

    bool contains(left_value_type const &a, right_value_type const &b) const
    {
        return l.contains(a) && r.contains(b);
    };

private:
    L l;
    R r;
};

} // namespace alex::set