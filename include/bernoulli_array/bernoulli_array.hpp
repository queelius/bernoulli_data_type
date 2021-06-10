#pragma once
#include <vector>
#include <array>
#include <optional>
#include <random>
#include <set>
using std::vector;
using std::optional;
using std::array;

/**
 * Consider the Boolean algebra
 *     (array<bool,N>, +, *, ~, array<bool,N>{false}, array<bool,N>{true})
 * and suppose we define
 *     contains : (array<T,N>,int) -> bool
 * as
 *     bool contains(A,i) { return i >= 0 && i < N && A[i]; }.
 * 
 * Then, if we replace the bool value type by the bernoulli<K,bool,eq> value type,
 * which is understood to be the a Bernoulli type over the equality
 * predicate for Boolean values, then, for instance,
 *     P[bernoulli<K,bool,eq>{x} == bool{x}] = E.
 * Thus, we have a Bernoulli approximation of the original Boolean algebra,
 *     (array<random_bernoulli<K,bool>,N>,
 *      |, &, ~,
 *      array<random_bernoulli<0,bool>,N>{false},
 *      array<random_bernoulli<0,bool>,N>{true}).
 * 
 * Naturally, since contains is defined with respect to equality on Boolean
 * values, the probability that
 *     contains(array<random_bernoulli<K,bool>,N>{A}, i)
 * is not equal to
 *     contains(array<bool,N>{A}, i)
 * for any i in the {0,...,N-1} is given by the distribution of error rates
 * on the contains. For instance, if we let K = 1, i.e., the first-order random
 * Bernoulli model with the same error rate E, then the answer is
 * simply E, i.e.,
 *     contains : (array<random_bernoulli<1,bool,E>,N>,int) ->
 *                random_bernoulli<1,bool,E>
 * where we took the liberty of parameterizing the error rate into the type
 * signature to make the distribution on the output explicit.
 * 
 * If we let K = 2 with the natural partition into positive and
 * negative elements such that the positive elements (A) have an error rate
 * denoted the false negative rate and the negative elements (complement(A))
 * have error rate denoted the false positive rate.
 * 
 * The *highest* order Bernoulli set over a univese of N elements is K == N,
 * e.g., an array
 *     array<random_bernoulli<1,bool>>
 * where every first-order Bernoulli Boolean of the array has a unique error
 * rate.
 * 
 * We denote this mathematical structure the Bernoulli set model of order K,
 *     bernoulli_set<K,X>,
 * in which X is partitioned into K blocks such that the j-th block has
 * Bernoulli error Ej over the contains predicate, i.e., if x in block i, then
 *     P[contains(bernoulli_set<K,X>{A},x) != contains(A,x)] == Ei.
 * 
 * Frequently, the expected error rates E[1], E[2], ..., E[K] are unknown, but
 * the expected false positive rate and false negative rate may be known or
 * estimated (either an interval or point estimate). Since we are
 * frequently not interested in the particulate error rates E[1], ..., E[K],
 * but instead the false positive and false negative rates, uncertainty over
 * E[1],...,E[K] may not be particularly important.
 * 
 * Most naturally, the order K = 2 with the natural partition of X into positive
 * and negative sets. We denote this the negative-positive Bernoulli set.
 * Algorithms that generate Bernoulli sets, such as the Bloom
 * filter, generate negative-positive Bernoulli sets with a false negative rate
 * 0 and a false positive rate 0 < E < 1. We consider this a special case
 * and denote it the positive Bernoulli set. The complemnt of the positive
 * Bernoulli set is the negative Bernoulli set.
 * 
 * Given a natural order K = 1 or K = 2, higher-order Bernoulli sets are then
 * the result of composing these Bernoulli sets, such as set-interection.
 * Moreover, as random sets, any function of these random sets is also random,
 * and in many cases the derived random element is another Bernoulli
 * approximation, e.g., the subset predicate
 *     subset : (
 *         array<random_bernoulli<K,bool>,N>,
 *         array<random_bernoulli<K,bool>,N>) -> random_bernouli<-1,bool>
 * maps to a random Bernoulli of some variable order (it depends upon the
 * number of )
 * 
 * 
 * Conversely, consider
 *     random_bernoulli<K,array<bool,N>>
 * over the equality predicate
 *     bernoulli<1,bool>
 *     operator==(
 *         random_bernoulli<K,array<bool,N>> lhs,
 *         array<bool,N> rhs).
 * 
 *     since this would apply to the entire value type of the vector, i.e.,
 * the error rate would be on the equality predicate ==. However, the
 * equality predicate also has an error rate for 
 *     array<random_approximate<1,bool>,N>,
 * but that is a lot more complicated.
 * 
 * We choose to provide a template specialization for this particular
 * type.
 */



// a bernoull type over vectors of type T for which
//
//     operator[](int) -> bernoulli<P,T,eq>
//
// models the bernoulli approximate.
//
// Observe that the equality predicate for this bernoulli type may be 
// approximated 

template <typename T>
struct seq;

template <typename P, typename T>
struct bernoulli<P,seq<T>,struct index>;

template <typename P, typename T>
using bernoulli_seq = bernoulli<P,seq<T>,struct index>


template <typeanme T>
struct bernoulli<first_order<universal<T>>,seq<T>,struct index>
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


/*
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
*/
























