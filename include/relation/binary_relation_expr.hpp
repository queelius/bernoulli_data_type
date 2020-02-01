/**
 * Relational (binary) algebraic template expressions.
 */

#pragma once

#include <utility>
#include <boost/type_index.hpp>

using std::pair;
using std::move;
using std::make_pair;

namespace alex::relation
{
template <typename E, typename T, typename U>
struct binary_relation_expr
{
    using value_type = pair<T,U>;

    auto size() const
    {
        return static_cast<E const &>(*this).size();
    };

    auto dim() const
    {
        return static_cast<E const &>(*this).dim();
    };

    bool operator()(value_type const & x) const
    {
        return static_cast<E const &>(*this)(x);
    };

    auto operator()(T x, U y) const
    {
        return static_cast<E const &>(*this)(make_pair(move(x),move(y)));
    };

    auto empty() const
    {
        return static_cast<E const &>(*this).empty();
    };

    auto image(T const & x) const
    {
        return static_cast<E const &>(*this).image();
    };

    auto preimage(T const & x) const
    {
        return static_cast<E const &>(*this).preimage();
    };

    auto cbegin() const
    {
        std::cout << "cbegin()\n";
        auto const & r = static_cast<E const &>(*this);
        std::cout << "r cast\n";
        auto const & b = r.cbegin();
        std::cout << "r.cbegin()\n";
        auto name = boost::typeindex::type_id<E>().pretty_name();
        std::cout << name << "\n";

        auto name2 = boost::typeindex::type_id<typename E::value_type>().pretty_name();

        std::cout << name2 << "\n";

        auto name3 = boost::typeindex::type_id<typename E::value_type::first_type>().pretty_name();

        std::cout << name3 << "\n";

        auto name4 = boost::typeindex::type_id<typename E::value_type::second_type>().pretty_name();

        std::cout << name4 << "\n";
        
        //auto iter_name = boost::typeindex::type_id<decltype(b)>.pretty_name();
        return r.cbegin();
    };

    auto cend() const
    {
        std::cout << "end\n";
        auto r = static_cast<E const *>(this);
        std::cout << "now r\n";
        return r->cend();
    };
};

template <typename E1, typename E2, typename T, typename U>
struct binary_relation_union_expr:
    public binary_relation_expr<binary_relation_union_expr<E1,E2,T,U>,T,U>
{
    binary_relation_union_expr(E1 const & l, E2 const & r) : l(l), r(r) {};

    E1 const & l;
    E2 const & r;

    auto cbegin() const
    {
        return static_cast<E1 const &>(*this).cbegin();
    };

    auto cend() const
    {
        return static_cast<E1 const &>(*this).cend();
    };

    bool operator()(T const & x) const { return l(x) || r(x); };
};

template <typename E>
auto binary_relation(E const & r)
{
    using T = typename E::value_type::first_type;
    using U = typename E::value_type::second_type;

    return binary_relation_expr<E,T,U>(r);
};

template <typename E1, typename E2>
auto binary_relation_union(E1 const & l, E2 const & r)
{
    using T = typename E1::value_type::first_type;
    using U = typename E1::value_type::second_type;

    return binary_relation_union_expr<E1,E2,T,U>(l,r);
};




/*

template <typename E, typename T, typename U>
struct binary_relation_complement_expr: public binary_relation_expr<complement_expr<E,T,U>>
{
    complement_expr(E const & r) : r(r) {};

    E const & r;

    bool operator()(value_type const & x) const
    {
        return !r(x);
    };
};

template <typename E1, typename E2, typename T, typename V>
struct composition_expr: public expr<composition_expr<E,T,V>>
{
    composition_expr(E1 const & l, E2 const & r) : l(l), r(r) {};

    E1 const & l;
    E2 const & r;

    // todo: include the forward iterator type-erasure since composition
    // requires iterators for E1 and E2 relation expressions. Then, we can
    // implement operator()(value_type) through that interface.

    bool operator()(value_type const & p) const
    {
        for (auto y : image(l, p.first))
            if (contains(image(r, y), p.second)) return true;
        return false;
    };
};

template <typename E>
auto complement(E const & r)
{
    using p = typename E::value_type;
    using left_type = p::first_type;
    using right_type = p::second_type;

    return complement_expr<E,left_type,right_type>(r);
};

template <typename E1, typename E2>
auto composition(E1 const & l, E2 const & r)
{
    using lp = typename E1::value_type;
    using rp = typename E2::value_type;
    using left_type = lp::first_type;
    using middle_type = lp::second_type;
    using right_type = rp::second_type;
    // is_same_v<lp::second_type,rp::first_type>

    return composition_expr<E1,E2, left_type, right_type>(l,r);
};


template <typename E1, typename E2, typename T>
struct join_expr: public expr<join_expr<E1,E2,T>,T>
{
    join_expr(E1 const & l, E2 const & r) : l(l), r(r) {};

    E1 const & l;
    E2 const & r;

    bool operator()(T const & x) const { return l(x) || r(x); };
};

template <typename E1, typename E2>
auto join(E1 const & l, E2 const & r)
{
    using T = typename E1::value_type;

    return join_expr<E1,E2,T>(l,r);
};

template <typename E1, typename E2, typename T>
struct meet_expr: public expr<meet_expr<E1,E2,T>,T>
{
    meet_expr(E1 const & l, E2 const & r) : l(l), r(r) {};

    E1 const & l;
    E2 const & r;

    bool operator()(T const & x) const { return l(x) || r(x); };
};

*/

}