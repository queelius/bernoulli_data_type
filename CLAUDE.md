# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This repository implements the Bernoulli data type concept - a framework for understanding and constructing probabilistic data structures with controlled error rates. The project includes theoretical foundations, C++ implementations, and research papers exploring various aspects of Bernoulli types including sets, maps, booleans, and relations.

## Build and Development Commands

### CMake Build (Recommended)
```bash
# Configure build
cmake -B build -DBERNOULLI_BUILD_TESTS=ON -DBERNOULLI_BUILD_EXAMPLES=ON

# Build project
cmake --build build

# Run tests
cmake --build build --target run_tests
# or
cd build && ctest --output-on-failure

# Run specific test
cd build && ./tests/bernoulli_tests --gtest_filter="TestSuiteName.TestName"
```

### Manual Compilation
```bash
# Compile individual test files
g++ -std=c++20 -O3 -o test_hash_set tests/test_hash_set.cpp -I include -I ~/sources/algebraic_hash_functions/include -pthread

g++ -std=c++20 -O3 -o test_hash_map tests/test_hash_map.cpp -I include -I ~/sources/algebraic_hash_functions/include -pthread

# Quick header syntax check
make -f Makefile.test check-headers
```

### Documentation Generation
```bash
# Generate Doxygen documentation
doxygen Doxyfile

# Build LaTeX documentation (from docs/latex/)
cd docs/latex && make pdf
```

## Architecture and Key Concepts

### Core Namespaces
- `bernoulli` - Main namespace for Bernoulli data types
- `bernoulli::set::algebra` - Set algebra expressions and operations
- `coders` - Encoding/decoding utilities
- `std` extensions for Bloom filter implementations

### Type Hierarchy

1. **Bernoulli Boolean** (`bernoulli_bool/`)
   - Simplest Bernoulli type modeling probabilistic boolean values
   - Supports different orders (0th = perfect, 1st = symmetric errors, 2nd+ = asymmetric)

2. **Bernoulli Set** (`bernoulli_set/`)
   - Type-erased probabilistic set with false positive/negative rates
   - Expression templates for set operations (union, intersection, complement, etc.)
   - Bloom filter as a special case (false positives only)

3. **Bernoulli Map** (`bernoulli_map/`)
   - Generalizes to model any computable function with errors
   - Includes hash map implementations with probabilistic guarantees

4. **Bernoulli Relations** (`bernoulli_relation/`)
   - Models relations with probabilistic properties
   - Specialized types for reflexive and symmetric relations

### Key Design Patterns

- **Type Erasure**: `bernoulli_set<X>` uses type erasure to allow heterogeneous collections
- **Expression Templates**: Set operations use expression templates for lazy evaluation
- **Concept-based Design**: Heavy use of C++20 concepts for type constraints
- **Header-only Library**: All implementations in `.hpp` files under `include/`
- **Rate Spans**: Error rates modeled as intervals `[min, max]` to capture uncertainty

### External Dependencies

- Requires `algebraic_hash_functions` library (expected at `~/sources/algebraic_hash_functions/include`)
- C++20 compiler required (though CMake specifies C++17 minimum)
- pthread for multi-threading support
- Google Test (fetched automatically via CMake for tests)

### Important Implementation Notes

- Bernoulli types intentionally violate regular type requirements (e.g., equality returns Bernoulli Boolean)
- False positive/negative rates are modeled as intervals to capture uncertainty
- The `deprecated/` folder contains research papers and older implementations that are still relevant for understanding the theoretical foundations
- Hash set/map implementations include both single-level and two-level variants

### Testing Structure

- Tests use Google Test framework (automatically fetched by CMake)
- Test files in `tests/`:
  - `test_main.cpp` - Test runner entry point
  - `test_rate_span.cpp` - Tests for rate span interval arithmetic
  - `test_observed_bool.cpp` - Tests for observed Bernoulli booleans
  - `test_observed_set.cpp` - Tests for observed Bernoulli sets
  - `test_observed_map.cpp` - Tests for observed Bernoulli maps
  - `test_bloom_filter.cpp` - Tests for Bloom filter implementation
  - `test_hash_set.cpp` - Hash set implementation tests
  - `test_hash_map.cpp` - Hash map implementation tests