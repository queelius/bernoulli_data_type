#pragma once
#include "../approximate_value/first_order_random_approximate_value.hpp"
#include <vector>
#include <array>
#include <optional>
#include <random>
#include <set>
using std::vector;
using std::optional;
using std::array;

/**
 * The second-order random approximate set in which the
 * positive elements have an error rate fnr and the
 * negative elements have an error rate fpr is equivalent to
 * a vec_approximate_bool in which every true Boolean element
 * has an error rate fpr and every false Boolean element
 * has an error rate fnr.
 * 
 * We can use the Boolean algebra to derive other higher-order
 * random approximate sets.
 */

vec_approximate_bool operator+(approximate_bool x, approximate_bool y)
{
    return vec_approximate_bool{x,y};
}

vec_approximate_bool operator+(vec_approximate_bool x, approximate_bool y)
{
    x.push_back(y);
    return x;
}

vec_approximate_bool operator+(approximate_bool x, vec_approximate_bool y)
{
    y.push_back(x);
    return y;
}

vec_approximate_bool operator+(vec_approximate_bool x, vec_approximate_bool y)
{
    x.insert(x.end(),y.begin(),y.end());
    return x;
}

optional<vec_approximate_bool> operator&(
    vec_approximate_bool a,
    vec_approximate_bool const & b)
{
    if (a.size() != b.size())
        return {};

    for (size_t i = 0; i < a.size(); ++i)
        a[i] = a[i] & b[i];
    return a;
}


optional<vec_approximate_bool> operator|(
    vec_approximate_bool a,
    vec_approximate_bool const & b)
{
    if (a.size() != b.size())
        return {};

    for (size_t i = 0; i < a.size(); ++i)
        a[i] = a[i] | b[i];
    return a;
}


vec_approximate_bool operator~(vec_approximate_bool a)
{
    for (auto & x : a)
        x = ~x;
    return a;
}

optional<vec_approximate_bool> operator^(
    vec_approximate_bool a,
    vec_approximate_bool const & b)
{
    if (a.size() != b.size())
        return {};

    for (size_t i = 0; i < a.size(); ++i)
        a[i] = a[i] ^ b[i];
    return a;
}

double fpr(vec_approximate_bool const & a)
{
    double f = 0;
    size_t p = 0;
    for (auto const & x : a)
    {
        if (x.value)
        {
            ++p;
            f += x.alpha;
        }
    }
    return f / p;
}

double fnr(vec_approximate_bool const & a)
{
    double f = 0;
    size_t n = 0;
    for (auto const & x : a)
    {
        if (x.value)
        {
            ++n;
            f += x.alpha;
        }
    }
    return f / n;
}

double error_rate(vec_approximate_bool const & a)
{
    double f = 0;
    for (auto const & x : a)
        f += x.alpha;
    return f / a.size();
}

int order(vec_approximate_bool const & a)
{
    std::set<double> alphas;
    for (auto const & x : a)
        alphas.insert(x.alpha);
    return alphas.size();
}

























/**
 * Consider the Boolean algebra
 *     (array<bool,N>, +, *, ~, array<bool,N>{false}, array<bool,N>{true}).
 * 
 * Note that
 *     (array<bool,N>, +, *, ~, array<bool,N>{false}, array<bool,N>{true})
 * is the same as
 *     (array<random_approximate<0,bool>,N>, +, *, ~, array<bool,N>{false}, array<bool,N>{true}).
 * 
 * However, we may approximate this Boolean algebra with a first-order random
 * approximate Boolean algebra
 *     (array<random_approximate<1,bool>,N>, +, *, ~, array<bool,N>{false}, array<bool,N>{true}).
 * 
 * Define
 *     contains : array<T,N> -> int -> bool
 * by
 *     contains(A,i) := A[i] == true.
 * 
 * Now, we may ask questions, such as what is the probability that
 *     contains(array<random_approximate<1,bool>,N>{A}, i)
 * is not equal to
 *     contains(array<bool,N>{A}, i)
 * for any i in the {0,...,N-1}? Assuming each first-order random approximate
 * Boolean has the same error rate E, then the answer is simply E.
 * 
 * What do we wish to call this random structure? It is a vector of N first
 * order random approximate Booleans, which models the concept of
 *     array<random_approximate<1,bool>,N>
 * with an error rate E on the contains function.
 * 
 * Note that it does not model the concept of
 *     random_approximate<1,array<bool,N>>
 * since this would apply to the entire value type of the vector, i.e.,
 * the error rate would be on the equality predicate ==. However, the
 * equality predicate also has an error rate for 
 *     array<random_approximate<1,bool>,N>,
 * but that is a lot more complicated.
 * 
 * We choose to provide a template specialization for this particular
 * type.
 */
template <typename T, size_t N>
struct first_order_random_approximate_vector {};

template <size_t N>
struct first_order_random_approximate_vector<bool,N>
{
    array<bool,N> given;
    double contains_error;

    auto contains(int x) const
    {
        return random_approximate<1,bool>{value[x],contains_error};
    }

    template <typename R>
    auto sample(R & r) const
    {
        std::bernoulli_distribution ber(contains_error);
        array<bool,N> s;
        for (size_t i = 0; i < N; ++i)
        {
            if (ber(r))
                s[i] = !given[i];
            else
                s[i] = given[i];
        }
        return first_order_approximate_vector<bool,N>{s,contains_error};
    }
};

template <size_t N>
struct first_order_approximate_vector<bool,N>
{
    array<bool,N> value;
    double contains_error;

    auto contains(int x) const
    {
        return approximate<1,bool>{value[x],contains_error};
    }

    auto operator==(first_order_approximate_vector<bool,N> const & rhs)
    {
        for (size_t i = 0; i < N; ++i)
        {
            // auto p = approximate<1,bool>{rhs.value[i]} == approximate<1,bool>{value[i]};
            // ???
        }
    }
};





