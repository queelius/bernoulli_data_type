#pragma once

#include <stdexcept>
#include <vector>
#include "bounded_nat.hpp"
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
class abelian_op
{
public:
    abelian_op(bounded_nat const & n, bounded_nat init) :
        n_(n),
        els_(n * n, init)
    {
        
    }

    bounded_nat cardinality() const { return bounded_nat(n_*n_); }

    bounded_nat const operator()(bounded_nat x, bounded_nat y) const
    {
        if (x >= n_ || y >= n_)
            throw invalid_argument("Not in the domain of definition.");
        return els_[max(x,y)*(max(x,y)+1)/2 + min(x,y)];
    }

    bounded_nat & operator()(bounded_nat x, bounded_nat y)
    {
        if (x >= n_ || y >= n_)
            throw invalid_argument("Not in the domain of definition.");
        return els_[max(x,y)*(max(x,y)+1)/2 + min(x,y)];
    }

    bounded_nat n() const { return n_; };

    operator const vector<bounded_nat> & () const { return els-; };

private:
    bounded_nat n_;
    vector<bounded_nat> els_;
};

bool is_commutative(abelian_op const & f)
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
bool is_group(abelian_op const & f)
{
    auto e = identity(f);
    if (!e || !is_associative())
        return false;


    
}

bool is_closed(abelian_op const &)
{
    return true;
}


bool invertible(abelian_op const & f)
{
    auto e = identity(f);
    if (!e)
        return false;

    for (bounded_nat x = 0; x < f.n(); x = x + 1)
    {
        for (bounded_nat y = 0; y <= x; y = y + 1)
        {
            if (f(x,y) == *e)
        }
    }
}



/**
 * f(f(x,y),z) = f(x,f(y,z)) for all x,y,z in {0,1,...,n-1}.
 */
bool is_associative(abelian_op const & f)
{
    for (bounded_nat i = 0; i < f.n(); ++i)
    {
        for (bounded_nat j = 0; j <= i; ++j)
        {
            for (bounded_nat k = 0; k < f.n(); ++k)
            {
                auto const a = f(i,j);
                auto const b = f(j,k);
                if (a >= f.n() || b >= f.n() || f(a,k) != f(i,b))
                    return false;
            }
        }
    }

    return true;
}

/**
 * true by definition of abelian operations, which abelian_op has as
 * an invariant.
 */
bool is_functional(abelian_op const &)
{
    return true;
}

/**
 * f(e,x) = f(x,e) = x for all x in {0,1,...,n-1}.
 */
optional<bounded_nat> identity(abelian_op const & f)
{
    for (bounded_nat e = 0; e < f.n(); e = e+1)
    {
        bool found = true;
        for (bounded_nat y = 0; y < f.n(); y=y+1)
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