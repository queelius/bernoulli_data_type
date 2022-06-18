#pragma once

#include <cmath>
#include <limits>

namespace bernoulli
{
    /**
     * @brief Models the concept of a bernoulli map.
     *
     * bernoulli_map<H,D> models a bernoulli map of type
     *     Hashable(H) -> ValueType(D)
     * where H is a hash function of type
     *     Hashable(H) -> size_t
     * and D models the concept of a prefix-free decoder.
     *
     * The error rate(s) are more complicated, since we do not
     * know generally what the decoder is doing. We can specialize
     * hash maps for specific decoders and provide more useful
     * error rates.
     * 
     * @tparam H hash function type
     * @tparam D decoder type
     */
    template <typename H, typename D>
    struct hash_map
    {
        using hash_fn_type = H;
        using hash_type = typename H::hash_type;
        using codomain = typename D::value_type;

        hash_map(hash_map const &) = default;
        hash_map(hash_map &&) = default;
        hash_map(H h, D d, size_t l, double err) :
            h(h), d(d), l(l), err(err) {}

        /**
         * @brief retrieves the minimum hash value
         * @tparam X the element type, must be hashable by H.
         * @param x the input
         * @returns the output associated with the input x
         */
        template <typename X>
        auto operator()(X const & x) const
        {
          return d(h.mix(h(x),l));
        }

        /**
         * @brief retrieves the error rate.
         */
        auto error_rate() const { return err; }

        /**
         * @brief retrieves the hash function
         */
        auto hash_fn() const { return h; }

        /**
         * @brief retrieves the decoder function
         */
        auto decoder_fn() const { return d; }

        H const h;
        D const d;
        size_t const l;
        double const err;
    };
}
