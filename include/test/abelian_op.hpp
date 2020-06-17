#pragma once

#include <stdexcept>
#include <vector>
#include "nat.hpp"
#include <algorithm>


/**
 * 0 1 3
 * 1 2 4
 * 3 4 5
 * 
 * 0 | 1 2 | 3 4 5
 * 
 * 0 -> (0,0)
 * 1 -> (1,0) ; (0,1)
 * 2 -> (1,1)       
 * 3 -> (2,0) ; (0,2)
 * 4 -> (2,1) ; (1,2)
 * 5 -> (2,2)           n(n+1)/2    n=2     2*3/2
 * 
 * 
 * (i,j) -> i(i+1)/2 + j
 * 
 * i=j -> i^2/2+i/2+i = i^2/2
 * 
 */

using std::invalid_argument;
using std::vector;
using std::max;
using std::min;
using std::all_of;

// a vector (x1,x2,...,xn), where xi in {0,1,...,m} and a natural number have a
// bijection. encode the vector as the natural number.


/**
 * abelian_op<K> is an abelian operation of type (N,N)->N where N is the set of natural
 * numbers {0,1,...,N}.
 */

template <unsigned int K>
class abelian_op
{
    static_assert(K>0);

public:
    abelian_op(nat<K> init) :
        els_(K*K, init)
    {
        
    }

    constexpr auto cardinality() const { return K*K; };

    nat<K> const & operator()(nat<N> const & x, nat<N> const & y) const
    {
        return els_[max((unsigned int)x,(unsigned int)y)*
            (max((unsigned int)x,(unsigned int)y)+1)/2+
            min((unsigned int)x,(unsigned int)y)];
    }

    nat<K> & operator()(nat<K> const & x, nat<K> const & y)
    {
        return els_[max((unsigned int)x,(unsigned int)y)*
            (max((unsigned int)x,(unsigned int)y)+1)/2+
            min((unsigned int)x,(unsigned int)y)];
    }

    constexpr auto k() const { return K; };

    operator const vector<nat<K>> & () const { return els_; };

private:
    vector<nat<K>> els_;
};

template <typename K>
bool is_commutative(abelian_op<K> const &)
{
    return true;
}

/**
 * (t, I), t : (I,I) -> I, is a group if:
 * 
 *      - there is an identity element e under t
 *          t(x,e) = t(e,x) = x
 * 
 *      - each x in I has a coresponding element x' in I such that
 *          t(x,x') = e
 *     - t(x',x) = e for every x in I
 * 
 */
template <typename K>
bool is_group(abelian_op<K> const & f)
{
    auto e = identity(f);
    if (!e || !is_associative())
        return false;
    return true;    
}

template <typename K>
bool is_closed(abelian_op<K> const &)
{
    return true;
}

template <typename K>
bool invertible(abelian_op<K> const & f)
{
    auto e = identity(f);
    if (!e)
        return false;

    for (nat<K> x = 0; x < K; ++x)
    {
        for (nat<K> y = 0; y <= x; ++y)
        {
            if (f(x,y) == *e)
        }
    }
}



/**
 * f(f(x,y),z) = f(x,f(y,z)) for all x,y,z in {0,1,...,n-1}.
 */
template <unsigned int K>
bool is_associative(abelian_op<K> const & f)
{
    for (nat<K> i = 0; i < K; ++i)
    {
        for (nat<K> j = 0; j <= i; ++j)
        {
            for (nat<K> k = 0; k < K; ++k)
                if (f(f(i,j),k) != f(i,f(j,k)))
                    return false;
        }
    }
    return true;
}

/**
 * true by definition of abelian operations, which abelian_op has as
 * an invariant.
 */
template <unsigned int K>
bool is_functional_relation(abelian_op<K> const &)
{
    return true;
}


/**
 * f(e,x) = f(x,e) = x for all x in {0,1,...,n-1}.
 */
template <unsigned int K>
optional<nat<K>> identity(abelian_op<K> const & f)
{
    for (auto e = nat<K>::min(); e < K; ++e)
    {
        bool found = true;
        for (auto y = nat<K>::min(); y < K; ++y)
        {
            if (f(e,y) != y)
            {
                found = false;
                break;
            }
        }
        if (found)
            return e;
    }

    return {};
}




/**
 * f|0 1 2
 * -+-----
 * 0|0 1 2
 * 1|1 a b
 * 2|2 b d
 * 
 * 3*3*3=27 possibilities
 * 
 * identity: f(e,a) = f(a,e) = a
 * let e = 0. a = 0 -> f(e,a) = f(a,e) = a = 0
 * 
 * 0 is identity
 * what makes 1 identity? f(0,1) = 1; f(1,a) = 1; f(2,a) = 2
 * 
 * f|0 1 2
 * -+-----
 * 0|0 1 2
 * 1|1 1 2
 * 2|2 2 b
 * 
 * what makes 2 identity? f()
 * 
 */