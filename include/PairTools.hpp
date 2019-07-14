#pragma once

namespace alex
{
    template <typename X, typename Y>
    X fst(std::pair<X,Y> const & p)
    {
        return p.first;
    };

    template <typename X, typename Y>
    X fst(std::tuple<X,Y> const & p)
    {
        return std::get<0>(p);
    };

    template <typename X, typename Y>
    Y snd(std::pair<X,Y> const & p)
    {
        return p.second;
    };

    template <typename X, typename Y>
    Y snd(std::tuple<X,Y> const & p)
    {
        return std::get<1>(p);
    };
}