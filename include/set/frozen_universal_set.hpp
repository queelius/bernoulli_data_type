#pragma once

#include <type_traits>

namespace alex::set
{
template <typename T>
class frozen_universal_set
{
public:
    using value_type = T;

    constexpr bool contains(T const &) const { return true; };
};

template <typename S, typename T>
constexpr bool subset(
    S const &,
    frozen_universal_set<T> const &)
{
    return std::is_same<typename S::value_type, T>::value;
};

template <typename T>
auto make_frozen_universal_set(T const &)
{
    return frozen_universal_set<T>{};
};

}