#pragma once

#include <vector>
#include <cmath>
#include <functional>
#include <utility>

namespace bernoulli
{
    /**
     * disjoint_hash_map<D,X,H> models a function of type
     *     X -> bernoulli<D::value_type>
     * where D models the concept of a decoder functor of type
     *     ByteString -> D::value_type,
     * H models the concept of a random hash function object of type
     *     X -> ByteString 
     * which uniformly distributes over its codomain, and
     *     ByteString
     * models the concept of a sequence of bytes.
     * 
     * By default, H = md5_hash<X>.
     * 
     * A perfect hash function variation of this may be used to construct
     * similar maps quickly for much larger output domains by simply storing
     * values at the hash indexes that point to the value that the input maps
     * to. However, this is not, technically, a bernoulli map. It is a map, and
     * it still may have approximation errors, but it fails to be a Bernoulli
     * map for the simple reason that inputs not in the guard set will be mapped
     * to elements that are correlated with the values mapped to by inputs in
     * the guard set.
     */

    template <typename H, typename D>
    struct rd_hash_map_lvl2
    {
    public:
        using hash_fn = H;
        using decoder_fn = D;
        using output_type = typename D::value_type;

        auto get_hash_fn() const { return h_; }
        auto get_decoder_fn() const { return d_; }

        rd_hash_map_lvl2(rd_hash_map_lvl2 const &) = default;
        rd_hash_map_lvl2(rd_hash_map_lvl2 &&) = default;

        auto nbins() const { return l2_.size(); }

        template <typename X>
        auto operator()(X const & x)
        {
            auto l0 = h.mix(h(s0),x) % sigma.size(); // TODO: only store l0 = h(s0) instead of s0
            auto l1 = sigma[l0_h];
            auto hash = h.mix();
            return d(s);
        }

        /**
         * In the rate-distorted disjoint hash map, which models the concept
         * of a Bernoulli map, we can divide the error categories into two
         * general types, errors over the *guarded set* and errors over the
         * the rest.
         * 
         * Errors over the guard set result from failing to find a
         * representation. There is some non-zero probability that this occurs;
         * the greater the rate distortion, the greater the probability of
         * such errors.
         * 
         * Errors over the rest are a random distribution such that
         * the probability of no error for an element x not in the guard
         * set is given by
         *     t(x) := sum (2^-|b| : b in codes(f(x)))
         * where codes : X -> {0,1}^* is a prefix-free coder. Thus, the
         * probabiliity of an error is e = 1-t.
         * 
         * Normally, each value has one prefix free code, and thus t
         * simplifies to
         *     t := 2^-|f(x)|.
         * 
         * The distribution of errors over the guard set is, apriori, random,
         * and follows a Bernoulli distribution for each x in the guard set.
         * However, once constructed, the number of errors may be observed
         * and thus the error rate is just
         *     #(errors over guard set) / #(guard set).
         */
        auto guarded_error_rate() const { return err; }

        /**
         * @brief Expected errors over the unguarded set
         * 
         * If value type X is finite, then the unguarded error rate may be
         * observed as in the errors over the finite guarded set. We assume that
         * X is sufficiently large such that observing the error rate is
         * impractical.
         * 
         * Instead, we return the expected unguarded error rate, which is a
         * function of the implicit probability of x provided by the decoder.
         * 
         * Example: if we use a bool decoder where we assign
         * true with an implicit probability of 0.5,
         * then the unguarded error rate of true is 0.5 and the
         * unguarded error rate of false is 0.5.
         * 
         * typically, what we do is assign false an unguarded error rate with a
         * very low implicit probability, say .001, and then we only include
         * the positive elements in the guard set, which yields a false negative
         * rate that is a function of the rate distortion (ideally none, in
         * which case the false negative rate is zero) and a false positive rate
         * of 0.001.
         * 
         * we call this a positive Bernoulli set, which is in the same class
         * as the Bloom filter but may theoretically have less space
         * complexity if we used in-place compression, which we may
         * do since we know the theoretical distribution of the bin
         * counts.
         * 
         * @tparam X the generic value type that is hashable by H
         * @param x the value to compute the expected error rate for.
         */
        template <typename X>
        auto unguarded_error_rate(X const & x) const
        {
            return 1-d.implicit_probability(x);
        }

        D const d;
        H const h;
        std::vector<size_t> const sigma;
        size_t const s0;
        double const err;
    };
}

