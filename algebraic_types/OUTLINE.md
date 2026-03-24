# Outline — Random Approximate Values over Algebraic Types

> Generated 2026-02-25 by `/papermill:outline`. This is a proposed reorganization of existing content into a coherent narrative.

## Central claim

Every algebraic data type constructor (void, unit, sum, product, exponential) has a well-defined random approximate counterpart. Approximation distributes over these constructors in a principled way, yielding a complete algebra of approximate types with quantifiable error propagation. This unifies Bernoulli sets, maps, and relations as special cases of a single type-theoretic framework.

## Narrative arc

```
[1] Motivation: we have approximate sets and maps — what about arbitrary data types?
        ↓
[2] Background: types = sets, type constructors = set operations
        ↓
[3] Core definition: what is a random approximate value?
        ↓
[4] Walk the type hierarchy bottom-up: void → unit → Bool → sum → product → exponential
        ↓
[5] Error propagation: how approximation composes through type constructors
        ↓
[6] The approximate value monad: computational realization
        ↓
[7] Data types with invariants: when approximation meets constraints
        ↓
[8] Discussion: approximate programs, connections, future work
```

---

## Section-by-section outline

### 1. Introduction
**File**: `sections/intro.tex`
**Purpose**: Motivate why algebraic types need a random approximation theory. Situate this paper in the Bernoulli framework (sets → maps → relations → **types**).
**Key arguments**:
- Bernoulli sets show that membership queries return approximate Booleans
- Bernoulli maps show that function application propagates error
- But real programs compose *types* — products, sums, optionals, records — not just sets and functions
- This paper provides the missing piece: a theory of approximate values over the full algebra of types

**Existing content**: 15 lines — defines types, values, ADTs. Incomplete sentences.
**Gap**: Needs motivation, related work sketch, paper roadmap. The "why" is entirely missing.

---

### 2. Preliminaries: Types as sets
**File**: `sections/prelim.tex` (new — content scattered across existing files)
**Purpose**: Establish the classical algebraic type system that will be "approximated."
**Key arguments**:
- A *type* is a set; *values* are elements
- **Void** (∅): 0 values. **Unit** ({*}): 1 value. **Bool** ({⊤,⊥}): 2 values.
- **Sum type** (A + B): disjoint union, |A| + |B| values
- **Product type** (A × B): Cartesian product, |A| · |B| values
- **Exponential type** (A → B): function space, |B|^|A| values
- A *data type* is a type equipped with invariants (a relation/subset)
- An *abstract data type* is a type + a computational basis (set of operations)
- C++ correspondence: `variant` = sum, `tuple`/`struct` = product, lambdas = exponential, `optional<X>` = X + unit

**Existing content**: `cpp.tex` lines 11–61 (C++ type catalog), `bernoulli_type_2.tex` lines 43–52 (brief type definitions), `prims.tex` lines 9–22 (void and unit)
**Gap**: Needs consolidation into one coherent section. The C++ examples are pedagogically useful but should follow (not replace) the mathematical definitions.

---

### 3. Random approximate values
**File**: `sections/approx_values.tex` (new — content from intro.tex + bernoulli_type_2.tex)
**Purpose**: Define $\AT{T}$ — what it means for a value of type T to be approximate.
**Key arguments**:
- A value $x \in T$ is *approximate* if there exists a latent (true) value $x^* \in T$ and $x$ may differ from $x^*$ with quantifiable probability
- The primary mechanism: an approximate map $X \amapsto{\varepsilon}{\eta} Y$ maps exact inputs to approximate outputs
- Notation: $\AT{T}[\varepsilon][\eta]$ denotes the approximate value type parameterized by FPR/FNR
- $\AT{T}$ is a *monad*: approximate values compose (applying a function to an approximate value yields an approximate value)
- **Definition** (random approximate value): formal statement

**Existing content**: `intro.tex` lines 7–15 (sketch), `bernoulli_type_2.tex` lines 4–40 (BitSet worked example with conditional PMFs)
**Gap**: Needs a clean formal definition. The BitSet example is good but belongs after the definition as a worked example. Currently mixes definition with example.

---

### 4. Primitive approximate types
**File**: `sections/prims.tex` (exists — needs reorganization)
**Purpose**: Walk through each primitive type constructor and define its approximate counterpart. Build intuition bottom-up from trivial (void, unit) to non-trivial (Bool).
**Key arguments**:
- **$\AT{\mathrm{Void}} \equiv \mathrm{Void}$**: No values to approximate. Completes the algebra.
- **$\AT{\mathrm{Unit}} \equiv \mathrm{Unit}$**: One value, no uncertainty possible. But $\APFun{f} : \mathrm{Unit} \to X$ models an *approximate constant*.
- **$\AT{\mathrm{Bool}}$**: The first non-trivial case. Connects to the Bernoulli Boolean model (symmetric/asymmetric channels, FPR/FNR). This is where `bernoulli_model/` and `bernoulli_sets/` intersect — cite them, don't re-derive.
- Observation: types with ≤1 value are trivially exact; approximation requires |T| ≥ 2.

**Existing content**: `prims.tex` lines 9–26 (void and unit — solid), lines 28–49 (sum/product/exponential — should move to §5)
**Gap**: Bool subsection doesn't exist here. It's covered in `bernoulli_model/` but this paper should at minimum state the connection and give the confusion matrix.

---

### 5. Composite approximate types
**File**: `sections/composite_types.tex` (new — content from prims.tex + bernoulli_boolean_algebra.tex)
**Purpose**: The main technical contribution — how approximation interacts with type constructors.
**Key arguments**:
- **Sum types**: Three distinct notions:
  - $\AT{(X + Y)}$: first-order, the *tag itself* may be wrong (value appears in wrong variant)
  - $\AT{X} + \AT{Y}$: higher-order, tag is correct but component values are approximate
  - $\AT{(\AT{X} + \AT{Y})}$: both tag and values are approximate
  - Connection to `optional<X>` = X + Unit: an approximate optional may falsely report "nothing" or falsely report a value
- **Product types**:
  - $\AT{X} \times \AT{Y}$: component-wise approximation, errors are independent
  - Connection to invariants: a product type with constraints may produce *invalid tuples*
  - Key question: $\Pr[\text{approximate value violates invariant}]$
- **Exponential types**:
  - $\AT{(X \to Y)}$ = approximate maps (see `bernoulli_maps/`)
  - The lifting perspective: a function $f : X \to Y$ applied to $\AT{X}$ yields $\AT{Y}$
  - This is the monadic `bind` operation
- **Boolean algebra over bit-vectors**: $(\AT{\{0,1\}^n}, \AT{\wedge}, \AT{\vee}, \AT{\neg}, \AT{1^n}, \AT{0^n})$ as a six-tuple. Isomorphism to sets over a universe of $n$ elements.

**Existing content**: `prims.tex` lines 28–49 (sum, product, exponential stubs), `bernoulli_boolean_algebra.tex` lines 4–44 (Boolean algebra 6-tuple, bit-vector isomorphism, partial function discussion), `bernoulli_type_2.tex` lines 38–52 (exponential = map, invariants)
**Gap**: Sum type treatment is the thinnest — needs the tag-error vs value-error distinction worked out with examples. Product type needs the invariant violation probability formalized. The Boolean algebra section has promising content but is fragmentary.

---

### 6. Error propagation through type constructors
**File**: `sections/error_propagation.tex` (new — content from bernoulli_type_2.tex)
**Purpose**: Show how FPR/FNR compose when approximate values flow through typed computations.
**Key arguments**:
- Lifting a function $g : \{0,1\} \to \{0,1\}$ through $\AT{\{0,1\}}$: conditional PMF derivation (already worked out for BitSet)
- General principle: if $f : X \to Y$ and we substitute $\AT{X}$ for $X$, the output is $\AT{Y}$ with computable error rates
- For constant functions: error vanishes (no dependence on input)
- For identity: error passes through unchanged
- For composite functions: error accumulates
- Partial functions: false mapping rates, total false mapping rate
- Key result: error bounds for arbitrary compositions via the computational basis

**Existing content**: `bernoulli_type_2.tex` lines 4–40 (BitSet worked example — the best technical content in the paper), `prims.tex` lines 54–89 (partial function false mapping rates, set-indicator example)
**Gap**: The BitSet example is only for $\{0,1\}$. Needs generalization statement (even if proof is deferred). The partial function material in prims.tex is good but needs to be relocated here.

---

### 7. The approximate value monad
**File**: `sections/monad.tex` (new — content from cpp.tex)
**Purpose**: Give the computational realization. Show that `approx<T>` is a monad with a well-defined computational basis.
**Key arguments**:
- `approx<T>`: a wrapper carrying a value and its error rates
- Computational basis: `fpr()`, `fnr()`, `value()`, `pmf()`
- Specialization for Bool: `approx<bool>` with concrete `fpr`, `fnr`, `value` fields
- `fmap`/`amap`: lifting functions through approximate values
- Composition of approximate algorithms: if values or functions are replaced by approximate versions, the whole program becomes approximate
- **Branching**: to infer error rates through branches, must explore all paths (generally intractable → Monte Carlo)
- **Type erasure**: `approx<T>` → `T` discards error information

**Existing content**: `cpp.tex` lines 114–219 (monad template, specialization, fmap, branching discussion, type erasure)
**Gap**: Missing `bind` operation (only `fmap` is shown). The monad laws aren't stated. The connection between the mathematical framework (§3–§6) and this computational realization needs to be made explicit.

---

### 8. Data types with invariants
**File**: `sections/invariants.tex` (new — content from cpp.tex + bernoulli_type_2.tex)
**Purpose**: Address the practical question: what happens when approximate values violate type invariants?
**Key arguments**:
- A data type = product type + invariants (constraints)
- Invariants define a *relation* (subset) over the product type
- An approximate value may land outside this relation → invalid result
- Key question: $\Pr[\text{approx value violates invariant}]$
- Example: `Rational = tuple<int,int>` with invariant `denom ≠ 0`. An approximate rational might produce `(3, 0)`.
- When invariants can be maintained: approximate algebraic data types compose just like their exact counterparts
- When they can't: the approximate value is simply invalid, and we can quantify the probability

**Existing content**: `cpp.tex` lines 18–20, 63–111 (rationals example, invariant discussion), `bernoulli_type_2.tex` lines 50–52 (standalone equation, incomplete)
**Gap**: The probability of invariant violation is posed but never computed. Needs at least a simple worked example (e.g., probability that a random approximate rational has denominator = 0).

---

### 9. Discussion and future work
**File**: `sections/discussion.tex` (new)
**Purpose**: Situate results, connections, open questions.
**Key arguments**:
- Unification: Bernoulli sets = approximate indicator functions over the exponential type; Bernoulli relations = approximate subsets of product types; this paper shows these are all instances of approximate algebraic types
- Oblivious data types: approximate types with controlled error enable space-efficient, privacy-preserving data structures
- Approximate programs: replacing types with approximate versions yields approximate programs; error propagation is tractable for straight-line code, intractable for branching (Monte Carlo)
- Future work: recursive types (lists, trees), approximate type inference, integration with encrypted/homomorphic computation

**Existing content**: None dedicated. Fragments in `cpp.tex` lines 194–197 (branching), `prims.tex` lines 52 (approximate programs), `bernoulli_boolean_algebra.tex` lines 20–24 (oblivious types)
**Gap**: Entirely unwritten as a section.

---

## Proposed section ↔ file mapping

| § | Title | New file | Sources (existing content) |
|---|-------|----------|---------------------------|
| 1 | Introduction | `sections/intro.tex` (rewrite) | `intro.tex` |
| 2 | Preliminaries: Types as sets | `sections/prelim.tex` (new) | `cpp.tex:11–61`, `bernoulli_type_2.tex:43–52` |
| 3 | Random approximate values | `sections/approx_values.tex` (new) | `intro.tex:7–15`, `bernoulli_type_2.tex:4–40` |
| 4 | Primitive approximate types | `sections/prims.tex` (trim) | `prims.tex:9–26` |
| 5 | Composite approximate types | `sections/composite_types.tex` (new) | `prims.tex:28–49`, `bernoulli_boolean_algebra.tex`, `bernoulli_type_2.tex:38–52` |
| 6 | Error propagation | `sections/error_propagation.tex` (new) | `bernoulli_type_2.tex:4–40`, `prims.tex:54–89` |
| 7 | The approximate value monad | `sections/monad.tex` (new) | `cpp.tex:114–219` |
| 8 | Data types with invariants | `sections/invariants.tex` (new) | `cpp.tex:18–20,63–111`, `bernoulli_type_2.tex:50–52` |
| 9 | Discussion | `sections/discussion.tex` (new) | fragments scattered throughout |

## Content disposition

| Current file | Disposition |
|---|---|
| `sections/intro.tex` | Rewrite in place |
| `sections/bernoulli_type_2.tex` | Content splits into §3 (approx values), §5 (composite types), §6 (error propagation). File becomes orphan. |
| `sections/prims.tex` | Keep void+unit+Bool (§4). Move sum/product/exp to §5, partial functions to §6. |
| `sections/bernoulli_boolean_algebra.tex` | Content moves to §5. File becomes orphan. |
| `sections/cpp.tex` | C++ type catalog → §2. Monad + fmap → §7. Invariants/rationals → §8. File becomes orphan. |
