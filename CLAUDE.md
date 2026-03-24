# CLAUDE.md

## Project Overview

Research paper(s): **"Random approximate values over algebraic types"** by Alexander Towell. Extends the Bernoulli set model to algebraic data types — Boolean, unit, absurd, sum (tagged union), product, and exponential (function) types. Contains two sub-papers at different stages of development:

- `algebraic_types/` — The main paper on approximate algebraic types, including approximate Boolean algebra and computational basis
- `bernoulli_model/` — A companion paper on the Bernoulli model as a probabilistic framework for types, covering order/channels, logic gates, and lifting

## Build

```bash
# algebraic_types sub-paper
cd algebraic_types && pdflatex -shell-escape main.tex && pdflatex -shell-escape main.tex

# bernoulli_model sub-paper
cd bernoulli_model && pdflatex main.tex && pdflatex main.tex
```

Note: `algebraic_types/` uses `minted` package, requiring `-shell-escape`.

## Structure

### `algebraic_types/`
- `main.tex` — Document root
- `sections/` — `intro.tex`, `prims.tex`, `bernoulli_boolean_algebra.tex`, `bernoulli_type_2.tex`, `cpp.tex`
- `sty/` — Custom notation packages including `approx_data_type_notation.sty`

### `bernoulli_model/`
- `main.tex` — Document root (defines `\Bool`, `\True`, `\False`, `\Ber{}`, `\Berr{}{}`)
- `sections/` — `intro.tex`, `order_and_channels.tex`, `prediction.tex`, `lifting.tex`, `set_indicator.tex`, `logic_gates.tex`, `bernoulli_maps.tex`

## Key Notation

### `algebraic_types/` (modular packages)
Uses shared `sty/` packages plus `approx_data_type_notation.sty`:
- Approximate types: `\AT{T}`, `\ATL{T}` — approximate type notation
- Boolean: `\Bool`, `\AndFn`, `\OrFn`, `\NegateFn`
- Bitwise: `\ATL{\BitSet^n}` — approximate bit vectors

### `bernoulli_model/` (self-contained macros)
- `\Ber{T}` — Bernoulli approximation of type $T$
- `\Berr{T}{k}` — Bernoulli approximation of order $k$
- `\Bool`, `\True`, `\False` — Boolean type and values

## Key Conceptual Distinction

**Model order vs. parameter count**: For product types $A \times B$, each component has its own Bernoulli error channel. A general $n$-th order channel on $k$ output symbols has $n(k-1)$ parameters, but element-wise independence forces a Kronecker product factorization: the joint confusion matrix of two independent Boolean tests has 4 parameters, not 12. This is why approximate AND, OR, and other logic gates over Bernoulli Booleans have tractable error formulas. The distinction between model order (partition granularity) and parameter count (constrained by independence) is central to the type-theoretic generalization: sum types inherit independent per-component errors, product types factor as Kronecker products, and exponential types compose via channel matrix multiplication. See `bernoulli_sets/` Remark on parametric parsimony.

## Cross-Paper Connections

This paper is the **most general treatment** in the Bernoulli framework:

| Topic | Paper |
|-------|-------|
| Set-level error model (FPR/TPR, precision) | `bernoulli_sets/` |
| Function spaces $X \to Y$, approximate maps | `bernoulli_maps/` |
| Relational algebra (join, project) | `bernoulli_relations/` |
| **Algebraic types (Bool, sum, product, unit, absurd)** | **This paper** |

**Type-theoretic hierarchy** (from this paper's perspective):
- **Absurd type** $\bot$: empty set, no values to instantiate — zero-order model (trivial)
- **Unit type** $\{*\}$: singleton set — zero-order model (no errors possible)
- **Boolean type** $\{\top, \bot\}$: the simplest non-trivial case — symmetric channel (first-order, 1 rate) or asymmetric channel (second-order, 2 rates = FPR/FNR)
- **Finite types**: $n$-element sets with up to $n$ distinct error rates
- **Sum types** $A + B$: tagged unions — errors can occur in the tag or the value
- **Product types** $A \times B$: component-wise errors
- **Exponential types** $A \to B$: approximate maps (connects to `bernoulli_maps/`)

**Key insight**: `bernoulli_sets/` shows that a membership query on a Bernoulli set returns a *Bernoulli Boolean*. This paper generalizes that: any operation on a Bernoulli type yields a Bernoulli type. The `bernoulli_model/` sub-paper already covers order/channels and logic gates (approximate AND, OR) in the Boolean case. The `algebraic_types/` sub-paper extends to the full algebraic type system.

**Existing content**: `order_and_channels.tex` already uses the correct first-order (symmetric) / second-order (asymmetric) terminology. `logic_gates.tex` already covers approximate AND with truth tables and error propagation. `bernoulli_boolean_algebra.tex` discusses approximate Boolean algebras over bit vectors.
