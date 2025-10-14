# Bernoulli Data Type

A general framework for understanding and constructing probabilistic data structures with controlled error rates. This framework can also serve as a foundation for constructing oblivious programs as a composition of oblivious data types and oblivious functions.

## Overview

The Bernoulli data type concept provides a formalism for reasoning about approximations of data structures that trade space and time complexity for bounded error rates. This framework unifies many existing probabilistic data structures (like Bloom filters) and enables the systematic construction of new ones.

## Quick Start

```bash
# Configure build with tests
cmake -B build -DBERNOULLI_BUILD_TESTS=ON -DBERNOULLI_BUILD_EXAMPLES=ON

# Build
cmake --build build

# Run tests
cd build && ctest --output-on-failure
```

## Documentation

📚 **[Full Documentation](https://oblivious-computing.github.io/bernoulli_data_type/)** (or run `mkdocs serve` locally)

### Key Documentation

- **Theory**
  - [Bernoulli Model Overview](docs/theory/bernoulli-model.md) - Foundational concepts
  - [Bernoulli Boolean](docs/theory/bernoulli-bool.md) - Simplest Bernoulli type
  - [Bernoulli Set](docs/theory/bernoulli-set.md) - Probabilistic sets (includes Bloom filters)
  - [Bernoulli Map](docs/theory/bernoulli-map.md) - Approximate functions (includes Miller-Rabin as example)
  - [Bernoulli Tuple](docs/theory/bernoulli-tuple.md) - Product types with approximation
  - [Bernoulli Unit](docs/theory/bernoulli-unit.md) & [Void](docs/theory/bernoulli-void.md) - Degenerate cases

- **Implementation**
  - [Codecs](docs/implementation/codec.md) - Encoding/decoding strategies
  - [Regular Types](docs/implementation/regular-types.md) - Type theory considerations

- **API Reference**
  - [C++ API Documentation](docs/html/index.html) (Doxygen)

### Local Documentation Server

```bash
# Install MkDocs with Material theme
pip install mkdocs-material

# Serve documentation locally at http://127.0.0.1:8000
mkdocs serve
```

## Project Structure

```
bernoulli_data_type/
├── include/              # Header-only C++ library
│   ├── bernoulli_bool/   # Bernoulli boolean types
│   ├── bernoulli_set/    # Bernoulli set types (Bloom filters, etc.)
│   ├── bernoulli_map/    # Bernoulli map/function types
│   └── ...
├── tests/                # Google Test suite
├── docs/                 # MkDocs documentation
│   ├── theory/           # Theoretical foundations
│   ├── implementation/   # Implementation guides
│   └── api/              # API reference (Doxygen output)
├── papers-latex/         # Academic papers (LaTeX sources)
└── research/             # Research notes and experiments
```

## About

This project is under active development. For more information, visit [metafunctor.com](https://metafunctor.com).


