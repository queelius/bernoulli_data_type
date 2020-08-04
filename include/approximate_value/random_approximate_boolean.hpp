/**
 * There are only two partitions of bool, {{0},{1}} and {{0,1}},
 * corresponding to first and second-order random approximate
 * Booleans.
 */

#pragma once

#include <random>


template <int N, typename T>
struct random_approximate {};

/**
 * This is the maximum uncertainty random approximate value for type T.
 * We denote the order by -1, since its order is however many unique elements
 * there are.
 * 
 * Make another one for the entropy coder?
 * Make anothr one for pdf p1, p2, ..., p3 with error rate a function of
 *     the p values?
 */
template <typename T>
struct random_approximate<-1,T>
{
    using value_type = T;
    constexpr int order = -1;

    auto error() const { return 1. - 1. / size(values<T>()); }
    auto fnr() const { return error(); }
    auto fpr() const { return error(); }

    template <typename I>
    static auto size(pair<I,I> xs) { return distance(xs.first,xs.second)};

    template <typename R>
    auto sample(R & r)
    {
        return approximate<-1,T>(values<T>().first +
            uniform_int_distribution(0,size(xs))(r));
    }
};

/**
 * This is the degenerate case, a random approximate value type T of order 0,
 * which is equivalent to a value type T.
 */
template <typename T>
struct random_approximate<0,T>
{
    using value_type = T;
    constexpr int order = 0;

    T given;

    auto fpr() const { return 0; }
    auto fnr() const { return 0; }
    auto error() const { return 0; }

    auto operator()() const { return given; }

    template <typename R>
    auto sample(R &) { return approximate<0,T>{given}; }   
};

/**
 * Conditional random Boolean variable
 * that models the concept of a first-order
 * random approximate Boolean.
 */
template <>
struct random_approximate<1,bool>
{
    using value_type = bool;
    constexpr int order = 1;

    double epsilon;
    bool given;

    auto fpr() const { return epsilon; }
    auto fnr() const { return epsilon; }
    auto error() const { return epsilon; }

    auto operator()() const { return given; }

    template <typename R>
    auto sample(R & r)
    {
        bernoulli_distribution ber(epsilon);
        if (given)
            return approximate<1,bool>{epsilon, (bool)(1-(int)ber(r))};
        else
            return approximate<1,bool>{epsilon, ber(r)};
    }
};

template <int N>
auto operator|(approximate<N,bool> a, approximate<N,bool> b)
{
    // a + b = (a' b')'
    return ~((~a) & (~b))
}

template <int N>
auto nor(approximate<N,bool> a, approximate<N,bool> b)
{
    return ~(a | b);
}

template <int N>
auto nand(approximate<N,bool> a, approximate<N,bool> b)
{
    return ~(a & b);
}

template <int N>
auto operator^(approximate<N,bool> a, approximate<N,bool> b)
{
    return (~a & b) | (a & ~b);
}


template <>
struct random_approximate<2,bool>
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
            std::bernoulli_distribution ber(fn);
            return approximate<1,bool>{fn, 1-ber(r)};
        }
        else
        {
            std::bernoulli_distribution ber(fp);
            return approximate<1,bool>{fp, ber(r)};
        }
    }
};

template <size_t N, typename T>
struct random_approximate_pdf {};

template <typename T>
struct random_approximate_pdf<0,T>
{
    using input_type = T;
    using output_type = double;

    random_approximate<0,T> b;

    auto operator()(T const & x) const
    {
        return x == b.given ? 1. : 0.;
    }
};


/**
 * The probability density function for first-order random approximate
 * Boolean values, which is a distriubtion where the probability of
 * an error is a fixed value epsilon.
 */
template <>
struct random_approximate_pdf<1,bool>
{
    using input_type = bool;
    using output_type = double;

    random_approximate<1,bool> b;

    auto operator()(bool x) const
    {
        return x == b.given ? 1 - b.epsilon : b.epsilon;
    }
};

/**
 * The probability density function for first-order random approximate
 * Boolean values, which is a distriubtion where the probability of
 * an error is a fixed value epsilon.
 */
template <>
struct random_approximate_pdf<2,bool>
{
    using input_type = bool;
    using output_type = double;

    random_approximate<2,bool> b;

    auto operator()(bool x) const
    {
        if (b.given)
            return x ? 1 - b.fnr : b.fnr;
        else
            return x ? b.fpr : 1 - b.fpr;
    }
};

template <int N, typename T>
auto pdf(random_approximate<N,T> x)
{
    return random_approximate_pdf<N,T>{x};
}


/**
 * The first-order random approximate Boolean is a straightforward monad.
 * 
 * As a function of a random variable, f : bool -> bool is a random
 * variable. Since we apply f to a first-order random approximate Boolean,
 * the output is a first-order random approximate Boolean in which the
 * given value is f(x()).
 * 
 * It is nocomputation in the context of a first-order approximation error
 * on boolean values.
 * 
 * This is the simplest type. random_approximate<unit> and
 * random_approximate<void>, where void denotes the absurd type which has
 * no values, are equivalent to unit and void.
 * 
 */
auto fmap(function<bool(bool)> f, random_approximate<1,bool> x)
{
    return random_approximate<1,bool>(f(x()),x.epsilon);
}

auto fmap(function<bool(bool)> f, random_approximate<2,bool> x)
{
    // return random_approximate<2,bool>(f(x.given),x.fpr,x.fnr);
    return;
}    

template <typename N>
auto fmap(function<void(void)> f, random_approximate<N,void> x)
{
    f();
    return random_approximate<N,void>();
}    


auto fmap(function<bool(bool)> f, approximate<1,bool> x)
{
    
    return approximate<1,bool>{x()};
}

auto fmap(function<bool(bool)> f, approximate<0,bool> x)
{
    
    return approximate<0,bool>{x()};
}











