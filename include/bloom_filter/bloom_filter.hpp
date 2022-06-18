#pragma once

#include <vector>
#include <functional>
#include <typeinfo>
#include <cmath>
#include <iterator>

// T models the concept of hashable<T>, which means that
//     hash : T -> convertible_to<size_t>
// is defined.
//
// bloom_filter<T> models set-indicator functions over T with some specifable
// expected false positive rate.
template <typename T, typename H = std::hash<T>>
class bloom_filter
{
public:
    using value_type = T;
    using bit_string = std::vector<bool>;
    using const_iterator = bit_string::const_iterator;

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

    // I models the concept of InputIterator
    template <typename I>
    bloom_filter(I b, I e, size_t m, size_t k, H h = H()) :
        k_(k), 
        bits_(m,false),
        n_(0),
        hash_(h)
    {
        for (auto x = b; x != e; ++x)
        {
            ++n_;
            auto hx = hash_(*x);
            for (size_t s = 0; s < k; ++s)
                bits_[(s ^ h) % m] = true;
        }
    }

    // number of elements inserted
    auto n() const { return n_; }

    // bit length of bit array
    auto m() const { return bits_.size(); }

    // number of hash functions
    auto k() const { return k_; }

    // false positive rate
    auto fpr() const { return fpr_; }

    // convert bloom filter to a bit_string.
    operator std::vector<bool>() const { return bits_; }

    auto to_hash() const
    {
        return std::hash<std::vector<bool>>{}(bits_) ^
            k_ ^ n_ ^ typeid(T).hash_code();
    }

    void insert(T const & x)
    {
        size_t hx = hash_(x);
        ++n_;
        for (size_t s = 0; s < k_; ++s)
            bits_[(s ^ h) % m()] = true;
    }

    auto begin() const noexcept { return bits_.begin(); }
    auto end() const noexcept { return bits_.end(); }

private:
    std::vector<bool> bits_;
    size_t k_, n_;
    H hash_;
};

template <typename T>
auto contains(bloom_filter<T> const & bf, T const & x)
{
    return bf(x);
}

// I models the concept of ForwardIterator
//
// If we are given a false positive rate and no other constraints, we
// find values for k and m that minimize bit length.
template <typename I, typename R>
auto bloom_filter_fpr(I b, I e, R fpr)
{
    auto n = std::distance(b,e);
    auto k = static_cast<size_t>(std::ceil(-std::log2(fpr)));
    auto m = static_cast<size_t>(std::ceil(n*k/std::log2(2.)));

    using T = typename std::iterator_traits<I>::value_type;
    return bloom_filter<T>(b,e,m,k);
}


 
// I models the concept of ForwardIterator
//
// If we are given a bit length m and no other constraints, we
// find optimal values for k that minimize fpr.
template <typename I>
auto bloom_filter_bit_length(I b, I e, size_t m)
{
    using T = typename std::iterator_traits<I>::value_type;
    return bloom_filter<T>(b,e,m,static_cast<size_t>(ceil(m*log(2.))));
}

template <typename T>
auto cardinality(bloom_filter<T> const & bf)
{
    return bf.n();
}

template <typename T>
auto fpr(bloom_filter<T> const & bf)
{   
    return bf.fpr();
}

template <typename T>
auto fnr(bloom_filter<T> const & bf)
{
    return 0.;
}

namespace std
{
    //template <typename T>
    //auto hash(bloom_filter<T> const & bf)
   // {
   //     return bf.to_hash();
   // }
}

// fpr(complement(bf)) = 0
// fnr(complement(bf)) = fpr(bf)
// operator+(bf1,bf2) = boolean_join_expr<T>(bf1,bf2)
// fpr(boolean_join_expr<T>(bf1,bf2)) = ...