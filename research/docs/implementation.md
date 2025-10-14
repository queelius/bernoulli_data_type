# Bernoulli Types: C++ Implementation Guide

This guide explains how to implement Bernoulli types in C++, covering design patterns, best practices, and concrete examples.

## Table of Contents

1. [Core Concepts](#core-concepts)
2. [Basic Types](#basic-types)
3. [Type Erasure Pattern](#type-erasure-pattern)
4. [Error Propagation](#error-propagation)
5. [Expression Templates](#expression-templates)
6. [Practical Data Structures](#practical-data-structures)
7. [Performance Considerations](#performance-considerations)
8. [Testing and Validation](#testing-and-validation)

## Core Concepts

### The Observed Type Wrapper

The fundamental pattern wraps latent values with error information:

```cpp
template <typename T>
struct observed {
    T value;              // The observed value
    error_info error;     // Error characteristics
    
    // Implicit conversion for convenience
    operator T() const { return value; }
};
```

### Error Representation

Errors can be represented in several ways:

```cpp
// Simple rate
using error_rate = double;  // [0, 1]

// Interval for uncertainty
struct rate_span {
    double min, max;
    
    rate_span(double r) : min(r), max(r) {}
    rate_span(double lo, double hi) 
        : min(std::max(0.0, lo)), max(std::min(1.0, hi)) {}
};

// Full confusion matrix (for higher orders)
template <size_t N>
using confusion_matrix = std::array<std::array<double, N>, N>;
```

## Basic Types

### Observed Boolean

```cpp
struct observed_bool {
    bool value;
    rate_span error;
    
    observed_bool(bool v, double err = 0.0) 
        : value(v), error(err) {}
    
    // Logical operations with error propagation
    friend observed_bool operator!(observed_bool a) {
        return {!a.value, a.error};  // NOT preserves error rate
    }
    
    friend observed_bool operator&(observed_bool a, observed_bool b) {
        if (a.value && b.value) {
            // Both true: error if either was false
            auto err = a.error + b.error - a.error * b.error;
            return {true, err};
        } else {
            // At least one false: complex error analysis
            // ... (see full implementation)
        }
    }
};
```

### Observed Integer with Bounds

```cpp
template <typename Int = int>
struct observed_int {
    Int value;
    Int error_bound;  // Maximum absolute error
    
    observed_int(Int v, Int bound = 0) 
        : value(v), error_bound(bound) {}
    
    // Arithmetic with error propagation
    friend observed_int operator+(observed_int a, observed_int b) {
        return {a.value + b.value, a.error_bound + b.error_bound};
    }
    
    friend observed_int operator*(observed_int a, observed_int b) {
        auto bound = std::abs(a.value) * b.error_bound 
                   + std::abs(b.value) * a.error_bound 
                   + a.error_bound * b.error_bound;
        return {a.value * b.value, bound};
    }
};
```

## Type Erasure Pattern

### Generic Observed Set Interface

```cpp
template <typename T>
class observed_set {
public:
    using value_type = T;
    
    // Type-erased interface
    virtual ~observed_set() = default;
    virtual bool contains(T const& x) const = 0;
    virtual rate_span false_positive_rate() const = 0;
    virtual rate_span false_negative_rate() const = 0;
    
    // Convenience operator
    bool operator()(T const& x) const { return contains(x); }
};
```

### Concrete Implementation Wrapper

```cpp
template <typename T>
class observed_set_impl : public observed_set<T> {
public:
    template <typename SetType>
    observed_set_impl(SetType s) : impl_(std::make_unique<model<SetType>>(std::move(s))) {}
    
    bool contains(T const& x) const override {
        return impl_->contains(x);
    }
    
    rate_span false_positive_rate() const override {
        return impl_->false_positive_rate();
    }
    
    rate_span false_negative_rate() const override {
        return impl_->false_negative_rate();
    }
    
private:
    struct concept {
        virtual ~concept() = default;
        virtual bool contains(T const& x) const = 0;
        virtual rate_span false_positive_rate() const = 0;
        virtual rate_span false_negative_rate() const = 0;
    };
    
    template <typename SetType>
    struct model : concept {
        SetType set;
        
        explicit model(SetType s) : set(std::move(s)) {}
        
        bool contains(T const& x) const override {
            return set.contains(x);
        }
        
        rate_span false_positive_rate() const override {
            return set.false_positive_rate();
        }
        
        rate_span false_negative_rate() const override {
            return set.false_negative_rate();
        }
    };
    
    std::unique_ptr<concept> impl_;
};
```

## Error Propagation

### Interval Arithmetic

```cpp
namespace interval {
    rate_span add(rate_span a, rate_span b) {
        return {a.min + b.min, a.max + b.max};
    }
    
    rate_span multiply(rate_span a, rate_span b) {
        auto products = {
            a.min * b.min, a.min * b.max,
            a.max * b.min, a.max * b.max
        };
        return {
            *std::min_element(products.begin(), products.end()),
            *std::max_element(products.begin(), products.end())
        };
    }
    
    rate_span complement(rate_span a) {
        return {1.0 - a.max, 1.0 - a.min};
    }
}
```

### Set Operation Error Composition

```cpp
template <typename T>
class set_union_expr : public observed_set<T> {
    observed_set<T> const& lhs_;
    observed_set<T> const& rhs_;
    
public:
    set_union_expr(observed_set<T> const& l, observed_set<T> const& r) 
        : lhs_(l), rhs_(r) {}
    
    bool contains(T const& x) const override {
        return lhs_.contains(x) || rhs_.contains(x);
    }
    
    rate_span false_positive_rate() const override {
        auto fpr_l = lhs_.false_positive_rate();
        auto fpr_r = rhs_.false_positive_rate();
        // Union FPR: 1 - (1-α₁)(1-α₂)
        return interval::complement(
            interval::multiply(
                interval::complement(fpr_l),
                interval::complement(fpr_r)
            )
        );
    }
    
    rate_span false_negative_rate() const override {
        auto fnr_l = lhs_.false_negative_rate();
        auto fnr_r = rhs_.false_negative_rate();
        // Union FNR: β₁ · β₂
        return interval::multiply(fnr_l, fnr_r);
    }
};
```

## Expression Templates

### Lazy Evaluation for Set Algebra

```cpp
namespace algebra {
    template <typename Derived>
    class set_expr {
    public:
        Derived const& derived() const {
            return static_cast<Derived const&>(*this);
        }
        
        template <typename T>
        bool contains(T const& x) const {
            return derived().contains(x);
        }
    };
    
    // Union expression
    template <typename L, typename R>
    class union_expr : public set_expr<union_expr<L, R>> {
        L const& lhs_;
        R const& rhs_;
        
    public:
        union_expr(L const& l, R const& r) : lhs_(l), rhs_(r) {}
        
        template <typename T>
        bool contains(T const& x) const {
            return lhs_.contains(x) || rhs_.contains(x);
        }
    };
    
    // Operator overload
    template <typename L, typename R>
    union_expr<L, R> operator|(set_expr<L> const& lhs, set_expr<R> const& rhs) {
        return {lhs.derived(), rhs.derived()};
    }
}
```

## Practical Data Structures

### Bloom Filter Implementation

```cpp
template <typename T, typename Hash = std::hash<T>>
class bloom_filter : public observed_set<T> {
private:
    std::vector<bool> bits_;
    size_t num_hashes_;
    Hash hasher_;
    
    size_t hash(T const& item, size_t seed) const {
        return hasher_(item) ^ (seed * 0x9e3779b9);  // Golden ratio hash mixing
    }
    
public:
    bloom_filter(size_t size, size_t num_hashes)
        : bits_(size), num_hashes_(num_hashes) {}
    
    void insert(T const& item) {
        for (size_t i = 0; i < num_hashes_; ++i) {
            bits_[hash(item, i) % bits_.size()] = true;
        }
    }
    
    bool contains(T const& item) const override {
        for (size_t i = 0; i < num_hashes_; ++i) {
            if (!bits_[hash(item, i) % bits_.size()]) {
                return false;
            }
        }
        return true;
    }
    
    rate_span false_positive_rate() const override {
        double k = num_hashes_;
        double m = bits_.size();
        double n = std::count(bits_.begin(), bits_.end(), true) * k / m;  // Estimate
        double fpr = std::pow(1.0 - std::exp(-k * n / m), k);
        return fpr;
    }
    
    rate_span false_negative_rate() const override {
        return 0.0;  // Bloom filters have no false negatives
    }
};
```

### Count-Min Sketch

```cpp
template <typename T, typename Count = uint32_t>
class count_min_sketch {
private:
    std::vector<std::vector<Count>> counts_;
    size_t width_;
    size_t depth_;
    
    size_t hash(T const& item, size_t row) const {
        return (std::hash<T>{}(item) ^ (row * 0x517cc1b727220a95)) % width_;
    }
    
public:
    count_min_sketch(size_t width, size_t depth)
        : counts_(depth, std::vector<Count>(width, 0)),
          width_(width), depth_(depth) {}
    
    void update(T const& item, Count delta = 1) {
        for (size_t i = 0; i < depth_; ++i) {
            counts_[i][hash(item, i)] += delta;
        }
    }
    
    Count query(T const& item) const {
        Count min_count = std::numeric_limits<Count>::max();
        for (size_t i = 0; i < depth_; ++i) {
            min_count = std::min(min_count, counts_[i][hash(item, i)]);
        }
        return min_count;
    }
    
    // This returns an observed frequency ≥ true frequency
    observed<Count> observed_frequency(T const& item) const {
        return {query(item), rate_span(0.0, compute_error_rate())};
    }
    
private:
    double compute_error_rate() const {
        // ε = e/width, δ = 1/e^depth
        return 2.718 / width_;  // Simplified
    }
};
```

## Performance Considerations

### Memory Layout

```cpp
// Cache-friendly Bloom filter variant
template <typename T>
class cache_aligned_bloom {
    static constexpr size_t cache_line_size = 64;
    static constexpr size_t bits_per_line = cache_line_size * 8;
    
    struct alignas(cache_line_size) cache_line {
        std::array<uint64_t, cache_line_size / sizeof(uint64_t)> words;
    };
    
    std::vector<cache_line> data_;
    // ... implementation
};
```

### SIMD Operations

```cpp
#include <immintrin.h>

// Vectorized contains check for multiple items
template <typename T>
bool contains_any(bloom_filter<T> const& bf, std::array<T, 4> const& items) {
    __m256i results = _mm256_setzero_si256();
    
    for (auto const& item : items) {
        // Check all hash positions in parallel
        // ... SIMD implementation
    }
    
    return !_mm256_testz_si256(results, results);
}
```

## Testing and Validation

### Statistical Validation

```cpp
template <typename ObservedSet>
void validate_error_rates(ObservedSet& os, size_t num_trials = 100000) {
    size_t false_positives = 0;
    size_t false_negatives = 0;
    
    // Insert known elements
    std::set<int> truth_set;
    for (int i = 0; i < num_trials / 2; ++i) {
        truth_set.insert(i);
        os.insert(i);
    }
    
    // Test membership
    for (int i = 0; i < num_trials; ++i) {
        bool truth = truth_set.count(i) > 0;
        bool observed = os.contains(i);
        
        if (!truth && observed) false_positives++;
        if (truth && !observed) false_negatives++;
    }
    
    double observed_fpr = double(false_positives) / (num_trials - truth_set.size());
    double observed_fnr = double(false_negatives) / truth_set.size();
    
    auto expected_fpr = os.false_positive_rate();
    auto expected_fnr = os.false_negative_rate();
    
    // Check within confidence intervals
    assert(expected_fpr.min <= observed_fpr && observed_fpr <= expected_fpr.max);
    assert(expected_fnr.min <= observed_fnr && observed_fnr <= expected_fnr.max);
}
```

### Property-Based Testing

```cpp
template <typename T>
void test_set_algebra_properties() {
    // Commutativity of union
    auto check_union_commutative = [](observed_set<T> const& a, 
                                      observed_set<T> const& b,
                                      T const& x) {
        return (a | b).contains(x) == (b | a).contains(x);
    };
    
    // De Morgan's laws with error bounds
    auto check_de_morgan = [](observed_set<T> const& a,
                             observed_set<T> const& b,
                             T const& x) {
        auto lhs = ~(a | b);
        auto rhs = (~a) & (~b);
        // Check error bounds overlap
        return error_compatible(lhs.contains(x), rhs.contains(x));
    };
    
    // Run property tests with random inputs
    // ...
}
```

## Best Practices

1. **Make Error Rates Explicit**: Always expose error characteristics in the interface
2. **Use Value Semantics**: Observed types should behave like regular types when possible
3. **Lazy Evaluation**: Use expression templates for complex set operations
4. **Cache Efficiency**: Align data structures to cache boundaries
5. **Statistical Validation**: Always verify error rates empirically
6. **Documentation**: Clearly document the latent/observed relationship

## Advanced Techniques

### Adaptive Error Rates

```cpp
template <typename T>
class adaptive_bloom : public observed_set<T> {
    mutable size_t queries_ = 0;
    mutable size_t recent_fps_ = 0;
    
    void adjust_if_needed() const {
        if (queries_ % 10000 == 0) {
            double observed_fpr = double(recent_fps_) / queries_;
            if (observed_fpr > target_fpr_ * 1.5) {
                rehash_with_more_bits();
            }
        }
    }
    // ...
};
```

### Compressed Representations

```cpp
template <typename T>
class compressed_observed_set {
    using compressed_bits = std::vector<uint8_t>;
    
    static compressed_bits compress(std::vector<bool> const& bits) {
        // Run-length encoding, Golomb coding, etc.
    }
    
    static std::vector<bool> decompress(compressed_bits const& data) {
        // Inverse operation
    }
    // ...
};
```

This implementation guide provides the foundation for building robust Bernoulli type implementations in C++. The key is maintaining the distinction between latent and observed throughout the design while providing efficient and ergonomic interfaces.