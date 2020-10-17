// The Perfect Hash Filter (PHF) is a positive
// approximate set. A positive approximate set
// S* is a superset of S where an element not
// in S is in S* with a probability
//     false_positive_rate().
template <class H>
class perfect_hash_filter
{
public:
    typedef H perfect_hash_function;
    typedef load_factor_type;
    using approximate_set<byte_string>::value_type;
    using approximate_set<byte_string>::probability_type;
    using approximate_set<byte_string>::cardinality_type;
    
    
    // perfect_hash_function is a type that implements the
    // perfect hash function abstract data type. It should
    // implement two components:
    //     - a build method with a load factor
    //       parameter
    //     - an iterable collection of elements of
    //       type const char* or types that
    //       can be converted to const char*.
    perfect_hash_function get_perfect_hash_function() const;
    
    // Construct a Perfect Hash Filter (a positive approximate
    // set) with a false positive rate v and load factor r
    // for an iterable collection of byte_string types from
    // begin to end, where a byte_string is a sequence of bytes.
    //
    // The iterable collection is not mutated. If the operation
    // fails, a perfect_hash_filter_build_exception is thrown
    // and the state of the Perfect Hash Filter instance is
    // undefined.
    template <class Iter>
    perfect_hash_filter(
        load_factor_type r,
        probability_type v,
        Iter begin,
        Iter end)

    bit_matrix get_hashes() const;
    
    load_factor_type load_factor() const;

    // ******************************************
    // * approximate_set<byte_string> interface *
    // ******************************************
    bool contains(
        const char*& x) const;
    
    probability_type false_positive_rate() const;
    
    // The cardinality of an approximate set over the
    // countably infinite universe of byte_strings
    // is countably infinite.
    double cardinality() const;

private:
    // we again assume bytes, and thus the false positive
    // rate is the set given by
    //     { 1/(2^8)^k : k = 1,2,...}.
    // this may be easily relaxed to save up to 7 bits per element
    // if a precise false positive rate is desired at a cost to
    // computation time. (replace byte_matrix by bit_matrix)
    byte_matrix _V;
    perfect_hash_function _h;
};

// Build a positive approximate set as implemented
// by typed Perfect Hash Filter with a false positive
// rate v and load factor r for an iterable collection
// of byte_string types from begin to end, where a
// byte_string is a sequence of bytes.
//
// The iterable collection is not mutated. If the operation
// fails, a perfect_hash_filter_build_exception is thrown
// and the state of the Perfect Hash Filter instance is
// undefined.
template <class Iter>
auto build_perfect_hash_filter(
    load_factor_type r,
    probability_type v,
    Iter begin,
    Iter end) -> perfect_hash_filter<typename iterator_traits<Iter>::value_type>