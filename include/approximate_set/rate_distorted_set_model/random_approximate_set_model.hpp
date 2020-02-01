#pragma once

#include <set>
#include <map>
#include <random>

/**
 * An illustrative implementation of the random approximate set model.
 * 
 * Implementations in general will be more sophisticated in how they
 * generate (efficient) implementations of random approximate sets. This
 * implementation is not concerned with efficiency. Its primary purpose is
 * to serve as a reference or to serve as a way to estimate sampling
 * distributions based off of random approximate sets.
 */

namespace alex::approximate_set
{ 
    template <typename T, typename Entropy = std::mt19937>
    class random_approximate_set_model
    {
    public:
        using value_type = T;
        using rate_type = decltype(std::declval<std::bernoulli_distribution>().p());

        template <typename I> // I models forward iterator
        random_approximate_set_model(
            I begin, I end,
            rate_type expected_fpr,
            rate_type expected_tpr,
            Entropy & entropy) :
                a_(begin,end),
                entropy_(entropy),
                fpr_(expected_fpr),
                tpr_(expected_tpr)
        {
        };
        
        bool contains(T const & x) const
        {
            if (ras_.count(x) == 0)
            {
                if (a_.count(x) == 0)
                    ras_[x] = fpr_(entropy_);
                else
                    ras_[x] = tpr_(entropy_);
            }

            return ras_[x];
        };

        rate_type expected_fpr() const
        {
            return fpr_.p();
        };

        rate_type expected_tpr() const
        {
            return tpr_.p();
        };

    private:
        std::set<T> const a_;
        mutable std::map<T,bool> ras_;
        mutable std::bernoulli_distribution fpr_;
        mutable std::bernoulli_distribution tpr_;
        mutable Entropy & entropy_;
    };

    template <typename T>
    auto false_positive_rate(random_approximate_set_model<T> const & a)
    {
        return alex::math::interval<double>(a.fpr(),a.fpr());
    };

    template <typename T>
    auto true_positive_rate(random_approximate_set_model<T> const & a)
    {
        return alex::math::interval<double>(a.tpr(),a.tpr());
    };
}