#include "bernoulli.hpp"
#include "random_bernoulli.hpp"

namespace bernoulli
{

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

}