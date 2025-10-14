# Bernoulli Tuples  

This document explores Bernoulli approximations of tuple types (product types), demonstrating two distinct approaches: tuples of Bernoulli values versus Bernoulli tuples. We focus on pairs for concrete examples, but the principles generalize to n-tuples.

## Two Approaches to Approximate Pairs

Consider the pair type `(bool, bool)` with values like `(true, false)`. There are two fundamentally different ways to introduce Bernoulli approximation:

### 1. Pair of Bernoulli Bools: `(B_bool, B_bool)`

This is a **compositional approach** where we first construct Bernoulli approximations of each component, then pair them.

```cpp
// Latent pair
(x1, x2) where x1, x2 ∈ bool

// Pair of Bernoulli observations  
(B_bool(x1), B_bool(x2))
```

**Construction Process:**
1. Start with latent values `x1` and `x2`
2. Independently observe `B_bool(x1)` and `B_bool(x2)` 
3. Construct the pair `(B_bool(x1), B_bool(x2))`

**Error Characteristics:**
- Each component has its own error rate: `Pr{B_bool(x1) ≠ x1} = ε1`, `Pr{B_bool(x2) ≠ x2} = ε2`
- Errors are independent between components
- The pair is correct iff both components are correct: `Pr{pair correct} = (1-ε1)(1-ε2)`

### 2. Bernoulli Pair of Bools: `B_{(bool,bool)}`

This is a **direct approximation** of the pair type itself, treating the pair as a single unit.

```cpp
// Latent pair
p = (x1, x2) where p ∈ (bool, bool)

// Bernoulli approximation of the pair
B_{(bool,bool)}(p) 
```

**Construction Process:**
1. Start with latent pair `p = (x1, x2)`
2. Directly construct approximation `B_{(bool,bool)}(p)` using some process (e.g., hash-based, channel-based)
3. Extract components via `first(B_{(bool,bool)}(p))` and `second(B_{(bool,bool)}(p))`

**Error Characteristics:**
- Single error function: `Pr{B_{(bool,bool)}(p) ≠ p} = Error(p)`
- Can have correlation between component errors
- Allows different error patterns than independent component errors

## Detailed Analysis

### Case 1: Pair of Bernoulli Bools

**Individual Component Confusion Matrices:**

First component: `B_bool^{(1)}` (first-order) with symmetric error ε₁:
| latent \ observed | T | F |
|-------------------|---|---|
| T | 1-ε₁ | ε₁ |
| F | ε₁ | 1-ε₁ |

Second component: `B_bool^{(1)}` (first-order) with symmetric error ε₂:
| latent \ observed | T | F |
|-------------------|---|---|
| T | 1-ε₂ | ε₂ |
| F | ε₂ | 1-ε₂ |

**Induced Confusion Matrix for `(B_bool^{(1)}, B_bool^{(1)})`:**

| latent \ observed | `(T,T)` | `(T,F)` | `(F,T)` | `(F,F)` |
|-------------------|---------|---------|---------|---------|
| `(T,T)` | `(1-ε₁)(1-ε₂)` | `(1-ε₁)ε₂` | `ε₁(1-ε₂)` | `ε₁ε₂` |
| `(T,F)` | `(1-ε₁)ε₂` | `(1-ε₁)(1-ε₂)` | `ε₁ε₂` | `ε₁(1-ε₂)` |
| `(F,T)` | `ε₁(1-ε₂)` | `ε₁ε₂` | `(1-ε₁)(1-ε₂)` | `(1-ε₁)ε₂` |
| `(F,F)` | `ε₁ε₂` | `ε₁(1-ε₂)` | `(1-ε₁)ε₂` | `(1-ε₁)(1-ε₂)` |

**Why is this Order 2?**

The 4×4 confusion matrix above is fully determined by just 2 parameters: ε₁ and ε₂. Here's why:

1. **Maximum possible order**: 4×4 matrix with row-sum constraint gives 4(4-1) = 12 degrees of freedom
2. **Actual order**: Only 2 free parameters due to independence constraint
3. **Structure**: Each entry is a product `P₁ᵢⱼ × P₂ₖₗ` where Pᵢⱼ comes from component confusion matrices

**Special Case - Identical Components (ε₁ = ε₂ = ε):**

| latent \ observed | `(T,T)` | `(T,F)` | `(F,T)` | `(F,F)` |
|-------------------|---------|---------|---------|---------|
| `(T,T)` | `(1-ε)²` | `(1-ε)ε` | `ε(1-ε)` | `ε²` |
| `(T,F)` | `(1-ε)ε` | `(1-ε)²` | `ε²` | `ε(1-ε)` |
| `(F,T)` | `ε(1-ε)` | `ε²` | `(1-ε)²` | `(1-ε)ε` |
| `(F,F)` | `ε²` | `ε(1-ε)` | `(1-ε)ε` | `(1-ε)²` |

This becomes **order 1** with just parameter ε, showing perfect symmetry.

**Rank Analysis:**

The rank of the confusion matrix captures something more fundamental than parameter count:

For different components (ε₁ ≠ ε₂):
- **Parameter count**: 2
- **Rank**: 4 (all entries are unique products)
- **Inference**: Full rank → perfect Bayesian inference theoretically possible

For identical components (ε₁ = ε₂ = ε):
- **Parameter count**: 1  
- **Rank**: 3 when ε ≠ 0.5, rank 1 when ε = 0.5
- **Inference**: Rank 3 allows inference, rank 1 (maximum entropy) makes inference impossible

**Key insight**: Parameter count affects estimation difficulty, but **rank determines fundamental inferability** - whether P(latent|observed) can be computed even with perfect parameter knowledge.

## Information-Theoretic Analysis

Adding entropy as a third fundamental measure alongside order and rank:

**Matrix Entropy H(confusion matrix):**
- **Different components** (ε₁ ≠ ε₂): All 16 entries unique → higher entropy
- **Identical components** (ε₁ = ε₂ = ε): Only 3 unique values → lower entropy
- **Maximum entropy case** (ε = 0.5): Uniform confusion → H maximized

**Information Hierarchy for Compositional vs Direct:**

**Compositional (B_bool, B_bool):**
1. Unconstrained 4×4 matrix: maximum entropy space
2. Independence constraint: entropy reduced (Kronecker product structure)
3. Parameter knowledge (ε₁, ε₂): further entropy reduction
4. Observations: conditional entropy H(latent|observed)

**Direct B_{(bool,bool)}:**
1. Unconstrained 4×4 matrix: same maximum entropy space  
2. Structural constraints (α, β parameters): different entropy reduction
3. Can achieve entropy patterns impossible under independence

**Entropy and Inference Quality:**
- Lower matrix entropy → more structured approximation
- Independence constraint creates highly structured (low entropy) confusion matrices
- Direct approximation allows higher entropy (less structured) patterns
- Rank-1 cases correspond to maximum conditional entropy H(latent|observed)

## Confusion Matrix Scale Analysis

**Small tuples**: For `(bool, bool)` we have 4 possible values, so 4×4 confusion matrix is manageable.

**Large tuples**: For `(X, Y)` where X, Y are large types, confusion matrix becomes `|X×Y| × |X×Y|` which is typically enormous. However:

**Compositional structure creates patterns**:
- Independence constraint forces Kronecker product structure
- Sparsity determined by component impossibilities  
- Rank properties inherited from component ranks
- Block structure enables targeted analysis of sub-problems

**Direct approximation allows arbitrary patterns**:
- Can break independence to create correlations impossible compositionally
- May achieve different sparsity patterns optimized for specific use cases
- Rank unconstrained by component limitations

This structural perspective explains why compositional and direct approximation lead to fundamentally different inference capabilities even for the same underlying types.

### Case 2: Bernoulli Pair of Bools

This allows much richer error patterns. For example, a second-order model might have different error rates for different pairs:

| Latent Pair | Error Rate | 
|-------------|-----------|
| `(T,T)`     | `ε₁`      |
| `(T,F)`     | `ε₂`      |
| `(F,T)`     | `ε₃`      |
| `(F,F)`     | `ε₄`      |

**Confusion Matrix for B_{(bool,bool)}:**

For the direct Bernoulli approximation of pairs, we have the latent/observed confusion matrix:

| latent \ observed | `(T,T)` | `(T,F)` | `(F,T)` | `(F,F)` |
|-------------------|---------|---------|---------|---------|
| `(T,T)`           | `q11`   | `q12`   | `q13`   | `q14`   |
| `(T,F)`           | `q21`   | `q22`   | `q23`   | `q24`   |
| `(F,T)`           | `q31`   | `q32`   | `q33`   | `q34`   |
| `(F,F)`           | `q41`   | `q42`   | `q43`   | `q44`   |

Where `qij = Pr{observe pair_j | latent pair_i}` and each row sums to 1.

**Maximum Order:**
- There are 4 possible pairs in `(bool, bool)`
- Maximum confusion matrix is 4×4 with 4(4-1) = 12 degrees of freedom
- So `B_{(bool,bool)}^{(12)}` is the highest-order model

**Example: Parameterized Confusion Matrix**

A practical second-order model with parameters α (spurious rate) and β (miss rate):

| latent \ observed | `(T,T)` | `(T,F)` | `(F,T)` | `(F,F)` |
|-------------------|---------|---------|---------|---------|
| `(T,T)`           | `1-β`   | `β/3`   | `β/3`   | `β/3`   |
| `(T,F)`           | `α/3`   | `1-α`   | `α/3`   | `α/3`   |
| `(F,T)`           | `α/3`   | `α/3`   | `1-α`   | `α/3`   |
| `(F,F)`           | `α/3`   | `α/3`   | `α/3`   | `1-α`   |

Where:
- `α` = spurious rate (probability of observing wrong pair when latent is non-target)  
- `β` = miss rate (probability of missing the correct pair)
- Off-diagonal errors are distributed uniformly among wrong options

This introduces correlations impossible with independent component errors while maintaining tractable parameterization.

## Set-Theoretic Interpretation

We can view pairs as characteristic functions over the product space:

### Pair of Bernoulli Bools
```cpp
χ₁: {0,1} → B_bool  // First component indicator  
χ₂: {0,1} → B_bool  // Second component indicator
pair_membership(i,j) = (χ₁(i), χ₂(j))
```

### Bernoulli Pair  
```cpp
Χ: {0,1}×{0,1} → B_bool  // Joint indicator over product space
Χ = B_{({0,1}×{0,1}) → bool}(characteristic_function)
```

## Implementation Considerations

### Memory Layout
- **Pair of Bernoulli Bools**: Store two separate Bernoulli values
- **Bernoulli Pair**: Store single approximation + extraction functions

### Error Propagation
- **Compositional**: Errors compose via probability arithmetic
- **Direct**: Single error model for the entire structure

### Use Cases

**Pair of Bernoulli Bools:**
- When components come from independent noisy sources
- When you need to operate on components separately
- Bloom filter pairs, independent sensor readings

**Bernoulli Pair:**  
- When space efficiency requires joint compression
- When error patterns have natural correlation
- Hash-based approximate pairs, encrypted/oblivious pairs

## Connection to General Framework

This demonstrates the general pattern for any product type `A × B`:

1. **Compositional**: `(B_A, B_B)` - approximate components first, then compose
2. **Direct**: `B_{A×B}` - approximate the product type directly

Both are valid Bernoulli types with different error characteristics and use cases. The choice depends on whether you want independent component errors or allow correlation through joint approximation.

## Future Work

Similar patterns apply to:
- **Sum types**: `B_A + B_B` vs `B_{A+B}`  
- **Function types**: `B_A → B_B` vs `B_{A→B}`
- **Higher-order products**: `B_A × B_B × B_C` vs `B_{A×B×C}`

The systematic exploration of these compositional vs direct approximation patterns forms the foundation of the complete Bernoulli type system.