# Entropy Maps Paper

This paper presents entropy maps as a theoretical framework for understanding and implementing probabilistic data structures through the lens of information theory.

## Building

To compile the paper:
```bash
pdflatex main.tex
pdflatex main.tex  # Run twice to resolve references
```

## Abstract

Entropy maps provide a unified framework for probabilistic data structures by mapping domain values to codomain values using hash functions and prefix-free codes. This enables space-efficient representations with controlled error rates, naturally giving rise to Bernoulli maps—probabilistic approximations with well-characterized error distributions.

## Key Contributions

- Formal framework for entropy maps based on prefix-free coding
- Concept of Bernoulli maps as probabilistic function approximations
- Analysis of confusion matrices and degrees of freedom
- Practical algorithms for implementation
- Connections to Bloom filters and other probabilistic data structures