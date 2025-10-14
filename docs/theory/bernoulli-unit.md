# Bernoulli Unit

The unit type, denoted by `()`, is the singleton type with exactly one value. This document explores the Bernoulli approximation `B_{()}` of the unit type.

## Theoretical Properties

**Unit Type Characteristics:**
- Cardinality: `|()| = 1` 
- Values: Just `()`
- No uncertainty possible in a singleton

## Bernoulli Unit Analysis

**Confusion Matrix:**
For `B_{()}`, we have a 1×1 confusion matrix:

| latent \ observed | `()` |
|-------------------|------|
| `()`              | 1    |

**Fundamental Measures:**
- **Order**: 0 (no free parameters)
- **Rank**: 1 (trivial 1×1 identity matrix)
- **Entropy**: H = 0 (no uncertainty)

## Why B_{()} = () 

Since there's only one possible value, there's no room for approximation or error:

```cpp
Pr{B_{()}(()) ≠ ()} = 0
```

Any "noise" in the unit type would have to map to... what? There are no other values available.

**Degenerate Case:**
The unit type represents the degenerate case where Bernoulli approximation becomes trivial:
- No approximation possible
- Perfect inference always
- Information-theoretically trivial

## Connection to Other Types

**Compositional Role:**
While `B_{()}` itself is trivial, it plays important roles in:

1. **Sum types**: `B_{X + ()}` ≈ optional types with noise
2. **Product types**: `B_{() × Y}` ≈ `B_Y` (unit contributes no information)
3. **Function types**: `B_{() → Y}` ≈ `B_Y` (constant functions)

**Algebraic Identity:**
The unit type serves as the multiplicative identity in type algebra:
- `X × () ≅ X`
- `B_X × B_{()} ≅ B_X`

## Implementation Note

In practice, `B_{()}` is useful for:
- **Theoretical completeness** of the Bernoulli type system
- **Compositional building blocks** where units appear in complex types
- **Degenerate case handling** in generic Bernoulli type implementations

The unit type demonstrates that Bernoulli approximation gracefully degenerates to perfect accuracy when no uncertainty is possible.