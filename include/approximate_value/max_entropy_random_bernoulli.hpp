#pragma once

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
struct uniform_random_bernoulli<-1,T>
{
    using value_type = T;
    constexpr int order = -1;

    auto error() const { return 1. - 1. / size(values<T>()); }
    auto fnr() const { return error(); }
    auto fpr() const { return error(); }

    static auto cardinality()
    {
        return distance(values<T>().begin(),values<T>().end())
    }

    template <typename R>
    auto sample(R & r)
    {
        return bernoulli<-1,T>(values<T>().first +
            uniform_int_distribution(0,size(xs))(r));
    }
};

