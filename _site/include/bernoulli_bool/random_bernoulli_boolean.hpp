#pragma once

#include <random>


/**
 * This is the maximum uncertainty random approximate value for type T.
 * We denote the order by -1, since its order is however many unique elements
 * there are.
 * 
 * Make another one for the entropy coder?
 * Make anothr one for pdf p1, p2, ..., p3 with error rate a function of
 *     the p values?
 */
template <typename T, typename F>
struct random_bernoulli<first_order,T,F>
{
    using value_type = T;
    constexpr int order = -1;

    double const error_rate;
    T const & given;

    auto error(T const &) const
    {
        return error_rate;
    }

    static auto cardinality()
    {
        return distance(values<T>().begin(),values<T>().end())
    }

    template <typename U>
    auto sample(U & urbg)
    {
        bernoulli_distribution ber(error_rate);
        if (ber(urbg))
        {
            return bernoulli<first_order,T,F>(values<T>().first + uniform_int_distribution(0,size(xs))(urbg));
        }
        return bernoulli<first_order,T,F>(given);
    }
};




template <>
struct random_bernoulli<second_order<pos_neg>,bool,eq>
{
    using value_type = bool;
    constexpr int order = 2;

    double fp, fn;
    bool given;

    auto fpr() const { return fp; }
    auto fnr() const { return fn; }
    //auto error() const { return ?; }

    auto operator()() const { return given; }

    template <typename R>
    auto sample(R & r)
    {
        if (given)
        {
            std::bernoulli_distribution ber(1-fn);
            return bernoulli<2,bool>{fp,fn,ber(r)};
        }
        else
        {
            std::bernoulli_distribution ber(fp);
            return bernoulli<2,bool>{fp,fn,ber(r)};
        }
    }
};

template <typename T>
struct random_bernoulli_pdf {};

template <typename T>
struct random_bernoulli_pdf<bernoulli<0,T>>
{
    using input_type = bernoulli<0,T>;
    using output_type = double;

    random_bernoulli<0,T> b;

    auto operator()(bernoulli<0,T> const & x) const
    {
        return x.value == b.given ? 1. : 0.;
    }
};


/**
 * The probability density function for first-order random Bernoulli over
 * Boolean values.
 */
template <>
struct random_bernoulli_pdf<bernoulli<1,bool>>
{
    using input_type = bernoulli<1,bool>;
    using output_type = double;

    random_bernoulli<1,bool> b;

    auto operator()(bernoulli<1,bool> x) const
    {
        return x.value == b.given ? 1 - b.err : b.err;
    }
};

/**
 * The probability density function for first-order random approximate
 * Boolean values, which is a distriubtion where the probability of
 * an error is a fixed value epsilon.
 */
template <>
struct random_bernoulli_pdf<bernoulli<2,bool>>
{
    using input_type = bernoulli<2,bool>;
    using output_type = double;

    random_bernoulli<2,bool> b;

    auto operator()(bernoulli<2,bool> x) const
    {
        if (b.given)
            return x.value ? 1 - b.fnr : b.fnr;
        else
            return x.value ? b.fpr : 1 - b.fpr;
    }
};



template <typename X>
struct random_bernoulli_pdf<bernoulli_set<1,X>>
{
    using input_type = bernoulli_set<1,X>;

    // it may make sense to replace long double with, say, 
    using output_type = long double;

    random_bernoulli_set<1,X> b;

    // If values<X>() is infinite (e.g., lazily generates all natural numbers),
    // then this procedure never halts. a sample of values<X>() may be more
    // appropriate since even if values<T> is finite, it may be quite large.
    auto operator()(bernoulli_set<1,X> s) const
    {
        long double p = 1;
        for (auto x : values<X>())
            p *= s.contains(x) != b.given.contains(x) ? b.err : (1 - b.err);
        return p;
    }
};

template <typename X>
struct random_bernoulli_pdf<bernoulli_set<2,X>>
{
    using input_type = bernoulli_set<1,X>;
    using output_type = long double;

    random_bernoulli_set<2,X> b;

    // If values<X>() is infinite (e.g., lazily generates all natural numbers),
    // then this procedure never halts. a sample of values<X>() may be more
    // appropriate since even if values<T> is finite, it may be quite large.
    auto operator()(bernoulli_set<2,X> s) const
    {
        long double p = 1;
        for (auto x : values<X>())
        {
            if (b.given.contains(x))
                p *= s.contains(x) ? (1 - b.fnr) : b.fnr;
            else
                p *= s.contains(x) ? b.fpr : (1-b.fpr);
        }
        return p;
    }
};

template <int N, typename T>
auto pdf(random_bernoulli<N,T> x)
{
    return random_bernoulli_pdf<random_bernoulli<N,T>>{x};
}










