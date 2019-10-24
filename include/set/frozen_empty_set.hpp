#pragma once

#include <cstddef>

namespace alex::set
{
template <typename T>
struct empty_set
{
    using value_type = T;
    using cardinality_type = size_t;

    constexpr bool contains(T const &) const { return false; };
    constexpr cardinality_type cardinality() const { return 0; };

    // empty_set<T> is a singleton, i.e., there is only one
    // value of empty_set<T>. The iterator is pretty straightforward
    // as a result, i.e., it is always at position e, the end, in
    // the half-open interval [b, e).
    struct const_iterator
    {
        T operator*() { return T{}; };
        const_iterator operator++() { return *this; };
        const_iterator operator++(int) { return *this; };
        bool operator==(const_iterator const &) { return true; };
        bool operator!=(const_iterator const &) { return false; };
    };

    const_iterator cend() const
    {
        return const_iterator();
    };

    const_iterator end() const
    {
        return const_iterator();
    };
};

template <
    typename T,
    typename S>
bool subset(empty_set<T>, S const &)
{
    return true;
};

template <
    typename T,
    typename S>
bool equal(empty_set<T>, S const &s)
{
    return s.cardinality() == 0;
};

} // namespace alex::set