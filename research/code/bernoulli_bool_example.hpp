#pragma once

#include <random>
#include <functional>
#include <iostream>
#include <iomanip>

namespace bernoulli {

/**
 * @brief A Boolean value with controlled error probability
 * 
 * Models a Boolean that may be flipped with specified probabilities.
 * Order 1: Symmetric errors (same rate for false->true and true->false)
 * Order 2: Asymmetric errors (different rates for each direction)
 */
template<int Order = 1>
class bernoulli_bool {
private:
    bool value_;
    double error_rate_;      // For Order 1
    double false_positive_;  // For Order 2
    double false_negative_;  // For Order 2
    
    mutable std::mt19937 rng_{std::random_device{}()};
    
public:
    // Order 1 constructor (symmetric error)
    template<typename = std::enable_if_t<Order == 1>>
    bernoulli_bool(bool value, double error_rate = 0.0) 
        : value_(value), error_rate_(error_rate) {
        if (error_rate < 0.0 || error_rate > 1.0) {
            throw std::invalid_argument("Error rate must be in [0,1]");
        }
    }
    
    // Order 2 constructor (asymmetric error)
    template<typename = std::enable_if_t<Order == 2>>
    bernoulli_bool(bool value, double fp_rate, double fn_rate)
        : value_(value), false_positive_(fp_rate), false_negative_(fn_rate) {
        if (fp_rate < 0.0 || fp_rate > 1.0 || fn_rate < 0.0 || fn_rate > 1.0) {
            throw std::invalid_argument("Error rates must be in [0,1]");
        }
    }
    
    // Observe the value (may return incorrect result)
    bool observe() const {
        std::uniform_real_distribution<> dist(0.0, 1.0);
        double p = dist(rng_);
        
        if constexpr (Order == 1) {
            // Symmetric error: flip with probability error_rate_
            return (p < error_rate_) ? !value_ : value_;
        } else if constexpr (Order == 2) {
            // Asymmetric error
            if (value_) {
                // True value: false negative with probability fn_rate
                return (p >= false_negative_);
            } else {
                // False value: false positive with probability fp_rate
                return (p < false_positive_);
            }
        }
    }
    
    // Implicit conversion (samples on each use)
    operator bool() const {
        return observe();
    }
    
    // Get the true value (for testing/debugging)
    bool true_value() const { return value_; }
    
    // Get error characteristics
    double error_rate() const { 
        if constexpr (Order == 1) {
            return error_rate_;
        } else {
            // Average error rate for Order 2
            return (false_positive_ + false_negative_) / 2.0;
        }
    }
    
    // Logical operations propagate errors
    bernoulli_bool operator&&(const bernoulli_bool& other) const {
        bool result = value_ && other.value_;
        
        if constexpr (Order == 1) {
            // Error propagation for AND under symmetric model
            double combined_error = error_rate_ + other.error_rate_ 
                                  - error_rate_ * other.error_rate_;
            return bernoulli_bool(result, combined_error);
        } else {
            // Complex error propagation for Order 2
            // This is a simplification; exact formula depends on value distribution
            double new_fp = false_positive_ * other.false_positive_;
            double new_fn = 1 - (1 - false_negative_) * (1 - other.false_negative_);
            return bernoulli_bool(result, new_fp, new_fn);
        }
    }
    
    bernoulli_bool operator||(const bernoulli_bool& other) const {
        bool result = value_ || other.value_;
        
        if constexpr (Order == 1) {
            double combined_error = error_rate_ + other.error_rate_ 
                                  - error_rate_ * other.error_rate_;
            return bernoulli_bool(result, combined_error);
        } else {
            double new_fp = 1 - (1 - false_positive_) * (1 - other.false_positive_);
            double new_fn = false_negative_ * other.false_negative_;
            return bernoulli_bool(result, new_fp, new_fn);
        }
    }
    
    bernoulli_bool operator!() const {
        if constexpr (Order == 1) {
            return bernoulli_bool(!value_, error_rate_);
        } else {
            // Negation swaps false positive and false negative rates
            return bernoulli_bool(!value_, false_negative_, false_positive_);
        }
    }
    
    // Equality returns a Bernoulli boolean!
    bernoulli_bool operator==(const bernoulli_bool& other) const {
        bool equal = (value_ == other.value_);
        
        if constexpr (Order == 1) {
            // Probability of incorrect equality result
            double eq_error = 2 * error_rate_ * other.error_rate_ 
                            * (1 - error_rate_) * (1 - other.error_rate_);
            return bernoulli_bool(equal, eq_error);
        } else {
            // More complex for Order 2
            return bernoulli_bool(equal, 0.1, 0.1); // Simplified
        }
    }
};

// Example: Three-valued logic with uncertainty
enum class tribool { false_val, true_val, unknown };

tribool to_tribool(const bernoulli_bool<1>& b, double confidence_threshold = 0.8) {
    if (b.error_rate() > (1.0 - confidence_threshold)) {
        return tribool::unknown;
    }
    return b.true_value() ? tribool::true_val : tribool::false_val;
}

// Example usage and testing
inline void demonstrate_bernoulli_bool() {
    std::cout << "=== Bernoulli Boolean Demonstration ===\n\n";
    
    // Order 1: Symmetric error
    std::cout << "Order 1 (Symmetric Error):\n";
    bernoulli_bool<1> noisy_true(true, 0.1);  // 10% error rate
    
    // Sample multiple times to see distribution
    int true_count = 0;
    const int samples = 10000;
    for (int i = 0; i < samples; ++i) {
        if (noisy_true.observe()) {
            true_count++;
        }
    }
    
    std::cout << "True value: " << noisy_true.true_value() << "\n";
    std::cout << "Observed true rate: " << std::fixed << std::setprecision(2) 
              << (100.0 * true_count / samples) << "% ";
    std::cout << "(expected ~90%)\n\n";
    
    // Order 2: Asymmetric error (like a Bloom filter bit)
    std::cout << "Order 2 (Asymmetric Error - Bloom filter style):\n";
    bernoulli_bool<2> bloom_bit(false, 0.01, 0.0);  // 1% FP, 0% FN
    
    int false_positives = 0;
    for (int i = 0; i < samples; ++i) {
        if (bloom_bit.observe()) {
            false_positives++;
        }
    }
    
    std::cout << "True value: " << bloom_bit.true_value() << "\n";
    std::cout << "False positive rate: " << std::fixed << std::setprecision(2)
              << (100.0 * false_positives / samples) << "% ";
    std::cout << "(expected ~1%)\n\n";
    
    // Logical operations
    std::cout << "Logical Operations with Error Propagation:\n";
    bernoulli_bool<1> a(true, 0.05);
    bernoulli_bool<1> b(false, 0.05);
    
    auto and_result = a && b;  // Should be false
    auto or_result = a || b;   // Should be true
    
    std::cout << "a (true±5%) && b (false±5%) = " 
              << and_result.true_value() << " with error rate " 
              << (100.0 * and_result.error_rate()) << "%\n";
    
    std::cout << "a (true±5%) || b (false±5%) = " 
              << or_result.true_value() << " with error rate " 
              << (100.0 * or_result.error_rate()) << "%\n\n";
    
    // Equality is probabilistic!
    std::cout << "Probabilistic Equality:\n";
    bernoulli_bool<1> x(true, 0.1);
    bernoulli_bool<1> y(true, 0.1);
    
    int equal_count = 0;
    for (int i = 0; i < samples; ++i) {
        auto eq = (x == y);
        if (eq.observe()) {
            equal_count++;
        }
    }
    
    std::cout << "x==y (both true±10%) observed equal " 
              << (100.0 * equal_count / samples) << "% of the time\n";
    std::cout << "(not 100% due to observation errors!)\n";
}

} // namespace bernoulli