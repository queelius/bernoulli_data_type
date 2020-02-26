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

namespace alex::set
{
    template <typename X>
    struct Identity
    {
        X operator()(X x) const { return x };
    };

    /**
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
     * 
     * Todo: make N a template parameter, and thus fpr a template?
     */
    template <
        typename T,
        typename HashSetFunction,
        unsigned int K, // fpr = 2^(-K) or K = -log2(fpr)
        typename Hash,
        typename Coder = Identity<T>,
        bool Complement = false>
    class approximate_hash_set // models FirstOrderRateDistortedSet[T,FPR,0]
                               //        RateDistortedSet[T,FPR,0]
                               //        FirstOrderRandomApproximateSet[T,FPR,0]
                               //        RandomApproximateSet[T,FPR,0]
                               //        Set[T]
    {
    public:
        using value_type = T;

        using hash_fn = Hash;

        auto hash_set(T const & x) const { return _ph(_coder(x)); }

        template <typename I>
        approximate_hash_set(
            I begin,
            I end,
            float load_factor = .85f,
            Hash h = Hash{},
            Coder coder = Coder{})
        {
            auto const m = std::distance(begin, end);
            _N = static_cast<hash_type>(std::ceil(m / load_factor));
            _ph = HashSetFunction(begin, end, load_factor);
            _coder = coder;
            _h = h;
            _hashes.resize(_N);

            for (auto x = begin; x != end; ++x)
            {
                auto const code = _coder(*x);
                _hashes[_ph(code)] = _h(code) % K;
            }
        }

        bool contains(T const & x) const
        {
            auto const code = _coder(x);
            auto const hash_index = _ph(code);
            auto const hash_check = _h(code) % _N;
            
            return _hashes[hash_index] == hash_check;
        }

        constexpr auto fnr() const
        {
            return _ph.rate_distortion() * tnr();
        }

        constexpr auto tpr() const
        {
            return 1. - fnr();
        }

        constexpr auto fpr() const
        {
            return power_probability<K,2u>{};
        }

        constexpr auto tnr() const
        {
            return 1. - static_cast<long double>(power_probability<K,2u>{});
        }

        template <
            typename T2,
            unsigned int K2>
        bool operator==(
            approximate_hash_set<T2, HashSetFunction, K2, Hash, Coder> const & rhs) const
        {
            if (_N != rhs._N ||
                _h != rhs._h ||
                _ph != rhs._ph ||
                _coder == rhs._coder)
            {
                return false;
            }

            for (size_t i = 0; i < static_cast<size_t>(_N); ++i)
            {
                if (_hashes[i] != rhs._hashes[i])
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
        using hash_type = typename Hash::hash_type;

        HashSetFunction _ph;
        Hash _h;
        hash_type _N;
        Coder _coder;

        // this should be a packed container type
        // that (assuming discrete uniform distribution
        // [0,N)) that can be compactly stored
        // in ceil(|S|/r log2 N) bits.
        std::vector<hash_type> _hashes;
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
        typename HashSetFunction,
        unsigned int K1,
        unsigned int K2,
        typename Hash,
        typename Coder,
        bool Complement
    >
    constexpr bool subset_eq(
        approximate_hash_set<T,HashSetFunction,K1,Hash,Coder,Complement> const lhs,
        approximate_hash_set<T,HashSetFunction,K2,Hash,Coder,Complement> const rhs)
    {
        return lhs == rhs;
    }

    template <
        typename T1,
        typename T2,
        typename HashSetFunction1,
        typename HashSetFunction2,
        unsigned int K1,
        unsigned int K2,
        typename Hash1,
        typename Hash2,
        typename Coder1,
        typename Coder2,
        bool Complement1,
        bool Complement2
    >
    constexpr bool subset(
        approximate_hash_set<T1,HashSetFunction1,K1,Hash1,Coder1,Complement1> const lhs,
        approximate_hash_set<T2,HashSetFunction2,K2,Hash2,Coder2,Complement2> const rhs)
    {
        return false;
    }

    template <
        typename T,
        typename HashSetFunction,
        unsigned int K,
        typename Hash,
        typename Coder,
        bool Complement
    >
    constexpr bool is_member(
        T lhs,
        approximate_hash_set<T,HashSetFunction,K,Hash,Coder,Complement> const rhs)
    {
        return rhs.contains(lhs);
    }

    template <
        typename T,
        typename HashSetFunction,
        unsigned int K,
        typename Hash,
        typename Coder,
        bool Complement
    >
    constexpr bool contains(
        approximate_hash_set<T,HashSetFunction,K,Hash,Coder,Complement> const lhs,
        T rhs,
    {
        return lhs.contains(rhs);
    }



}