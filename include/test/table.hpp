#pragma once

#include <stdexcept>
#include <vector>
#include "bounded_nat.hpp"

using std::invalid_argument;
using std::vector;

class op_table
{
public:
    table(bounded_nat const & nr, bounded_nat const & nc, bounded_nat) :
        cols_(nc.get()),
        els_(nr.get() * cols_, init)
    {
        
    }

    bounded_nat size() const { return els_.size(); }

    bounded_nat & operator()(bounded_nat i)
    {
        if (i > els_.size())
            throw invalid_argument("Index out of range.");

        return els_[i];
    }

    bounded_nat operator()(bounded_nat i) const
    {
        if (i > els_.size())
            throw invalid_argument("Index out of range.");

        return els_[i];
    }

    bounded_nat const operator()(bounded_nat r, bounded_nat c) const
    {
        if (c >= nc() || r >= nr())
            throw invalid_argument("Index out of range.");

        return els_[r * nc() + c];
    }

    bounded_nat & operator()(int r, int c)
    {
        if (c >= nc() || r >= nr())
            throw invalid_argument("Index out of range.");

        return els_[r * nc() + c];
    }

    bounded_nat nc() const { return cols_; };
    bounded_nat nr() const { return els_.size() / cols_; };

private:
    bounded_nat cols_;
    vector<bounded_nat> els_;
};

bool is_square(table const & t)
{
    return t.nr() == t.nc();
}

bool is_symmetric(table const & t)
{
    if (!is_square(t))
        return false;

    for (unsigned int i = 1; i < t.nr(); ++i)
    {
        for (unsigned int j = 0; j < i; ++j)
        {
            if (t(i,j) != t(j,i))
                return false;
        }
    }

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
bool is_group(table const & t)
{

}


/**
 * R(R(x,y),z) = R(x,R(y,z))
 */
bool is_associative(table const & t)
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
table<unsigned int> compose(
    table<unsigned int> const & t,
    table<unsigned int> const & u)
{
    for (int i = 0; i < t.nr(); ++i)
    {
        for (int j = 0; j < t.nc(); ++j)
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