#pragma once

#include <vector>
#include <functional>
#include <typeinfo>
#include <cmath>
#include <iterator>

// H models the concept of a hash function object.
// T models the concept of hashable<T>,
//     hashable<T> => hash : T -> convertible_to<size_t>
// where hash is an instance of H.
//
// By default, H = std::hash<T>, but bloom filters assume that instances of
// H model random hash functions. std::hash<T>, if defined, make no such
// guarantee. Thus, it is advisable to parameterize with a different hash
// function type.
//
// space_optiomal_bloom_filter<T> models set-indicator functions over T with some specifable
// expected false positive rate.
template <typename T, typename H = std::hash<T>>
class space_optimal_bloom_filter
{
public:
    using value_type = T;
    using hash_fn_type = H;
    using const_iterator = std::vector<bool>::const_iterator;

    static constexpr double BITS_PER_ELEMENT =
        1.4426950408889634073599246810018921374266459541529859341354494069;

    static const size_t salt(size_t index)
    {
        static const size_t salts[] =
        {
            0x8CA63C47, 0x42CC2884, 0x8E89919B, 0x6EDBD7D3,
            0x15B6796C, 0x1D6FDFE4, 0x63FF9092, 0xE7401432,
            0xEFFE9412, 0xAEAEDF79, 0x9F245A31, 0x83C136FC,
            0xC3DA4A8C, 0xA5112C8C, 0x5271F491, 0x9A948DAB,
            0xCEE59A8D, 0xB5F525AB, 0x59D13217, 0x24E7C331,
            0x697C2103, 0x84B0A460, 0x86156DA9, 0xAEF2AC68,
            0x23243DA5, 0x3F649643, 0x5FA495A8, 0x67710DF8,
            0x9A6C499E, 0xDCFB0227, 0x46A43433, 0x1832B07A,
            0xC46AFF3C, 0xB9C8FFF0, 0xC9500467, 0x34431BDF,
            0xB652432B, 0xE367F12B, 0x427F4C1B, 0x224C006E,
        };
        return index > sizeof(salts) ?
            salts[index % sizeof(salts)] ^ index : salts[index];
    }

    space_optimal_bloom_filter(space_optimal_bloom_filter const & bf) = default;

    /**
     * Given a false positive rate and the elements to model as a
     * approximate set, we generate a space-optiomal bloom filter with the
     * minimum bit length given by approximately 1.44 bits/element.
     * 
     * I models the concept of ForwardIterator and the range
     *     [b,e) has no duplicates.
     */
    template <typename I>
    space_optimal_bloom_filter(I b, I e, double fpr, H h = H()) :
        n_(std::distance(b,e)),
        fpr_(fpr),
        hash_(h),
        k_(std::round(std::log2(1/fpr))),
        bits_(std::round(std::distance(b,e)*BITS_PER_ELEMENT*std::log2(1/fpr)))
    {
        for (auto x = b; x != e; ++x)
        {
            auto hx = hash_(*x);
            for (size_t s = 0; s < k_; ++s)
                bits_[(salt(s) ^ hx) % m()] = true;
        }
    }

    // if we do not wish to store the fpr, and we only have the bloom filter
    // to estimate the fpr, a reasonable estimator is
    //     fpr = 2^(-k), -- note: k = m / n ln 2
    //     fpr = 2^(m / n ln 2)
    // although since the 'true' optimal value for k is some real number
    // rather than an integer, this can be off quite a bit.
    // we can bound it with
    //     fpr in [2^(-k-1),2^(-k+1)].
    // given that this is a space-optimal bloom filter, there is no way to
    // make the fpr more uncertain. if the bloom filter is not space-optimal,
    // e.g., we add some additional bits, then we can make the fpr estimator
    // less certain.
    auto fpr() const { return fpr_; };

    auto operator()(T const & x) const
    {
        auto hx = hash_(x);
        for (size_t s = 0; s < k_; ++s)
        {
            if (!(bits_[(salt(s) ^ hx) % m()]))
                return false;
        }
        return true;
    }

    auto bit_length() const { return bits_.size(); }

    // number of elements (+-1)
    // assumes that given m, k is chosen to minimize fpr.
    // this is an estimator, we don't store.
    //auto n() const { return (1./BITS_PER_ELEMENT)*m()/k_; }
    auto n() const { return n_; }

    // bit length of bit array
    auto m() const { return bits_.size(); }

    // number of hash functions
    auto k() const { return k_; }

    auto hash_fn() const { return hash_; }

    // convert optimal_space_bloom filter to a bit_string.
    operator std::vector<bool>() const { return bits_; }

    auto to_hash() const
    {
         return std::hash<std::vector<bool>>{}(bits_) ^
             k() ^
             n() ^
             typeid(T).hash_code();
    }

    auto begin() const noexcept { return bits_.begin(); }
    auto end() const noexcept { return bits_.end(); }

private:
    double fpr_;
    std::vector<bool> bits_;
    size_t k_;
    size_t n_;
    H hash_;
};

template <typename T, typename H>
auto contains(space_optimal_bloom_filter<T,H> const & bf, T const & x)
{
    return bf(x);
}

template <typename T, typename H>
auto cardinality(space_optimal_bloom_filter<T,H> const & bf)
{
    return bf.n();
}

template <typename T, typename H>
auto fpr(space_optimal_bloom_filter<T,H> const & bf)
{   
    return bf.fpr();
}

template <typename T, typename H>
auto fnr(space_optimal_bloom_filter<T,H> const & bf)
{
    return 0.;
}

namespace std
{
    template <typename T, typename H>
    struct hash<space_optimal_bloom_filter<T,H>>
    {
        size_t operator()(space_optimal_bloom_filter<T,H> const & bf) const
        {
            return bf.to_hash();
        }
    };
}

// fpr(complement(bf)) = 0
// fnr(complement(bf)) = fpr(bf)
// operator+(bf1,bf2) = boolean_join_expr<T>(bf1,bf2)
// fpr(boolean_join_expr<T>(bf1,bf2)) = ...