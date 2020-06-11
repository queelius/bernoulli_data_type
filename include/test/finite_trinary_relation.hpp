#pragma once

#include <vector>
#include <algorithm>
#include <tuple>
#include <utility>
#include <vector>
#include "set_view.hpp"

using std::merge;
using std::sort;
using std::tuple;
using std::pair;
using std::make_tuple;
using std::lower_bound;
using std::upper_bound;

// models trinary relations, with the ability to efficiently
// map (X, Y) -> 2^Z. this may occur if, for instance, we take the
// union of two functional relations (X, Y) -> Z, e.g.,
// suppose f : (X,Y)->Z and g : (X,Y)->Z and f(x,y) = a and g(x,y) = b for some
// (x,y) in (X,Y). Then, union(f,g) : (X,Y)->Z has union(f,g)(x,y) = {a,b}.
// g.
template <typename X, typename Y, typename Z>
struct finite_trinary_relation
{
    using domain = pair<X,Y>;
    using codomain = set_view<Z>;
    using relation = tuple<X,Y,Z>;

    struct total_order
    {
        bool operator<(relation a, relation b) const
        {
            if (get<0>(a) != get<0>(b))
                return get<0>(a) < get<0>(b);

            if (get<1>(a) != get<1>(b))
                return get<1>(a) < get<1>(b);

            return get<2>(a) < get<2>(b);
        }
    };

    struct eq
    {
        bool operator<(relation a, relation b) const
        {
            return get<0>(a) == get<0>(b) &&
                   get<1>(a) == get<1>(b) &&
                   get<2>(a) == get<2>(b);
        }
    };

    struct mapping_partial_order
    {
        bool operator<(relation a, relation b) const
        {
            return get<0>(a) != get<0>(b) ?
                get<0>(a) < get<0>(b) :
                get<1>(a) < get<1>(b);
        }
    };

    template <typename I>
    finite_trinary_relation(I first, I last) : els_(first, last)
    {
        sort(els_.begin(),els_.end(), total_order{});
        erase(unique(els_.begin(),els._end, eq{}), els_.end());
    }

    template <typename I>
    finite_trinary_relation(finite_partial_binary_fn<X,Y,Z> f) : els_(f.cbegin(), f.cend())
    {
    }

    auto cbegin() const { return els_.begin(); }
    auto cend() const { return els_.end(); }

    set_view<Z> operator()(X x, Y y)
    {
        auto l = lower_bound(
            els_.begin(),
            els_.end(),
            make_tuple(x,y,Z{}),
            mapping_partial_order{});

        auto u = upper_bound(
            l,
            els_.end(),
            make_tuple(x,y,Z{}),
            mapping_partial_order{});

        return set_view<Z>(l,u);
    }

    vector<relation> els_;
};


template <typename X, typename Y, typename Z>
auto make_union(
    finite_trinary_relation<X,Y,Z> const & f1,
    finite_trinary_relation<X,Y,Z> const & f2)
{
    auto l = merge(f1.cbegin(), f2.cbegin(), f1.cend(), f2.cend(),
        finite_trinary_relation<X,Y,Z>::total_order{});
    
    return finite_trinary_relation<X,Y,Z>(b, e);
}

