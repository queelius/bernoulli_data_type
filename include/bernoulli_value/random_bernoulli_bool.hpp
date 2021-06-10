#pragma once

/**
 * The first-order random approximate Boolean is a straightforward monad.
 * 
 * This is the simplest type. There are conceptually two simpler types,
 *     template <size_t N, typename T> // T models Singleton
 *     random_approximate
 * and
 *     template <size_t N>
 *     random_approximate<N,void>,
 * where void denotes the absurd type which has no values, are equivalent to
 * T and void, or alternatively, random_approximate<0,void>
 */
template <>
struct random_bernoulli<first_order,bool,eq>
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
        std::bernoulli_distribution ber(epsilon);
        if (given)
            return bernoulli<first_order,bool,eq>{epsilon, 1-ber(r)};
        else
            return bernoulliapproximate<1,bool>{epsilon, ber(r)};
    }
};


template <>
struct random_bernoulli<second_order<pos_neg>,bool,eq>
{
    using value_type = bool;
    constexpr int order = 1;

    double const error[2];
    bool const given;

    auto fpr() const { return error[0]; }
    auto fnr() const { return error[1]; }
    auto error(bool x) const { return x ? fpr() : fnr(); }

    auto operator()() const { return given; }

    template <typename U>
    auto sample(U & urbg)
    {
        if (given)
        {
            std::bernoulli_distribution ber(fnr());
            return bernoulli<second_order<pos_neg>,bool,eq>{error[0],error[1],1-ber(urbg)};
        }
        std::bernoulli_distribution ber(fpr());
        return bernoulli<second_order<pos_neg>,bool,eq>{error[0],error[1],1-ber(urbg)};
    }
};
