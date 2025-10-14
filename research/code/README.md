# Bernoulli Type Implementation Examples

This directory contains C++ implementations demonstrating key concepts from the Bernoulli type framework.

## Files

### `bernoulli_bool_example.hpp`
- Implementation of Bernoulli booleans with symmetric (Order 1) and asymmetric (Order 2) errors
- Demonstrates error propagation through logical operations
- Shows how equality becomes probabilistic
- Includes conversion to three-valued logic for handling uncertainty

### `bernoulli_set_example.hpp`
- Bloom filter implementation as a positive Bernoulli set (false positives only)
- General Bernoulli set with both false positives and false negatives
- Set operations with error propagation
- Binary classification metrics (precision, recall, F1)

### `bernoulli_map_example.hpp` (coming soon)
- Hash functions as Bernoulli maps
- Miller-Rabin primality test implementation
- Count-Min sketch for frequency estimation
- Monte Carlo integration

## Usage

These are header-only implementations. To use them:

```cpp
#include "bernoulli_bool_example.hpp"
#include "bernoulli_set_example.hpp"

int main() {
    bernoulli::demonstrate_bernoulli_bool();
    bernoulli::demonstrate_bernoulli_sets();
    return 0;
}
```

## Compilation

Requires C++17 or later:

```bash
g++ -std=c++17 -O3 -o demo demo.cpp
./demo
```

## Key Concepts Demonstrated

1. **Error Models**: How different orders capture different error patterns
2. **Error Propagation**: How errors compound through operations
3. **Probabilistic Equality**: Why `x == x` might return false
4. **Space-Accuracy Trade-offs**: Bloom filter efficiency
5. **Information Theory**: Relating error rates to information loss

## Extending the Examples

These implementations are simplified for clarity. Production versions would need:
- Thread safety
- Serialization support
- More sophisticated hash functions
- Optimal parameter selection
- Integration with existing STL containers

## Research Directions

- Implement higher-order Bernoulli types (Order > 2)
- Explore automatic error rate inference
- Develop Bernoulli-aware algorithms
- Create debugging tools for probabilistic programs