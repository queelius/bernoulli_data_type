#pragma once

#include <cstddef>
#include "universal_set.hpp"

namespace alex::set::iterable
{
template <typename L, typename R, typename I = std::size_t>
class CartesianProduct
{
public:
    using left_value_type = typename L::value_type;
    using right_value_type = typename R::value_type;
    using value_type = std::pair<left_value_type, right_value_type>;
    using cardinality_type = I;

    CartesianProduct(L l, R r) : l(l), r(r){};

    size_t cardinality() const { return l.cardinality() * r.cardinality(); };

    bool contains(value_type const &p) const
    {
        return l.contains(p.first) && r.contains(p.second);
    };

    bool contains(left_value_type const &a, right_value_type const &b) const
    {
        return l.contains(a) && r.contains(b);
    };

    struct const_iterator
    {
        using left_const_iterator = typename L::const_iterator;
        using right_const_iterator = typename R::const_iterator;

        left_const_iterator cur_l;
        right_const_iterator cur_r;

        left_const_iterator l_begin;
        left_const_iterator l_end;
        right_const_iterator r_end;

        const_iterator(
            left_const_iterator cur_l,
            right_const_iterator cur_r,
            left_const_iterator l_begin,
            left_const_iterator l_end,
            right_const_iterator r_end) : cur_l(cur_l), cur_r(cur_r), l_begin(l_begin), l_end(l_end), r_end(r_end){};

        value_type operator*()
        {
            return std::make_pair(*cur_l, *cur_r);
        };

        const_iterator operator++()
        {
            ++cur_l;
            if (cur_l == l_end)
            {
                ++cur_r;
                if (cur_r != r_end)
                    cur_l = l_begin;
            }
            return *this;
        };

        bool operator==(const_iterator const &other)
        {
            return cur_l == other.cur_l && cur_r == other.cur_r;
        };

        bool operator!=(const_iterator const &other)
        {
            return cur_l != other.cur_l || cur_r != other.cur_r;
        };
    };

    const_iterator cbegin() const
    {
        return const_iterator(l.cbegin(), r.cbegin(), l.cbegin(), l.cend(), r.cend());
    };

    const_iterator begin() const
    {
        return const_iterator(l.cbegin(), r.cbegin(), l.cbegin(), l.cend(), r.cend());
    };

    const_iterator cend() const
    {
        return const_iterator(l.cend(), r.cend(), l.cbegin(), l.cend(), r.cend());
    };

    const_iterator end() const
    {
        return const_iterator(l.cend(), r.cend(), l.cbegin(), l.cend(), r.cend());
    };

private:
    L l;
    R r;
};

template <typename S, typename I, typename B>
auto make_cartesian_product(UniversalSet<S, I> const &s, B const &b)
{
    return make_cartesian_product(static_cast<S>(s), b);
};

template <typename S1, typename S2, typename I1, typename I2>
auto make_cartesian_product(UniversalSet<S1, I1> const &a, UniversalSet<S2, I2> const &b)
{
    return make_cartesian_product(static_cast<S1>(a), static_cast<S2>(b));
};

template <typename A, typename S, typename I>
auto make_cartesian_product(A const &a, UniversalSet<S, I> const &s)
{
    return make_cartesian_product(a, static_cast<S>(s));
};

template <typename A, typename B>
auto make_cartesian_product(A const &a, B const &b)
{
    return CartesianProduct<A, B, std::size_t>(a, b);
};

} // namespace alex::set::iterable