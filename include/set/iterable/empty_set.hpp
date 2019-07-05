#pragma once

#include <cstddef>

namespace alex::set::iterable
{

template <
    typename T,
    typename I = std::size_t
>
struct EmptySet
{
    using value_type = T;
    using cardinality_type = I;

    constexpr bool contains(T const &) const { return false; };
    constexpr I cardinality() const { return 0; };

    // EmptySet<T> is a singleton, i.e., there is only one
    // value of EmptySet<T>. The iterator is pretty straightforward
    // as a result, i.e., it is always at the end in the half-open
    // interval [).
    struct const_iterator
    {
        T operator*() { return T(); };
        const_iterator operator++() { return *this; };
        const_iterator operator++(int) { return *this; };
        bool operator==(const_iterator const &) { return true; };
        bool operator!=(const_iterator const &) { return false; };
    };

    const_iterator cbegin() const
    {
        return const_iterator();
    };

    const_iterator begin() const
    {
        return const_iterator();
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
    typename S,
    typename I>
bool subset_equal(EmptySet<T,I>, S const &)
{
    return true;
};

template <
    typename T,
    typename S,
    typename I>
bool equal(EmptySet<T,I>, S const & s)
{
    return s.cardinality() == 0;
};

}