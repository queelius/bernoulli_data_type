#pragma once

#include <vector>
#include <cmath>
#include <functional>
#include <utility>

namespace alex::hash
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

    template <
        typename D,
        typename X,
        typename H = std::hash<X>>
    class disjoint_hash_map
    {
    public:
        using hash_fn = H;
        using decoder_fn = D;
        using output_type = typename D::value_type;
        using intput_type = X;

        auto get_hash_fn() const { return h_; }
        auto get_decoder_fn() const { return d_; }

        disjoint_hash_map(disjoint_hash_map const &) = default;
        disjoint_hash_map(disjoint_hash_map &&) = default;

        auto nbins() const { return l2_.size(); }

        // I models a forward iterator of (x,y) pairs.
        template <typename I>
        disjoint_hash_map(
            I begin,
            I end,
            D d,
            H h,
            double r) :
                h_(h)
                d_(d),
                hs_(static_cast<size_t>(std::ceil(std::distance(begin,end)/r)))
        {
            using std::vector;
            using std::pair;

            auto const m = std::distance(begin,end);
            size_t min_block_errors = m;
            for (size_t s0 = 1; s0 != 0; ++s0)
            {
                vector<vector<pair<size_t,output_type>>> parts(nbins());
                for (auto xy = begin; xy != end; ++xy)
                {
                    auto s = s0_;
                    auto hx = h_(xy.first);
                    hash_combine(s, hx);
                    auto bin = s % nbins();
                    parts[bin].push_back(make_pair(hx,xy.second));
                }

                for (size_t block = 0; block < parts.size(); ++block)
                {
                    size_t block_errors = 0;
                    for (size_t bs = 1; bs != 0; ++bs)
                    {
                        size_t block_bs_errors = 0;
                        for (auto [hx,y] : parts[block])
                        {
                            auto s = s0_;
                            hash_combine(s,hx);
                            hash_combine(s,bs);
                            if (d_(s) != y)
                                ++block_bs_errors;
                        }

                        if (block_bs_errors > block_errors)
                        {
                            max_correct = correct;
                            s0_ = s0;

                        }
                    }
                }
            }
        }

        static void hash_combine(size_t & s, size_t h)
        {
            s ^= h + 0x9e3779b9 + (s << 6) + (s >> 2);
        }

        template <typename X>
        auto operator()(X const & x)
        {
            auto hash = h.mix(h(x),s0_);
            hash_combine(s, hs_[s % nbins()]);
            return d_(s);
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
        auto guarded_error_rate() const { return err_; }

        /**
         * Errors over the non-guarded set.
         * 
         * Technically, if X is finite, then the error rate may be observed as
         * in the errors over the guard set. However, we assume that X is
         * sufficiently large such that observing the error rate is impractical.
         * 
         * Instead, we return the expected unguarded error rate.
         */
        auto unguarded_error_rate(input_type const & x) const
        {
            return 1. - d_.implicit_probability(x);
        }

    private:
        D d_;
        H h_;
        std::vector<size_t> hs_;
        size_t s0_;

    };


    struct bool_decoder
    {
        using value_type = bool;

        auto implicit_probability(bool x)
        {
            static auto p = (double)k / std::numeric_limits<size_t>::max();
            return x ? p : 1-p;
        }

        bool_decoder(bool_decoder const &) = default;
        bool_decoder() : bool_decoder(0.5) {}
        bool_decoder(size_t k) : k(k) {}


        auto operator()(size_t code) const { return code <= k; }
        size_t const k;
    };

 
     /**
     * disjoint_hash_set<H> models a set indicator function of type
     *     H::domain_type -> bool
     * where H is a hash function of type
     *     H::domain_type -> size_t
     */
    template <typename H>
    using disjoint_hash_set = disjoint_hash_map<H, bool_decoder>;

    template <typename H>
    auto unguarded_error(disjoint_hash_set<H> const & dhs)
    {
        return dhs.error_rate();
    }

    template <typename H>
    auto guarded_error(disjoint_hash_set<H> const & dhs)
    {
        return dhs.guarded_error_rate();
    }
}
