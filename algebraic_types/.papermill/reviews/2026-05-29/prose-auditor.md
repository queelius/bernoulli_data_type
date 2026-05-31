# Prose Auditor Report

**Paper**: Random approximate values over algebraic types
**Date**: 2026-05-29
**Scope**: Writing quality, narrative arc, notation consistency, section-level polish. Calibrated to an early draft.

## Verdict

The front of the paper (intro through invariants, sections 1 to 7) is **well written**: clear, professionally structured, with a coherent narrative arc, good signposting, and consistent notation. The single serious prose problem is the **C++ section (cpp.tex)**, which is markedly rougher than everything before it: typos, ungrammatical sentences, a tone shift to informal notes, and near-duplication of an orphaned file. There is also a small `BitSet`/`Bool` discipline issue and a few mechanical items. Confidence: **high**.

## Strengths (to preserve)
- The intro (intro.tex) is strong: motivation (records, nullables, query results), a crisply stated central claim, four enumerated contributions each tied to a result, and an explicit organization paragraph. This is publication-grade framing.
- Narrative arc matches the OUTLINE.md plan and proceeds bottom-up (void to unit to Bool to product to sum to exponential to error propagation to Boolean algebra to invariants), which is pedagogically sound.
- `rem:bitset_bool` proactively addresses a notation ambiguity (`BitSet` vs `Bool`). Good authorial awareness.
- The summary table (composite_types.tex) is an effective at-a-glance synthesis.

## Findings

### MAJOR (prose): cpp.tex is substantially below the quality of the rest of the paper
- **Location**: cpp.tex, throughout.
- **Representative quoted defects**:
  - Grammatical: "a product type may the Cartesian product of integer and Boolean types" (missing verb "be").
  - Typo in code comment narrative and prose: "we git a random approximate value type as output" appears in the orphaned twin; in cpp.tex the parallel prose is "applying `f` to an approximate value of type `X` yields an approximate value of type `Y`" (OK) but the section opens with the same blocky paragraphs as the orphan.
  - Mis-statement: "the confusing notation of `void` denotes the unit type (and the single value)" then later "Exponential types are *functions*. In C++, `[](tuple<char,bool>) -> bool` is the set of functions ... which has a cardinality of `2^{512}`." The cardinality `2^512` is correct for `(char x bool) -> bool` since `|char x bool| = 512`, but the prose conflates a single lambda value with "the set of functions"; a lambda is one element, not the set.
  - Informal tone: "This may not seem particularly useful, but ..."; "significantly more complicated if *fully* implemented." Acceptable in notes, not in a results section.
  - `\textless`/`\textgreater` used inside math mode at cpp.tex line ~166 (`\mathtt{approx\textless bool\textgreater}`), which produces 4 LaTeX warnings (see format-validator). Reads fine in PDF but is fragile.
- **Problem**: The reader exits a polished theoretical development into what reads like raw notes. It undercuts the paper's credibility precisely at the "realization" claim.
- **Suggestion**: Rewrite cpp.tex from scratch as a tight 1 to 1.5 page section: (1) one paragraph mapping each constructor to its C++ form (struct/tuple, variant, optional, lambda), (2) the corrected `approx<bool>` + `bind` (see logic-checker MAJOR-1 / methodology MAJOR), (3) one sentence on type-erasure. Delete the redundant ADT-tutorial paragraphs that restate prims/composite. Much of the section duplicates earlier material and can simply be cut.

### MINOR (notation): `BitSet`/`Bool` discipline is declared but not followed
- **Location**: `rem:bitset_bool` (approx_values.tex) declares `BitSet` for numerical/channel contexts and `Bool` for logical/type contexts. But `thm:approx_and` and `thm:approx_or` (bernoulli_boolean_algebra.tex) state inputs as `\AT{\Bool}` while writing the conditional events as `\Given a = 0, b = 0` (numeric 0/1) and the AND-gate table in error_propagation.tex uses `\BitSet`. The two symbols are used interchangeably within single results, which is exactly what the remark says it will avoid.
- **Suggestion**: Pick `BitSet` for all the channel/rate theorems (they are numerical) and reserve `Bool` for the type-algebra prose. Mechanical pass.

### MINOR (cross-section duplication): three near-identical "approximate bit lifting" derivations
- **Location**: approx_values.tex §lifting_preview (Eqs lift_cond_0/1), the orphaned bernoulli_type_2.tex, and error_propagation.tex `thm:error_prop`. The same `g: Bit->Bit` lifting is derived multiple times.
- **Problem**: Mild redundancy; the reader sees the same four-cases-of-Bit->Bit argument twice in included files.
- **Suggestion**: Keep the general `thm:error_prop` as the authoritative statement and make §lifting_preview explicitly a "preview/special case of `thm:error_prop`" (it already half-does this). Ensure the orphaned twin stays out of the build (it does).

### MINOR (terminology): "false positive rate" reused for general payload error
- **Location**: composite_types.tex `def` of approximate sum type.
- **Quoted text**: "Here `\fprate_{\Set{A}}` denotes the general per-value error rate within component A ... not specifically the false positive rate. We reuse the `\fprate` symbol for notational economy ..."
- **Problem**: The paper notices the overload and excuses it, but overloading the FPR symbol to mean "generic payload error" invites confusion two pages after FPR was given a precise meaning. The disclaimer helps but does not fully remove the hazard.
- **Suggestion**: Use a distinct symbol for generic per-component payload error (e.g., `\rho_A`) and reserve `\fprate`/`\fnrate` for the binary case. Cleaner than a disclaimer.

### SUGGESTION (polish)
- Abstract uses `A + \mathrm{Unit}` while the body uses `\Set{A} + \Unit`; harmonize.
- `prims.tex` is very short (37 lines) and its "Composite type constructors" subsection only forwards to §composite. Consider merging the void/unit material into §composite's preamble so the paper has one "types" section rather than a 1-paragraph orphan section.
- Several sections end abruptly without a transition sentence to the next; one connective clause each would smooth the flow.

## Cross-verification note
Per the area-chair workflow, the novelty-assessor was asked whether the cpp.tex roughness is "unclear writing hiding a weak contribution." Assessment: **no**, this is genuinely a presentation problem, not a hidden-weakness problem. The underlying contribution (the realization claim) is real but is undercut by both a code bug (logic-checker MAJOR-1) and rough prose (this report). The two are independent defects on the same section. Both stand.

## Summary
Sections 1 to 7 are in good shape and need only light polish (notation discipline, minor de-duplication). Section 8 (cpp.tex) needs a ground-up rewrite and is the prose priority. The notation-discipline and symbol-overload items are quick wins.
