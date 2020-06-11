#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <optional>
#include <tuple>
#include <stdexcept>
#include "table.hpp"

using std::pair;
using std::optional;
using std::tuple;
using std::make_tuple;
using std::sort;
using std::get;
using std::unique;
using std::vector;
using std::lower_bound;
using std::invalid_argument;

/**
 * For an input (x,y), returns z if (x,y,z) is a tuple value in the functional
 * relation.
 */
template <typename X, typename Y, typename Z>
class finite_partial_binary_fn
{
public:
    using domain = pair<X,Y>;
    using codomain = Z;
    using fn_relation = tuple<X,Y,Z>;

    struct total_order
    {
        bool operator()(fn_relation a, fn_relation b) const
        {
            if (get<0>(a) != get<0>(b))
                return get<0>(a) < get<0>(b);

            if (get<1>(a) != get<1>(b))
                return get<1>(a) < get<1>(b);

            return get<2>(a) < get<2>(b);
        }
    };

    struct mapping_partial_order
    {
        bool operator()(fn_relation a, fn_relation b) const
        {
            return get<0>(a) != get<0>(b) ?
                get<0>(a) < get<0>(b) :
                get<1>(a) < get<1>(b);
        }
    };

    struct eq
    {
        bool operator()(fn_relation a, fn_relation b) const
        {
            return get<0>(a) == get<0>(b) &&
                   get<1>(a) == get<1>(b) &&
                   get<2>(a) == get<2>(b);
        }
    };

    template <typename I>
    finite_partial_binary_fn(I first, I last) : els_(first, last)
    {
        sort(els_.begin(),els_.end(), total_order{});
        els_.erase(unique(els_.begin(),els_.end(), eq{}), els_.end());
    }

    optional<Z> operator()(X x, Y y) const
    {
        auto i = lower_bound(
            els_.begin(),
            els_.end(),
            make_tuple(x,y,Z{}),
            mapping_partial_order{});

        if (i == els_.end() || get<0>(*i) != x || get<1>(*i) != y)
            return {};
        return get<2>(*i);
    }

    auto cbegin() const { return els_.cbegin(); }
    auto cend() const { return els_.cend(); }
    auto begin() const { return els_.begin(); }
    auto end() const { return els_.end(); }

    auto cardinality() const { return els_.size(); };
    
private:
    vector<fn_relation> els_;
};

template <typename X,typename Y,typename Z>
auto dod(finite_partial_binary_fn<X,Y,Z> const & f)
{
    using domain = typename finite_partial_binary_fn<X,Y,Z>::domain;
    vector<domain> d;
    d.reserve(f.cardinality());
    for (auto const & r : f)
        d.push_back(domain(get<0>(r), get<1>(r)));
    return d;
}

template <typename X,typename Y,typename Z>
auto image(finite_partial_binary_fn<X,Y,Z> const & f)
{
    using codomain = typename finite_partial_binary_fn<X,Y,Z>::codomain;
    vector<codomain> img;
    img.reserve(f.cardinality());
    for (auto const & r : f)
    {
        img.push_back(codomain(get<2>(r)));
    }
    sort(img.begin(),img.end());
    img.erase(unique(img.begin(),img.end()),img.end());
    return img;
}

template <typename X,typename Y,typename Z>
bool is_symmetric(finite_partial_binary_fn<X,Y,Z> const & f)
{
    for (auto const & r : f)
    {
        auto fyx = f(get<1>(r),get<0>(r));
        if (!fyx || fyx != get<2>(r))
            return false;
    }
    return true;
}

template <typename X, typename Y, typename Z>
finite_partial_binary_fn<X,Y,Z> to_partial_binary_fn(
    table<unsigned int> const & t,
    vector<X> xs,
    vector<Y> ys,
    vector<Z> zs)
{
    if ((int)xs.size() < t.nr())
        throw invalid_argument("some integer to x mappings unspecified");
    if ((int)ys.size() < t.nc())
        throw invalid_argument("some integer to y mappings unspecified");

    vector<tuple<X,Y,Z>> fn_relation;
    fn_relation.reserve(t.nr() * t.nc());

    for (int i = 0; i < t.nr(); ++i)
    {
        for (int j = 0; j < t.nc(); ++j)
        {
            int k = t(i,j);
            if (k >= (int)zs.size())
                throw invalid_argument("some integer to z mappings unspecified");
            fn_relation.push_back(make_tuple(xs[i],ys[j],zs[k]));
        }
    }

    return finite_partial_binary_fn<X,Y,Z>(fn_relation.begin(), fn_relation.end());
}

template <typename X,typename Y,typename Z>
auto project_z(finite_partial_binary_fn<X,Y,Z> const & f)
{
    return image(f);
}

template <typename X,typename Y,typename Z>
vector<X> project_x(finite_partial_binary_fn<X,Y,Z> const & f)
{
    vector<X> xs;
    for (auto const & r : f)
        xs.push_back(get<0>(r));
    sort(xs.begin(),xs.end());
    xs.erase(unique(xs.begin(),xs.end()),xs.end());
    return xs;
}

template <typename X,typename Y,typename Z>
vector<Y> project_y(finite_partial_binary_fn<X,Y,Z> const & f)
{
    vector<Y> ys;
    for (auto const & r : f)
        ys.push_back(get<1>(r));
    sort(ys.begin(),ys.end());
    ys.erase(unique(ys.begin(),ys.end()),ys.end());
    return ys;
}

template <typename X, typename Y, typename Z>
tuple<vector<X>,vector<Y>,vector<Z>,table<unsigned int>> to_table(
    finite_partial_binary_fn<X,Y,Z> const & f)
{
    auto xs = project_x(f);
    auto ys = project_y(f);
    auto zs = project_z(f);

    sort(xs.begin(),xs.end());
    sort(ys.begin(),ys.end());
    sort(zs.begin(),zs.end());

    table<unsigned int> t((unsigned int)xs.size(), (unsigned int)ys.size());

    for (int i = 0; i < t.nr(); ++i)
    {
        for (int j = 0; j < t.nr(); ++j)
        {
            auto x = xs[i];
            auto y = ys[j];
            auto z = f(x,y);
            for (int k = 0; k < zs.size(); ++k)
            {
                if (zs[k] == z)
                {
                    t(i,j) = k;
                    break;
                }
            }
        }
    }

    return make_tuple(xs,ys,zs,t);
}



template <typename X,typename Y,typename Z>
bool is_commutative(finite_partial_binary_fn<X,Y,Z> const & f)
{
    return is_symmetric(get<3>(to_table(f)));
}

template <typename X,typename Y,typename Z>
bool is_associative(finite_partial_binary_fn<X,Y,Z> const & f)
{
    return is_associative(get<3>(to_table(f)));
}
