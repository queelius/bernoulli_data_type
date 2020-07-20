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

    // The iterator is pretty straightforward since empty_set<T> has only
    // a single state that it can be in which represents a set with no elements.
    // As a result, the iterator is always at the end.
    struct const_iterator
    {
        T operator*() { return T{}; }
        const_iterator operator++() { return *this; }
        const_iterator operator++(int) { return *this; }
        bool operator==(const_iterator const &) { return true; }
        bool operator!=(const_iterator const &) { return false; }
    };

    const_iterator end() const { return const_iterator(); }
    const_iterator begin() const { return const_iterator(); }
};

template <typename T, typename S>
auto operator+(empty_set<T> const &, S s) { return s; }

template <typename T, typename S>
auto operator+(S s, empty_set<T> const &) { return s; }

template <typename T, typename S>
auto operator*(S const &, empty_set<T> const &) { return empty_set<T>{}; }

template <typename T, typename S>
auto operator*(empty_set<T> const &, S const &) { return empty_set<T>{}; }

template <typename T>
bool contains(empty_set<T> const &, T const &) { return false; }

template <typename T, typename S>
bool operator<=(empty_set<T> const &, S const &) { return true; }

template <typename T, typename S>
bool operator<(empty_set<T> const &, S const & s) { return cardinality(s) != 0; }

template <typename T, typename S>
bool operator==(empty_set<T> const &, S const & s)
{
    return cardinality(s) == 0;
}

}