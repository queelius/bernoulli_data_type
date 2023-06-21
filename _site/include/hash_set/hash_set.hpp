<<<<<<< HEAD
#pragma once

#include <cmath>
#include <limits>

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
    struct hash_set
    {
        using hash_fn_type = H;
        using hash_type = typename H::hash_type;

        hash_set(hash_set const &) = default;
        hash_set(hash_set &&) = default;
        hash_set(size_t N, H h, size_t l0, double fnr) :
            N(N), h(h), l0(l0), fnr(fnr) {}

        /**
         * @brief Membership test
         * @tparam X the element type, must be hashable by H.
         * @param x the element to test membership for
         */

        template <typename X>
        auto operator()(X const & x) const
        {
          return h.mix(l0,x) <= N;
        }

        /**
         * @brief retrieves the false positive rate.
         * 
         * The false positive rate is defined as the probability that an
         * element in the complement of the set tests as being a member of the
         * set.
         */
        auto false_positive_rate() const { return (double)N / h.max(); }

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

        /**
         * @brief retrieves the hash family index
         * 
         * We consider the hash function of type H to denote a family
         * of hash functions. We choose a particular hash function in this
         * family by its index, which in this case corresponds to l0.
         */
        auto index() const { return l0; }

        size_t const N;
        H const h;
        size_t const l0;
        double const fnr;
    };
}
=======
#pragma once

#include <cmath>
#include <limits>

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
    struct hash_set
    {
        using hash_fn_type = H;
        using hash_type = typename H::hash_type;

        hash_set(hash_set const &) = default;
        hash_set(hash_set &&) = default;
        hash_set(size_t N, H h, size_t l0, double fnr) :
            N(N), h(h), l0(l0), fnr(fnr) {}

        /**
         * @brief retrieves the minimum hash value
         * @tparam X the element type, must be hashable by H.
         * @param x the element to retrieve the hash of.
         */

        template <typename X>
        auto contains(X const & x) const
        {
          return h.mix(l0,x) <= N;
        }

        /**
         * @brief retrieves the false positive rate.
         * 
         * The false positive rate is defined as the probability that an
         * element in the complement of the set tests as being a member of the
         * set.
         */
        auto false_positive_rate() const { return (double)N / h.max(); }

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

        /**
         * @brief retrieves the hash family index
         * 
         * We consider the hash function of type H to denote a family
         * of hash functions. We choose a particular hash function in this
         * family by its index, which in this case corresponds to l0.
         */
        auto index() const { return l0; }

        size_t const N;
        H const h;
        size_t const l0;
        double const fnr;
    };


     /**
     * hash_set<H> models a set indicator function of type
     *     Hashable(H) -> bool
     * where H is a hash function of type
     *     Hashable(H) -> size_t
     * template <typename H>
     * using rd_hash_set = rd_hash_map<H, bool_decoder>;
     *
     * template <typename H>
     * auto false_positive_rate(rd_hash_set<H> const & hs)
     * {
     *     return hs.unguarded_error_rate(false);
     * }
     * 
     * template <typename H>
     * auto false_negative_rate(rd_hash_set<H> const & hs)
     * {
     *     return hs.guarded_error_rate();
     * }
     */
}
>>>>>>> e479b9fe396c547b403344f8accd0a9b35d06062
