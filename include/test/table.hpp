#pragma once

#include <stdexcept>
#include <vector>
#include "bounded_nat.hpp"

using std::invalid_argument;
using std::vector;

class binary_op_table
{
public:
    binary_op_table(bounded_nat const & n, bounded_nat init) :
        n_(n),
        els_(n * n, init)
    {
        
    }

    bounded_nat cardinality() const { return bounded_nat(els_.size()); }

    // naturally represents a function type (N,N)->N, but it also
    // provides a bijection between (N,N) and N, e.g.,
    // if f(x,y) = z, then f(p) = z where p is the encoding of (x,y),
    // which is just p = x*n+y. alternatively, if we want to see it 
    // as a unary function f : N -> N, but wish to map it to a
    // binary function, then if f(i) = j, f(i/n, i-floor(i/n)*n) = j.
    bounded_nat & operator()(bounded_nat i)
    {
        if (i > els_.size())
            throw invalid_argument("Index out of range.");

        return els_[i];
    }

    bounded_nat operator()(bounded_nat i) const
    {
        if (i > bounded_nat(els_.size()))
            throw invalid_argument("Index out of range.");

        return els_[i];
    }

    // let f be of type binary_op_table. then,
    // it represents an operation of type (bounded_nat, bounded_nat) -> bounded_nat,
    // where bounded_nat is the set of natural numbers {0,1,...,n-1}.
    bounded_nat const operator()(bounded_nat x, bounded_nat y) const
    {
        if (x >= n_ || y >= n_)
            throw invalid_argument("Not in the domain of definition.");

        return els_[x * n_ + y];
    }

    bounded_nat & operator()(bounded_nat x, bounded_nat y)
    {
        if (x >= n_ || y >= n_)
            throw invalid_argument("Not in the domain of definition.");

        return els_[x * n_ + y];
    }

    bounded_nat n() const { return n_; };

private:
    bounded_nat n_;
    vector<bounded_nat> els_;
};

bool is_commutative(binary_op_table const & f)
{
    for (bounded_nat x(1); x < f.n(); x = x + bounded_nat(1))
    {
        for (bounded_nat y(0); y < x; y = y + bounded_nat(1))
        {
            if (f(x,y) != f(y,x))
                return false;
        }
    }

    return true;
}

bool is_closed(binary_fn_table const & f)
{
    return all_of(
        vector<bounded_nat>(f).begin(),
        vector<bounded_nat>(f).end(),
        [](bouned_nat const & z) { return z < f.n(); };
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
bool is_group(binary_op_table const & t)
{

}


/**
 * R(R(x,y),z) = R(x,R(y,z))
 */
bool is_associative(binary_op_table const & t)
{
    if (!is_square(t))
        return false;

    unsigned int const N = t.nr();
    for (unsigned int i = 0; i < N; ++i)
    {
        for (I j = 0; j < N; ++j)
        {
            for (I k = 0; k < N; ++k)
            {
                auto const a = t(i,j);
                auto const b = t(j,k);
                if (a >= N || b >= N || t((I)a,k) != t(i,(I)b))
                    return false;
            }
        }
    }

    return true;
}


/**
 * let v = t . u. t()
 */
binary_op_table compose(
    binary_op_table const & t,
    binary_op_table const & u)
{
    for (bounded_nat i = 0; i < t.nr(); ++i)
    {
        for (bounded_nat j = 0; j < t.nc(); ++j)
        {
            auto k = t(i,j);
        }
    }
}


bool is_functional(table<int> t)
{
    // true by definition since each x,y corresponding to row,col has exactly
    // one entry.
    return true;
}



// /**
//  * R(e,x) = x for all x
//  */
// template <template <typename> typename Table>
// vector<int> left_identity(Table<int> const & t)
// {
//     if (!is_square(t))
//         return false;

//     vector<int> es;
//     int const N = t.nr();
//     for (int e = 0; e < N; ++e)
//     {
//         bool found = true;
//         for (int i = 0; i < N; ++i)
//         {
//             if (t(e,i) != i)
//             {
//                 found = false;
//                 break;
//             }
//         }
//         if (found)
//             es.push_back(e);
//     }
//     return es;
// }