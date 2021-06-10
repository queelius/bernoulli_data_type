/**
 * approximate_hash_set<X,W,F> models a rate-distorted
 * set (RDS) of type X with a false negative rate 256^(-W), and false positive
 * rate 256^(-F).
 * 
 * 
 * 
 * It composes a rate-distorted perfect hash function and a byte matrix of
 * hashes consisting of F columns and N rows where m
 * is the cardinality of the undistorted set.
 *
 * The rate distortion comes from two independent sources:
 * 
 *     (1) False negatives arise from hash collisions on elements of the
 *         rate-distorted perefect hash function of the static set.
 * 
 *     (2) False positives arise from hash collisions on elements from the
 *         universal set on the byte matrix.
 *
 * A rate_distorted_hash_set<X,W,F,N> has a constant byte length given by
 *     F*N + M*W??? wrong, check notebooks, do more math
 * 
 * Notes: i like the idea of static size, static properties, etc.
 * 
 * give a collection of rate_distorted_set<X,W,F,N>, where N is greater than
 * the largest cardinality in the collection of sets (if not, maybe another
 * source of false negatives?), every set looks like a lot of noise of the
 * same size, nice entropy (confidentiality), uniformly distributed.
 * 
 * typical use: rate_distorted_set<X,W,F,N>
 * 
 * rate_distorted_perfect_hash<X,2,2,10000>, which is around 2*10000 + ? bytes.
 */



#include "power_probability.hpp"
#include <vector>

namespace bernoulli::set
{
    template <typename X>
    struct identity_fn
    {
        X const & operator()(X const & x) const { return x };
    };

    /**
     * phf<X,PH// models second-order bernoulli over (2^X, member-of : (X,2^X) -> bool)
     * Hash : Hashable -> HashType
     * PerfHash : Hashable -> [0,N)
     * Coder : T -> Hashable
     * Default: Coder is identity on T, in which
     *          case T must satisfy Hashable.
     * 
     * If Coder is not injective, then two or more elements
     * of T map to the same element in HashType, which
     * induces an equivelance relation such that the perfect
     * hash filter is not over T but the quotient set given by
     * T \ Coder.
     * 
     * If this was not intended, then Coder separately increases
     * the false positive rate.
     * 
     * The default Coder is the identity on ValueType, in which
     * case ValueType must be Hashable.
     */
    template <
        typename    X,
        typename    PH,
        int         K,  // fpr() := 2^(-8K)
        typename    H,
        typename    Coder = identity_fn<X>>
    class phf //: public set_expr<phf<X,PH,K,H,Coder>>
    {
    public:
        static_assert(K > 0, "K must be positive");

        using value_type = X;
        using hash_fn = H;
        using coder_fn = Coder;
        using perf_hash_fn = PH;
        using hash_type = typename H::hash_type;

        /**
         * phf<X,PH,K,H,Coder>
         */
        auto ph(X const & x) const { return _ph(_coder(x)); }

        template <typename I>
        phf(
            I begin,
            I end,
            double load_factor = .85,
            H h = H{},
            Coder coder = Coder{}) :
                coder_(coder),
                h_(h)
        {
            auto const m = std::distance(begin, end);
            auto const N = static_cast<hash_type>(std::ceil(m / load_factor));
            ph_ = PH(begin, end, N);
            
            hashes_.resize(N);
            for (auto x = begin; x != end; ++x)
            {
                auto const code = coder_(*x);
                hashes_[ph_(code)] = h_(code) % K;
            }
        }

        bool contains(X const & x) const
        {
            auto const code = coder_(x);
            return hashes_[ph_(code)] == h_(code) % K;
        }

        constexpr auto fnr() const
        {
            return ph_.rate_distortion() * tnr();
        }

        constexpr auto tpr() const
        {
            return static_cast<decltype(fnr())>(1) - fnr();
        }

        constexpr auto fpr() const
        {
            return power_probability<K,2u>{};
        }

        constexpr auto tnr() const
        {
            return static_cast<long double>(1) -
                   static_cast<long double>(fpr());
        }

        template <
            typename X2,
            unsigned int K2
        >
        bool operator==(
            perfect_hash_filter<X2, PerfHashFn, K2, HashFn, CoderFn> const & rhs) const
        {
            if (h_ != rhs.h_ ||
                ph_ != rhs.ph_ ||
                coder_ == rhs.coder_)
            {
                return false;
            }

            for (size_t i = 0; i < static_cast<size_t>(N_); ++i)
            {
                if (hashes_[i] != rhs.hashes_[i])
                    return false;
            }
            return true;            
        }


        // If ValueType is a countably infinite set, then
        //     representational equality => equality and
        //     equality => representational equality.
        // If ValueType is finite, then
        //     representational equality => equality but
        //     equality does not imply representational equality.
        // In particular, two PHFs X, Y may have different representations
        // but have the same output on any regular function for
        // approximate sets, e.g., contains(X,a) = contains(Y,a) for all
        // a in the finite universe. Note that if the function is not
        // regular, e.g., if load_factor or size of objective set was
        // stored in the PHF and used in the function, two PHFs that
        // behave the same with respect to functions like the member-of
        // predicate may not behave the same with non-regular functions
        // that compute output based on load_factor or objective set size.

    private:

        PerfHashFn ph_;
        HashFn h_;
        hash_type N_;
        CoderFn coder_;

        // this should be a packed container type
        // that (assuming discrete uniform distribution
        // [0,N)) that can be compactly stored
        // in ceil(|S|/r log2 N) bits.
        std::vector<hash_type> hashes_;
    };


    // If T has a countably infinite set of values, then the probability
    // that any two rate distorted sets are equal is zero. However, if two
    // rate distorted sets have the same representation, they must be equal. 
    //
    // Note that when we incorporate set-theoretic operations, like differences
    // and intersections, which lead to higher-order rate distorted sets,
    // then subsets are possible over a countably infinite universe, e.g.,
    // X is a subset of the union of X and Y assuming X and Y are value types
    // and not distributions.
    template <
        typename T,
        typename PerfHashFn,
        unsigned int K1,
        unsigned int K2,
        typename HashFn,
        typename CoderFn,
        bool Complement
    >
    constexpr bool subset_eq(
        perf_hash_filter<T,PerfHashFn,K1,HashFn,CoderFn,Complement> const lhs,
        perf_hash_filter<T,PerfHashFn,K2,HashFn,CoderFn,Complement> const rhs)
    {
        return lhs == rhs;
    }

    template <
        typename T1,
        typename T2,
        typename PerfHashFn1,
        typename PerfHashFn2,
        unsigned int K1,
        unsigned int K2,
        typename HashFn1,
        typename HashFn2,
        typename CoderFn1,
        typename CoderFn2,
        bool Complement1,
        bool Complement2
    >
    constexpr bool subset(
        perf_hash_filter<T1,PerfHashFn1,K1,HashFn1,CoderFn1,Complement1> const lhs,
        perf_hash_filter<T2,PerfHashFn2,K2,HashFn2,CoderFn2,Complement2> const rhs)
    {
        return false;
    }

    template <
        typename T,
        typename PerfHashFn,
        unsigned int K,
        typename HashFn,
        typename CoderFn,
        bool Complement
    >
    constexpr bool is_member(
        T lhs,
        perf_hash_filter<T,PerfHashFn,K,HashFn,CoderFn,Complement> const rhs)
    {
        return rhs.contains(lhs);
    }

    template <
        typename T,
        typename PerfHashFn,
        unsigned int K,
        typename HashFn,
        typename CoderFn,
        bool Complement
    >
    constexpr bool contains(
        perf_hash_filter<T,PerfHashFn,K,HashFn,CoderFn,Complement> const lhs,
        T rhs)
    {
        return lhs.contains(rhs);
    }
}