# Bernoulli Data Type

Welcome to the Bernoulli Data Type documentation. This project presents a general framework for understanding and constructing probabilistic data structures with controlled error rates.

## Overview

The Bernoulli data type concept provides a formalism for reasoning about approximations of data structures that trade space and time complexity for bounded error rates. This framework unifies many existing probabilistic data structures (like Bloom filters) and enables the systematic construction of new ones.

## Key Concepts

**Bernoulli Types** are approximations of standard data types where operations may produce errors with known probability bounds. For any type `T`, we denote its Bernoulli approximation as `bernoulli<T>`.

**Error Models** are characterized by:
- **Order**: The number of independent error parameters
- **Rank**: The fundamental inferability of latent values
- **Entropy**: The information-theoretic uncertainty

**Applications** include:
- Space-efficient probabilistic data structures
- Oblivious computing and privacy-preserving algorithms
- Approximate query processing with bounded error guarantees
- Trade-offs between accuracy and resource consumption

## Quick Start

### Theory

Start with the [Bernoulli Model Overview](theory/bernoulli-model.md) to understand the foundational concepts. Then explore specific types:

- [**Bernoulli Boolean**](theory/bernoulli-bool.md) - The simplest Bernoulli type, modeling noisy boolean values
- [**Bernoulli Set**](theory/bernoulli-set.md) - Probabilistic set membership (includes Bloom filters)
- [**Bernoulli Map**](theory/bernoulli-map.md) - Approximate functions with bounded error
- [**Bernoulli Tuple**](theory/bernoulli-tuple.md) - Product types with approximation
- [**Bernoulli Unit**](theory/bernoulli-unit.md) - Degenerate case with no uncertainty
- [**Bernoulli Void**](theory/bernoulli-void.md) - Empty type analysis

### Implementation

Explore practical aspects:

- [**Codecs**](implementation/codec.md) - Encoding and decoding strategies
- [**Regular Types**](implementation/regular-types.md) - Why Bernoulli types violate regularity

### API Reference

See the [C++ API Documentation](api/html/index.html) for implementation details (generated via Doxygen).

## Building the Project

```bash
# Configure build with tests
cmake -B build -DBERNOULLI_BUILD_TESTS=ON -DBERNOULLI_BUILD_EXAMPLES=ON

# Build
cmake --build build

# Run tests
cd build && ctest --output-on-failure
```

## Documentation

This documentation is built with [MkDocs](https://www.mkdocs.org/) using the Material theme.

```bash
# Install MkDocs with Material theme
pip install mkdocs-material

# Serve documentation locally
mkdocs serve

# Build static site
mkdocs build
```

## Project Structure

```
bernoulli_data_type/
├── include/              # Header-only C++ library
│   ├── bernoulli_bool/
│   ├── bernoulli_set/
│   ├── bernoulli_map/
│   └── ...
├── tests/                # Test suite
├── docs/                 # Documentation
│   ├── theory/           # Theoretical foundations
│   ├── implementation/   # Implementation guides
│   └── api/              # API reference (Doxygen)
├── papers-latex/         # Academic papers (LaTeX sources)
└── research/             # Research notes and experiments
```

## Contributing

This project is under active development. See the [GitHub repository](https://github.com/oblivious-computing/bernoulli_data_type) for the latest updates.

## License

See the LICENSE file in the repository for licensing information.

## About

The Bernoulli data type framework was developed to provide a rigorous foundation for probabilistic data structures and oblivious computing. It generalizes concepts from information theory, binary classification, and type theory into a unified framework for approximate computation.
