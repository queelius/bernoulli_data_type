#pragma once

#include <cstddef>

namespace alex::set::noniterable
{
template <typename T>
class UniversalSet
{
public:
    using value_type = T;

    constexpr bool contains(T const &) const { return true; };
};

template <
    typename S1,
    typename S2>
bool subset_equal(UniversalSet<S1> const & a, UniversalSet<S1> const & b)
{
    return std::same<S1,S2>();
};

template <typename S>
auto make_universal_set(S const & s)
{
    return UniversalSet<S>(s);
};

}