#pragma once

#include <cstddef>
#include "set_container.hpp"
#include "universal_set.hpp"
#include "empty_set.hpp"

namespace alex::set::iterable
{
template <
    typename S,
    typename I = std::size_t,
    template <typename> typename C = Set>
class PowerSet
{
public:
    using set_type = S;
    using value_type = C<typename S::value_type>;
    using cardinality_type = I;

    PowerSet(S const & s) : _s(s) {};

    bool contains(value_type const & x) const
    {
        return subset_equal(x, _s);
    };

    I cardinality() const
    {
        return static_cast<I>(1) << _s.cardinality();
    };

    bool operator<(PowerSet<S,I,C> const & a) const
    {
        return cardinality() < a.cardinality();
    };

    bool operator==(PowerSet<S,I,C> const & a) const
    {
        return cardinality() == a.cardinality();
    };

    struct const_iterator
    {
        I index;
        S const & s;

        value_type operator*()
        {
            auto tmp = index;
            value_type result;
            for (auto const & x : s)
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
            return const_iterator{index+1,s};
        };

        bool operator==(const_iterator const & other)
        {
            return index == other.index;
        };

        bool operator!=(const_iterator const & other)
        {
            return index != other.index;
        };

        bool operator<(const_iterator const & other)
        {
            return index < other.index;
        };
    };

    const_iterator cbegin() const
    {
        return const_iterator{0,_s};
    };

    const_iterator begin() const
    {
        return const_iterator{0,_s};
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

template <typename S, typename I>
auto make_power_set(UniversalSet<S,I> const & u)
{
    return PowerSet<S>(static_cast<S>(u));
};

template <typename T, typename I>
auto make_power_set(EmptySet<T,I> e)
{
    return Set<EmptySet<T,I>>{ e };
};

template <typename S>
auto make_power_set(S s)
{
    return PowerSet<S>(std::move(s));
};

}