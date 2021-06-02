#pragma once

#include <vector>
#include <cmath>
#include <functional>
#include <utility>

namespace alex::hash
{
    /**
     * disjoint_hash_map<D,H> models a function of type
     *     Hashable -> bernoulli<value_type<D>>
     * where Hashable is a concept that models all types T such that
     * specializations H<T> is defined.
     * 
     * Let hash be a type that models H must have a base case defined as
     * 
     * 
     * For instance if H<string> and H<int> are defined for
     * specializations of H, then disjoint_hash_map<D,H> models the
     * function 
     *     (string + int) -> bernoulli<typename D::value_type>
     * where + is the sum type operator.
     * 
     * D models the concept of a decoder functor of type
     *     ByteString -> D::value_type.
     * ByteString models the 
     * 
     * D is a template parameter that models a decoder function of type
     *     ByteString -> typename D::value_type
     * where a value that models a ByteString represents prefix-free
     * code in D. Note that the byte string
     * may be of any size, but some prefix of the byte string should
     * correspond to some prefix-free code for some y in Y.
     * 
     * H models the concept of a family of hash function functors.
     * If the family supports a type X -> ByteString
     * 
     * H is a template-template parameter such that if H<X> and H<Y> 
     * are defined for some X1 and X2, then disjoint_hash_map<H,D>
     * is a function of some type
     *     (X1+X2+...) -> bernoulli<typename D::value_type>.
     * 
     * By default, H = std::hash. Since many types are defined for std::hash,
     * disjoint_hash_map can deal with many different input types, theoretically,
     * out of the gate. In practice, they may not be that practical except for
     * relataively small output domains.
     * 
     * A perfect hash function variation of this may be used to construct similar maps
     * quickly for much larger output domains by simply storing values at the hash
     * indexes that point to the value that the input maps to.
     * 
     * However, this is not, technically, a bernoulli map. It is a map, and it still
     * may have approximation errors,* but it fails to be a Bernoulli map for the
     * simple reason that inputs not in the guard set will be mapped to elements
     * that are correlated with the values mapped to by inputs in the guard set.
     */

    template <typename D,
              typename H = std::hash>
    class disjoint_hash_map
    {
    public:
        using hash_fn = H;
        using decoder_fn = D;
        using output_type = typename D::value_type;

        auto get_decoder_fn() const { return d_; }

        disjoint_hash_map(disjoint_hash_map const &) = default;
        disjoint_hash_map(disjoint_hash_map &&) = default;

        auto nbins() const { return l2_.size(); }

        static void hash_combine(size_t & s, size_t h)
        {
            s ^= h + 0x9e3779b9 + (s << 6) + (s >> 2);
        }

        template <typename X>
        auto operator()(X const & x)
        {
            auto s = s0_;
            hash_combine(s, H<X>{}(x));
            hash_combine(s, hs_[s % nbins()]);
            return d_(s);
            //return bernoulli<output_type,2>{d_(s),guarded_error_rate(),unguarded_error_rate()};
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
         * Errors over the non-guard set.
         * 
         * Technically, if the domain is finite, then the error rate may be
         * observed as in the errors over the guard set. However, we
         * assume that the domain is large enough to make this impractical.
         * 
         * Instead, we return the expected unguarded error rate. (In fact, the
         * number of errors is binomially distributed
         *     FP = BIN(unguarded_errors(),n)
         * where n := cardinality of negative set. Thus, the unguarded error rate
         * is a random variable distribute as
         *     FPR := (1/n) * FP.
         */

        auto unguarded_error_rate() const { return (double)K_ / std::numeric_limits<size_t>::max(); }
        
        

    private:
        D d_;
        H h_;
        std::vector<size_t> hs_;
        size_t s0_;

    };


    struct bool_decoder
    {
        using value_type = bool;

        bool_decoder(bool_decoder const &) = default;
        bool_decoder() : bool_decoder(0.5) {}
        bool_decoder(double fpr) : p(fpr * std::numeric_limits<size_t>::max()) {}

        auto operator()(size_t code) const { return code < p; }
        size_t const p;
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
    auto fpr(disjoint_hash_set<H> const & dhs)
    {
        return dhs.unguarded_error_rate();
    }

    template <typename H>
    auto fnr(disjoint_hash_set<H> const & dhs)
    {
        return dhs.guarded_error_rate();
    }

}
