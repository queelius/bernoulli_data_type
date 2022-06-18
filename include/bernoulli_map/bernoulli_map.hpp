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
     * @tparam H hash function type
     * @tparam D decoder type
     */
    template <typename H, typename D>
    struct bernoulli_map
    {
        using hash_fn_type = H;
        using hash_type = typename H::hash_type;
        using codomain = typename D::value_type;

        bernoulli_map(bernoulli_map const &) = default;
        bernoulli_map(bernoulli_map &&) = default;
        bernoulli_map(H h, D d, size_t l, double err) :
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
         * @brief retrieves the hash function
         */
        auto hash_fn() const { return h; }

        /**
         * @brief retrieves the decoder function
         */
        auto decoder_fn() const { return d; }

        /**
         * @brief retrieves the error rate.
         */
        auto error_rate() const { return err; }

        D const d;
        H const h;
        size_t const l;
        double const err;
    };
}
