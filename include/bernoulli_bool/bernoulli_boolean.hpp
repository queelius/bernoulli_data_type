#pragma once

#include "../rate_span.hpp"
#include <algorithm>

/**
 * @brief Observed boolean type representing ~b where b is the latent boolean.
 * 
 * In the latent/observed framework:
 * - b represents the true/latent boolean value
 * - ~b represents the observed boolean which may differ from b with some error rate
 * 
 * The error rate represents P(~b ≠ b), i.e., the probability that the
 * observed value differs from the latent value.
 */
struct observed_bool
{
    bool value;           // The observed value
    rate_span error;      // Error rate interval [min, max]
    
    observed_bool(bool v, rate_span err = rate_span(0.0f)) 
        : value(v), error(err) {}
    
    observed_bool(bool v, float err) 
        : value(v), error(err) {}
    
    operator bool() const { return value; }
};

// Type alias for backward compatibility
using bernoulli_bool = observed_bool;

/**
 * Logical NOT for observed booleans.
 * The error rate remains the same since NOT is deterministic.
 */
inline auto operator~(observed_bool a)
{
    return observed_bool{!a.value, a.error};
}

/**
 * Logical AND for observed booleans.
 * Error propagation follows from probability theory.
 */
inline auto operator&(observed_bool a, observed_bool b)
{
    if (a.value && b.value) {
        // Both observed as true
        // Error occurs if either was actually false
        return observed_bool{true, a.error + b.error - a.error * b.error};
    }
    
    // At least one observed as false
    // Error analysis is more complex due to multiple cases
    auto err1 = b.error - a.error * b.error;
    auto err2 = a.error - a.error * b.error;
    auto err3 = rate_span(1.0f) - a.error - b.error + a.error * b.error;
    
    rate_span combined_error(
        std::min({err1.min, err2.min, err3.min}),
        std::max({err1.max, err2.max, err3.max})
    );
    
    return observed_bool{false, combined_error};
}

/**
 * Logical OR for observed booleans.
 * Implemented using De Morgan's law: a | b = ~(~a & ~b)
 */
inline auto operator|(observed_bool a, observed_bool b)
{
    return ~(~a & ~b);
}

/**
 * Logical NOR for observed booleans.
 */
inline auto nor(observed_bool a, observed_bool b)
{
    return ~(a | b);
}

/**
 * Logical NAND for observed booleans.
 */
inline auto nand(observed_bool a, observed_bool b)
{
    return ~(a & b);
}

/**
 * Logical XOR for observed booleans.
 * Implemented as (~a & b) | (a & ~b)
 */
inline auto operator^(observed_bool a, observed_bool b)
{
    return (~a & b) | (a & ~b);
}

/**
 * Logical XNOR (equivalence) for observed booleans.
 */
inline auto xnor(observed_bool a, observed_bool b)
{
    return ~(a ^ b);
}