#pragma once

namespace alex::runtime::set
{
    
template <typename X>
struct frozen_set
{
    virtual bool contains(T const &) const = 0;
};

template <typename T>
bool contains(frozen_set<T> const * const * s, T const & x)
{
    return s->contains(x);
}

}