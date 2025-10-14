# Bernoulli Void

The void type, denoted by `∅` or `Void`, is the empty type with no values. This document explores the theoretical properties of `B_Void`.

## Theoretical Properties

**Void Type Characteristics:**
- Cardinality: `|Void| = 0`
- Values: None (cannot construct values of this type)
- No functions out of void except the vacuous function

## Bernoulli Void Analysis

**Confusion Matrix:**
For `B_Void`, we have a 0×0 confusion matrix:

```
(empty matrix)
```

**Fundamental Measures:**
- **Order**: 0 (no entries, no parameters)
- **Rank**: 0 (empty matrix has rank 0)  
- **Entropy**: H = 0 (no values to be uncertain about)

## Why B_Void = Void

Since there are no values of the void type, there's nothing to approximate:

**No Values to Observe:**
- Can't construct `Void` values to begin with
- Can't construct `B_Void` observations 
- The approximation question is vacuous

**Vacuous Truth:**
Any statement about `B_Void` approximation is vacuously true since there are no instances to verify:
```cpp
∀x ∈ Void: Pr{B_Void(x) ≠ x} = ε  // vacuously true for any ε
```

## Connection to Other Types

**Compositional Role:**
The void type plays crucial theoretical roles:

1. **Sum types**: `B_{Void + X}` ≅ `B_X` (void contributes nothing)
2. **Product types**: `B_{Void × X}` ≅ `B_Void` (void annihilates everything)  
3. **Function types**: 
   - `B_{Void → X}` has cardinality 1 (unique vacuous function)
   - `B_{X → Void}` represents partial functions (possibly undefined)

**Algebraic Zero:**
The void type serves as the additive identity and multiplicative zero:
- `X + Void ≅ X` (additive identity)
- `X × Void ≅ Void` (multiplicative zero)
- `B_X + B_Void ≅ B_X`
- `B_X × B_Void ≅ B_Void`

## Practical Significance

**Error Handling:**
In implementations, `B_Void` represents:
- **Impossible cases** in pattern matching
- **Error states** that shouldn't occur
- **Compiler-enforced impossibility**

**Type System Completeness:**
`B_Void` ensures the Bernoulli type system is:
- **Algebraically complete** (all type constructors defined)
- **Compositionally sound** (operations compose consistently)
- **Theoretically elegant** (no special cases needed)

## Information Theory

**Degenerate Information:**
- **No information content** (can't observe anything)
- **No uncertainty** (no values to be uncertain about)  
- **Vacuous inference** (all inference questions are meaningless)

The void type represents the ultimate degenerate case where approximation, observation, and inference all become vacuous concepts due to the absence of any values to approximate.