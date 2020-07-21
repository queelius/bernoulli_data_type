#pragma once
#include "../approximate_value/approximate_value.hpp"
#include "approximate_boolean.hpp"
#include <vector>
#include <optional>
#include <set>
using std::vector;
using std::optional;

/**
 * Approximate Booleans are the simplest case to consider.
 */

using vec_approximate_bool = vector<approximate_bool>;


/**
 * The first-order random approximate set is equivalent to
 * a vec_approximate_bool in which every Boolean element
 * has the same error rate.
 * 
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