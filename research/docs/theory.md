# Bernoulli Types: Theory and Practice

## Table of Contents

1. [Introduction](#introduction)
2. [The Bernoulli Model](#the-bernoulli-model)
3. [Binary Channels and Information Theory](#binary-channels-and-information-theory)
4. [Bernoulli Sets](#bernoulli-sets)
5. [Bernoulli Maps](#bernoulli-maps)
6. [Practical Examples](#practical-examples)
7. [Implementation Considerations](#implementation-considerations)

## Introduction

The Bernoulli Model provides a unified framework for understanding probabilistic data structures and approximate computation. At its core, it formalizes the distinction between:

- **Latent values**: The true, mathematical objects we care about but cannot directly access
- **Observed values**: The noisy, computational approximations we work with in practice

This framework applies broadly:
- Bloom filters observe latent sets through membership queries with false positives
- Sketches observe latent distributions through space-bounded representations
- Randomized algorithms observe latent results through probabilistic computation

## The Bernoulli Model

### Core Concept

For any type `T`, we can define a Bernoulli type `B_T` (or `~T` in our notation) that represents noisy observations of values in `T`. The fundamental equation is:

```
P(~x ≠ x) = ε(x)
```

where `x` is the latent value and `~x` is its observation.

### Orders of Approximation

The **order** of a Bernoulli model describes the complexity of its error structure:

- **Order 0**: Perfect observation (no errors, `~x = x` always)
- **Order 1**: Symmetric errors (error rate independent of value)
- **Order 2**: Asymmetric errors (different rates for different value classes)
- **Order k**: Value-specific errors (up to k independent error parameters)

### Type Notation

We use several equivalent notations:
- `B_T^(k)`: k-th order Bernoulli type over T
- `~T`: Observed type T (order implicit)
- `bernoulli<T,k>`: C++ template notation

## Binary Channels and Information Theory

### Channel Correspondence

Every Bernoulli type corresponds to a discrete memoryless channel:

| Model | Channel Type | Error Structure |
|-------|--------------|-----------------|
| `B_bool^(1)` | Binary Symmetric Channel | P(error) = ε |
| `B_bool^(2)` | Binary Asymmetric Channel | P(0→1) = α, P(1→0) = β |

### Confusion Matrices

For a type T with n values, the confusion matrix Q captures all error behavior:

```
Q[i,j] = P(observe t_j | latent t_i)
```

Properties:
- Each row sums to 1 (stochastic matrix)
- Maximum order = n(n-1) (degrees of freedom)
- Diagonal dominance indicates quality

### Example: Boolean Confusion Matrix

For `B_bool^(2)` with false positive rate α and false negative rate β:

| latent \ observed | true | false |
|-------------------|------|-------|
| true              | 1-β  | β     |
| false             | α    | 1-α   |

## Bernoulli Sets

### The Model

A Bernoulli set `~S` observes a latent set `S` through noisy membership queries:

```
P(x ∈ ~S | x ∈ S) = 1 - β    (true positive rate)
P(x ∈ ~S | x ∉ S) = α        (false positive rate)
```

### Practical Realizations

1. **Bloom Filters**: Second-order model with β = 0 (no false negatives)
2. **Counting Bloom Filters**: Allow deletions but introduce false negatives
3. **Cuckoo Filters**: Support deletion with bounded false positive rates

### Space-Optimal Construction (HashSet)

Theoretical construction achieving information-theoretic bounds:

```
Given: Set S, hash function h: U × ℕ → {0,1}^k
1. Find seed s such that ∀x ∈ S: h(x,s) = 0^k
2. Membership test: x ∈? S ⟺ h(x,s) = 0^k

Properties:
- False positive rate: α = 2^(-k)
- Space: k bits per element (optimal)
- Construction time: O(2^(k|S|)) (exponential)
```

### Set Operations

Operations on Bernoulli sets produce new Bernoulli sets with composed error rates:

| Operation | False Positive Rate | False Negative Rate |
|-----------|-------------------|-------------------|
| A ∩ B | α_A · α_B | 1-(1-β_A)(1-β_B) |
| A ∪ B | 1-(1-α_A)(1-α_B) | β_A · β_B |
| Ā | β_A | α_A |

## Bernoulli Maps

### The Model

A Bernoulli map `~f: X → Y` observes a latent function `f: X → Y`:

```
P(~f(x) ≠ f(x)) = ε(x)
```

### Confusion Matrix for Functions

For functions of type `X → Y`, the confusion matrix has dimension `|Y|^|X| × |Y|^|X|`.

Example for `bool → bool` (4 possible functions):

| latent \ observed | id | not | true | false |
|-------------------|-----|-----|------|-------|
| id | 1-ε | ε/3 | ε/3 | ε/3 |
| not | ε/3 | 1-ε | ε/3 | ε/3 |
| true | ε/3 | ε/3 | 1-ε | ε/3 |
| false | ε/3 | ε/3 | ε/3 | 1-ε |

### Practical Examples

1. **Hash Functions**: Observe perfect discrimination through finite-range mapping
2. **Sketches**: Observe frequency distributions through space-bounded structures
3. **Approximate Nearest Neighbors**: Trade exactness for query speed

## Practical Examples

### Miller-Rabin Primality Test

The primality property becomes latent for large numbers due to computational constraints:

```
isPrime: ℕ → bool (latent - exponential time)
~isPrime_k: ℕ → bool (observed - polynomial time)

P(~isPrime_k(n) = "prime" | n is composite) ≤ 1/4^k
```

Key insight: We trade deterministic exponential time for probabilistic polynomial time.

### Function Equality Testing

Testing function equality through sampling:

```
Given: Functions f,g: X → Y
Test: Sample x₁,...,xₙ ~ Uniform(X)
      Accept if f(xᵢ) = g(xᵢ) for all i

Error rate: α = (1 - |D|/|X|)^n
where D = {x : f(x) ≠ g(x)}
```

### Software as Bernoulli Approximation

Philosophical view: Most software approximates some latent specification:
- Bugs introduce errors in the implementation
- Hardware failures create transient errors
- Finite precision approximates real arithmetic

## Implementation Considerations

### C++ Type System

```cpp
// Observed value with error interval
struct observed_bool {
    bool value;
    rate_span error;  // [min_error, max_error]
};

// Type-erased observed set
template <typename T>
class observed_set {
    virtual bool contains(T const&) const = 0;
    virtual rate_span fpr() const = 0;
    virtual rate_span fnr() const = 0;
};
```

### Error Propagation

When composing operations, errors compound:
- Use interval arithmetic for worst-case bounds
- Track error distributions for probabilistic guarantees
- Consider correlation between errors

### Design Principles

1. **Make approximation explicit**: Types should reflect uncertainty
2. **Propagate errors**: Operations should track error accumulation
3. **Provide guarantees**: Users need bounds on error rates
4. **Enable tradeoffs**: Allow tuning space/time/accuracy

## Advanced Topics

### Bayesian Inference

Given observation `~x`, infer the latent value:

```
P(x = t | ~x = t') = P(~x = t' | x = t) · P(x = t) / P(~x = t')
```

With uniform prior over boolean:
```
P(x = true | ~x = true) = (1-β) / (1-β + α)
```

### Multiple Observations

With k independent observations, majority vote improves accuracy:
```
P(error) ≤ exp(-2k(1/2 - ε)²)  (Hoeffding bound)
```

### Interval Arithmetic

When error rates are uncertain, represent as intervals:
```
[α] = [α_min, α_max]
[β] = [β_min, β_max]
```

Operations propagate uncertainty through interval arithmetic.

## Conclusion

The Bernoulli Model unifies many seemingly disparate concepts:
- Probabilistic data structures (Bloom filters, sketches)
- Randomized algorithms (Miller-Rabin, Monte Carlo)
- Approximate computing (lossy compression, quantization)
- Distributed systems (eventual consistency, Byzantine faults)

By making approximation a first-class concept at the type level, we can:
1. Reason formally about error propagation
2. Design systems with explicit accuracy/resource tradeoffs
3. Compose approximate components with predictable behavior
4. Quantify uncertainty throughout our computations

The latent/observed distinction is fundamental to bridging the gap between mathematical ideals and computational reality.