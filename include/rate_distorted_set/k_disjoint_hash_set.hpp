#pragma once

#include <unordered_set>
#include <cmath>

namespace alex::hash
{
    /**
     * disjoint_hash_set<H,E,Y> models a set indicator function of type
     * H::domain -> Y where H is a hash function of type
     *     H::domain -> size_t
     */

    template <typename H>
    class disjoint_hash_set
    {
    public:
        using codomain = bool;
        using argument_type = std::declval<H>{}();

        disjoint_hash_set(disjoint_hash_set const &) = default;
        disjoint_hash_set(disjoint_hash_set &&) = default;

        auto nbins() const { return l2_.size(); }

        // I models a forward iterator
        template <typename I>
        disjoint_hash_set(
            I begin,
            I end,
            double fpr,
            double r) :
                K_(fpr * std::numeric_limits<size_t>::max()),
                r_(r),
                hs_(static_cast<size_t>(std::ceil(std::distance(begin,end)/r)))
        {
            for (s0_ = 0; ; ++s0_)
            {
                size_t fpn = 0;
                std::vector<std::vector<X>> partition(nbins());

                for (auto x = begin; x != end; ++x)
                {
                    auto s = h_(x);
                    auto bin = s % nbins();
                    partition[bin]
                }


                    hs_[bin] + 0x9e3779b9 + (s << 6) + (s >> 2)) < K_;

            }
        }

        auto operator()(X const & x)
        {
            auto s = std::hash<X>{}(x);
            auto bin = s % nbins();
            return (hs_[bin] + 0x9e3779b9 + (s << 6) + (s >> 2)) < K_;
        }

        /**
         * False negative rate.
         * 
         * In the rate-distorted disjoint hash set, which models the concept
         * of a Bernoulli set, the false positive rate has an apriori
         * distribution as a function of the specified false positive rate and
         * bit length (rate distortion). Once constructed, however, the false
         * negative rate may be observed.
         */
        auto fnr() const { return fnr_; }

        /**
         * False positive rate.
         * 
         * Technically, if X is finite, then the false positive rate may be
         * observed. However, we assume that X is sufficiently large that
         * observing the false positive rate is not practical or even
         * impossible if X is countably infinite.
         * 
         * Instead, we return the expected false positive rate. (In fact, the
         * number of false positives is binomially distributed
         *     FP = BIN(fpr(),n)
         * where n := cardinality of negative set. Thus, the false positive rate
         * is a random variable distribute as
         *     FPR := (1/n) * FP.
         */
        auto fpr() const { return (double)K_ / std::numeric_limits<size_t>::max(); }

    private:
        H h_;
        size_t K_;
        std::vector<size_t> hs_;
        size_t s0_;
    };
}