#pragma once

#include <cstddef>
#include "frozen_universal_set.hpp"
#include "frozen_empty_set.hpp"
#include <set>

namespace alex::set::iterable
{
template <
    typename S,
    template <typename> typename C = std::set>
class frozen_iterable_power_set
{
public:
    using set_type = S;
    using value_type = C<typename S::value_type>;
    using cardinality_type = size_t;

    frozen_iterable_power_set(S const &s) : _s(s){};

    bool contains(value_type const &x) const
    {
        return subset(x, _s);
    };

    cardinality_type cardinality() const
    {
        return static_cast<cardinality_type>(1) << _s.cardinality();
    };

    bool operator==(frozen_iterable_power_set<S, C> const &a) const
    {
        return cardinality() == a.cardinality() &&
               _s == a._s;
    };

    struct const_iterator
    {
        I index;
        S const &s;

        value_type operator*()
        {
            auto tmp = index;
            value_type result;
            for (auto const &x : s)
            {
                if (tmp % 2 == 0)
                    result.insert(x);
                tmp /= 2;
            }
            return result;
        };

        I operator-(const_iterator lhs) const
        {
            return index - lhs.index;
        };

        const_iterator operator+=(I diff)
        {
            index += diff;
            return *this;
        };

        const_iterator operator-=(I diff)
        {
            index -= diff;
            return *this;
        };

        const_iterator operator++()
        {
            ++index;
            return *this;
        };

        const_iterator operator++(int)
        {
            return const_iterator{index + 1, s};
        };

        bool operator==(const_iterator const &other)
        {
            return index == other.index;
        };

        bool operator!=(const_iterator const &other)
        {
            return index != other.index;
        };

        bool operator<(const_iterator const &other)
        {
            return index < other.index;
        };
    };

    const_iterator cbegin() const
    {
        return const_iterator{0, _s};
    };

    const_iterator begin() const
    {
        return const_iterator{0, _s};
    };

    const_iterator cend() const
    {
        return const_iterator{cardinality(), _s};
    };

    const_iterator end() const
    {
        return const_iterator{cardinality(), _s};
    };

private:
    S _s;
};

template <typename S>
auto make_frozen_iterable_power_set(frozen_universal_set<S, I> const &u)
{
    return frozen_iterable_power_set<S>(static_cast<S>(u));
};

template <typename T>
auto make_frozen_iterable_power_set(empty_set<T> e)
{
    return empty_set<T>{e};
};

template <typename S>
auto make_frozen_iterable_power_set(S s)
{
    return frozen_iterable_power_set<S>(std::move(s));
};

} // namespace alex::set::iterable