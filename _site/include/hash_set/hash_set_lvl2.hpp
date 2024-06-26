#pragma once

#include <cmath>
#include <limits>
#include <vector>

namespace bernoulli
{
    /**
     * @brief Models the concept of a bernoulli set.
     *
     * hash_set<H> models a bernoulli set of type
     *     contains : Hashable(H) -> bool
     * where H is a hash function of type
     *     Hashable(H) -> size_t.
     * 
     * @tparam H hash function type
     */
    template <typename H>
    struct hash_set_lvl2
    {
        using hash_fn_type = H;
        using hash_type = typename H::hash_type;

        hash_set_lvl2(hash_set_lvl2 const &) = default;
        hash_set_lvl2(hash_set_lvl2 &&) = default;
        hash_set_lvl2(
          size_t N,
          H h,
          size_t s0,
          std::vector<size_t> sigma,
          double fnr) :
            N(N), h(h), s0(s0), fnr(fnr), sigma(sigma) {}

        /**
         * @brief retrieves the minimum hash value
         * @tparam X the element type, must be hashable by H.
         * @param x the element to retrieve the hash of.
         */

        template <typename X>
        auto contains(X const & x) const
        {
          return h.mix(h(sigma[h.mix(h(s0),x)]),x) <= N;
        }

        /**
         * @brief retrieves the false positive rate.
         * 
         * The false positive rate is defined as the probability that an
         * element in the complement of the set tests as being a member of the
         * set.
         */
        auto false_positive_rate() const
        {
            return (double)N / std::numeric_limits<size_t>().max();
        }

        /**
         * @brief retrieves the false negative rate.
         * 
         * The false negative rate is defined as the probability that an
         * element in the set tests as not being a member of the set.
         */
        auto false_negative_rate() const { return fnr; }

        /**
         * @brief retrieves the hash function that is used by phf
         */
        auto hash_fn() const { return h; }

        size_t const N;
        H const h;
        size_t const s0;
        double const fnr;
        std::vector<size_t> sigma;
    };
}
