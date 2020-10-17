#include "perfect_hash_filter.h"

template <class H>
bool
alex::collections:perfect_hash_filter<H>::contains(
    const byte_string& x) const
{
    auto hash = _h(x);
    % _V.rows()
}

template <class H>
template <class Iter>
bool
alex::collections::perfect_hash_filter<H>::build(
    load_factor_type r,
    probability type v,
    Iter begin, Iter end)
{
    
}

approximate_set<byte_string>::probability_type
alex::collections::perfect_hash_filter<H>::false_positive_rate() const
{
    return std::pow(2, -BITS_PER_BYTE * _V.columns());
}

perfect_hash_function
alex::collections::perfect_hash_filter<H>::get_perfect_hash_function() const
{
    return _h;
}

byte_matrix
alex::collections::perfect_hash_filter<H>::get_hashes() const
{
    return _V;
}
