

namespace alex::set
{
    template <typename X>
    struct Identity
    {
        X operator()(X x) { return x };
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
        typename PerfHash,
        typename Hash,
        typename Coder = Identity<T>>
    class PerfectHashFilter
    {
    public:
        using value_type = T;

        using hash_fn = Hash;

        auto perfect_hash(T const & x) const
        {
            return _ph(_coder(x));
        }

        template <typename I>
        PerfectHashFilter(
            I begin,
            I end,
            long double fpr,
            double load_factor = 0.99,
            Coder coder = Coder{})
        {
                        
        };

        bool contains(T const & x) const
        {
            auto code = _coder(x);
            auto index = perf_hash = _ph(code);
            auto hash = _h(code) % _N;
            
            return _hashes[index] == hash;
        };

        long double fpr() const
        {
            return 1 / static_cast<long double>(_N);
        };

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
        bool operator==(
            PerfectHashFilter<ValueType, PerfHash, Hash, Coder> const & other) const
        {
            if (_N != other._N ||
                _h != other._h ||
                _ph != other._ph ||
                _coder == other._coder ||
                _hashes.size() != other._hashes.size())
            {
                return false;
            }

            for (size_t i = 0; i < _hashes.size(); ++i)
            {
                if (_hashes[i] != other._hashes[i])
                    return false;
            }
            return true;
        };

        // If ValueType is a countably infinite set, then no perfect hash filter is a subset of another
        // perfect hash filter. (Every perfect hash filter is a subset of the universal set for ValueType.)
        //
        // Note that when we incorporate set-theoretic operations, like differences and intersections,
        // then subsets are possible given a countably infinite universe. For instance, suppose X and Y
        // are PHFs, then X \ Y is a subset of X.
        constexpr bool subset(
            PerfectHashFilter<ValueType, PerfHash, Hash, Coder> const & other) const
        {
            return false;
        };

    private:
        using hash_type = typename Hash::hash_type;

        PerfHash _ph;
        Hash _h;
        hash_type _N;
        Coder _coder;

        // this should be a packed container type
        // that (assuming discrete uniform distribution
        // [0,N)) that can be compactly stored
        // in ceil(|S|/r log2 N) bits.
        std::vector<hash_type> _hashes;
    };
}