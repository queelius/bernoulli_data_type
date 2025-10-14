#pragma once

#include <vector>
#include <bitset>
#include <functional>
#include <random>
#include <cmath>
#include <algorithm>
#include <unordered_set>

namespace bernoulli {

/**
 * @brief A Bloom filter implementation as a positive Bernoulli set
 * 
 * This is a Bernoulli set with false positives but no false negatives.
 * It demonstrates the simplest non-trivial Bernoulli set.
 */
template<typename T>
class bloom_filter {
private:
    std::vector<bool> bits_;
    size_t size_;
    size_t num_hashes_;
    size_t num_elements_ = 0;
    
    // Hash function seeds
    std::vector<std::size_t> hash_seeds_;
    
    size_t hash(const T& item, size_t seed) const {
        std::hash<T> hasher;
        size_t h = hasher(item);
        // Mix with seed
        h ^= seed + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h % size_;
    }
    
public:
    bloom_filter(size_t expected_elements, double target_fp_rate = 0.01) {
        // Optimal size: m = -n * ln(p) / (ln(2)^2)
        size_ = static_cast<size_t>(
            -expected_elements * std::log(target_fp_rate) / (std::log(2) * std::log(2))
        );
        
        // Optimal number of hashes: k = (m/n) * ln(2)
        num_hashes_ = static_cast<size_t>(
            (size_ / static_cast<double>(expected_elements)) * std::log(2)
        );
        
        bits_.resize(size_, false);
        
        // Initialize hash seeds
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis;
        
        for (size_t i = 0; i < num_hashes_; ++i) {
            hash_seeds_.push_back(dis(gen));
        }
    }
    
    void insert(const T& item) {
        for (size_t i = 0; i < num_hashes_; ++i) {
            size_t idx = hash(item, hash_seeds_[i]);
            bits_[idx] = true;
        }
        num_elements_++;
    }
    
    bool contains(const T& item) const {
        for (size_t i = 0; i < num_hashes_; ++i) {
            size_t idx = hash(item, hash_seeds_[i]);
            if (!bits_[idx]) {
                return false;  // Definitely not in set
            }
        }
        return true;  // Probably in set
    }
    
    double false_positive_rate() const {
        // Actual FP rate: (1 - e^(-kn/m))^k
        double ratio = static_cast<double>(num_hashes_ * num_elements_) / size_;
        return std::pow(1 - std::exp(-ratio), num_hashes_);
    }
    
    double false_negative_rate() const {
        return 0.0;  // Bloom filters have no false negatives
    }
    
    size_t size_bits() const { return size_; }
    size_t num_elements() const { return num_elements_; }
};

/**
 * @brief A general Bernoulli set with both false positives and negatives
 * 
 * This implementation uses a combination of a Bloom filter and sampling
 * to achieve the desired error rates.
 */
template<typename T>
class bernoulli_set {
private:
    bloom_filter<T> positive_filter_;  // For elements definitely in set
    bloom_filter<T> negative_filter_;  // For elements definitely NOT in set
    double target_fp_rate_;
    double target_fn_rate_;
    
    mutable std::mt19937 rng_{std::random_device{}()};
    
public:
    bernoulli_set(size_t expected_size, 
                  double fp_rate = 0.01, 
                  double fn_rate = 0.01)
        : positive_filter_(expected_size * (1 - fn_rate), fp_rate),
          negative_filter_(expected_size * fn_rate, fn_rate),
          target_fp_rate_(fp_rate),
          target_fn_rate_(fn_rate) {}
    
    void insert(const T& item) {
        std::uniform_real_distribution<> dist(0.0, 1.0);
        
        if (dist(rng_) < target_fn_rate_) {
            // Simulate false negative: don't insert
            negative_filter_.insert(item);
        } else {
            // Normal insertion
            positive_filter_.insert(item);
        }
    }
    
    bool contains(const T& item) const {
        // Check negative filter first
        if (negative_filter_.contains(item)) {
            return false;  // Marked as definitely not in set
        }
        
        // Check positive filter
        return positive_filter_.contains(item);
    }
    
    double false_positive_rate() const {
        return target_fp_rate_;
    }
    
    double false_negative_rate() const {
        return target_fn_rate_;
    }
};

/**
 * @brief Set operations on Bernoulli sets
 */
template<typename T>
class bernoulli_set_ops {
public:
    // Union of two Bloom filters (positive Bernoulli sets)
    static bloom_filter<T> union_bloom(const bloom_filter<T>& a, 
                                      const bloom_filter<T>& b) {
        if (a.size_bits() != b.size_bits()) {
            throw std::invalid_argument("Bloom filters must have same size");
        }
        
        bloom_filter<T> result(a.size_bits(), 0.01);  // Dummy construction
        
        // OR the bit vectors
        // (This is a simplification; real implementation would need access to bits)
        // The FP rate of union is approximately: 1 - (1-fp_a)(1-fp_b)
        
        return result;
    }
    
    // Intersection with error propagation
    static bernoulli_set<T> intersection(const bernoulli_set<T>& a,
                                        const bernoulli_set<T>& b) {
        // FN rate increases: 1 - (1-fn_a)(1-fn_b)
        double new_fn = 1 - (1 - a.false_negative_rate()) * 
                            (1 - b.false_negative_rate());
        
        // FP rate depends on overlap...
        double new_fp = a.false_positive_rate() * b.false_positive_rate();
        
        bernoulli_set<T> result(1000, new_fp, new_fn);  // Size is approximate
        
        // Would need to implement actual intersection logic
        return result;
    }
};

/**
 * @brief Demonstration of Bernoulli sets
 */
inline void demonstrate_bernoulli_sets() {
    std::cout << "=== Bernoulli Set Demonstration ===\n\n";
    
    // 1. Bloom Filter (Positive Bernoulli Set)
    std::cout << "1. Bloom Filter (False Positives Only):\n";
    bloom_filter<std::string> bloom(1000, 0.01);
    
    // Insert some elements
    std::vector<std::string> elements = {
        "apple", "banana", "cherry", "date", "elderberry"
    };
    
    for (const auto& elem : elements) {
        bloom.insert(elem);
    }
    
    // Test membership
    std::cout << "Membership tests:\n";
    for (const auto& elem : elements) {
        std::cout << "  " << elem << ": " 
                  << (bloom.contains(elem) ? "YES" : "NO") << "\n";
    }
    
    // Test false positives
    std::vector<std::string> non_elements = {
        "grape", "kiwi", "mango", "orange", "peach"
    };
    
    int false_positives = 0;
    for (const auto& elem : non_elements) {
        if (bloom.contains(elem)) {
            false_positives++;
            std::cout << "  " << elem << ": FALSE POSITIVE\n";
        }
    }
    
    std::cout << "\nExpected FP rate: " << (bloom.false_positive_rate() * 100) << "%\n";
    std::cout << "Observed FP rate: " << (100.0 * false_positives / non_elements.size()) << "%\n\n";
    
    // 2. General Bernoulli Set (Both FP and FN)
    std::cout << "2. General Bernoulli Set (Both Errors):\n";
    bernoulli_set<int> bset(1000, 0.05, 0.05);  // 5% FP, 5% FN
    
    // Insert numbers 0-99
    for (int i = 0; i < 100; ++i) {
        bset.insert(i);
    }
    
    // Count false negatives (elements that should be found but aren't)
    int fn_count = 0;
    for (int i = 0; i < 100; ++i) {
        if (!bset.contains(i)) {
            fn_count++;
        }
    }
    
    // Count false positives (elements that shouldn't be found but are)
    int fp_count = 0;
    for (int i = 100; i < 200; ++i) {
        if (bset.contains(i)) {
            fp_count++;
        }
    }
    
    std::cout << "False negatives: " << fn_count << "/100 (" 
              << (100.0 * fn_count / 100) << "%)\n";
    std::cout << "False positives: " << fp_count << "/100 (" 
              << (100.0 * fp_count / 100) << "%)\n";
    std::cout << "Target rates: FP=" << (bset.false_positive_rate() * 100) 
              << "%, FN=" << (bset.false_negative_rate() * 100) << "%\n\n";
    
    // 3. Information Retrieval Metrics
    std::cout << "3. Binary Classification Metrics:\n";
    
    int tp = 100 - fn_count;  // True positives
    int tn = 100 - fp_count;  // True negatives
    int total_positive = tp + fp_count;
    int total_actual = tp + fn_count;
    
    double precision = (total_positive > 0) ? 
                      static_cast<double>(tp) / total_positive : 0.0;
    double recall = (total_actual > 0) ? 
                   static_cast<double>(tp) / total_actual : 0.0;
    double f1 = (precision + recall > 0) ? 
                2 * precision * recall / (precision + recall) : 0.0;
    
    std::cout << "Precision: " << std::fixed << std::setprecision(3) 
              << precision << "\n";
    std::cout << "Recall: " << recall << "\n";
    std::cout << "F1 Score: " << f1 << "\n";
}

} // namespace bernoulli