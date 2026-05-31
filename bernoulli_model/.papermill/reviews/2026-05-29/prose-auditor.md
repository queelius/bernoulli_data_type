# Prose Auditor Report

**Paper**: The Bernoulli Model: A Probabilistic Framework for Data Structures and Types
**Reviewer lens**: writing quality, narrative arc, notation consistency
**Date**: 2026-05-29

## Summary

The prose is readable and the explanations are generally clear, but the manuscript reads as a working draft, not a finished paper. The single most pervasive problem is **notation inconsistency**: the paper defines macros `\Ber`, `\Berr`, `\Bool`, `\True`, `\False` in `main.tex`, then uses a *second, undefined-looking* notation `B_{...}` (literal `B` with subscript) throughout the body, and uses both within single paragraphs. There is a stray Markdown blockquote left in a LaTeX source file, a section that trails off mid-thought ("Here, we discuss how to generalize the results."), and a conclusion that restates the abstract without synthesizing the paper. None of these are fatal, but collectively they would draw immediate referee comment.

## Findings

### MAJOR

**P1. Two parallel notations for the Bernoulli type are used interchangeably, including within the same paragraph.**
- Location: pervasive. The macro form `\Ber{T}` (renders as a hat, `widehat{T}`) is defined in `main.tex` line 23 and used in `intro.tex`, parts of `prediction.tex`, and `lifting.tex`. The literal form `B_{T}` (a Roman `B` with a subscript) is used in `prediction.tex` (e.g. `B_{() -> Bool}^{(2)}`, line 43), throughout `set_indicator.tex` (`B_{X -> Bool}`), `logic_gates.tex` (`B_{Bool}^{(1)}`), and `bernoulli_maps.tex` (`B_{X -> Y}`).
- Examples of mixing within one region: `lifting.tex` line 30 uses `B_{Bool}^{(1)} -> B_{Bool}^{(1)}` and line 24 uses `\Berr{x}{1}`; `prediction.tex` line 11 uses `\Ber{x}` while line 43 uses `B_{() -> Bool}^{(2)}`. `lifting.tex` line 109--110 even writes `\Ber{\Bool}{1}` (macro `\Ber` takes ONE argument, so this renders as `widehat{Bool}1`, the `{1}` falling outside the hat), which is almost certainly meant to be `\Berr{\Bool}{1}`.
- Problem: a reader cannot tell whether `B_{Bool}` and `\Ber{Bool}` denote the same thing (they do) and whether the superscript order is being tracked consistently. This is the paper's most damaging *presentation* defect and interacts with the logic findings (order arithmetic) by making it hard to audit which order is meant.
- Suggestion: pick one notation (the macro `\Ber{}` / `\Berr{}{}` is the better choice since it is already defined and renders distinctively) and convert every `B_{...}` to it. Add `\Berr` usage where a superscript order is intended. Do a global search for literal `B_{` and `B_\Bool` and eliminate them. While doing so, fix the one-argument misuse `\Ber{\Bool}{1}` to `\Berr{\Bool}{1}` (occurs at least at `lifting.tex` lines 109--110).

**P2. A Markdown blockquote is left embedded in LaTeX source.**
- Location: `lifting.tex` line 102.
- Quoted: "`> It is very unlikely to be the case that the function is equal to the latent function, but it is very likely that the function is equal to the latent function on a large number of inputs.`"
- Problem: the leading `>` is Markdown, not LaTeX; it renders as a literal greater-than sign followed by the sentence, breaking the paragraph. This is a clear copy-paste artifact from notes.
- Suggestion: remove the `>` and integrate the sentence into the surrounding prose (it is a good observation and belongs in the higher-order discussion), or set it as `\emph{...}` / a `remark`.

**P3. A section trails off into a placeholder.**
- Location: `prediction.tex` lines 35--37 (the "Inducing Bernoulli Types" section opener).
- Quoted: "Here, we discuss how to generalize the results."
- Problem: this is a stub sentence standing in for an introduction to the section. The section then proceeds to "Unit Functions" without the promised generalization framing. It reads as an unfinished outline note. (This is the "trailing/unfinished fragment" flagged in the state file; it is located here, at the section head, rather than at end-of-file.)
- Suggestion: replace with a real one-to-two-sentence roadmap, or delete and let the `\subsection{Unit Functions}` carry the section.

### MINOR

**P4. The conclusion restates the abstract and does not synthesize the paper.**
- Location: `logic_gates.tex` lines 112--118 (the `\section{Conclusion}`).
- Quoted: "The Bernoulli Model is a way of thinking about the uncertainty in the output of a function ... This framework provides a formal foundation for developing space-efficient data structures with controllable error rates, such as Bloom filters and Count-Min sketches. It also enables the development of Oblivious Data Types ..."
- Problem: three short paragraphs that paraphrase the abstract. No synthesis of what was shown (lifting, the order/DOF distinction, the gate error tables), no statement of limitations, no pointer to the sibling `algebraic_types` paper that this is supposed to set up. For a foundational paper, the conclusion should hand off to the extension. (This is the "minimal/underdeveloped conclusion" flagged in the state file.) The conclusion also lives inside `logic_gates.tex`, which is structurally odd (a file named for logic gates ends the paper); consider moving it to its own `conclusion.tex` subfile for maintainability.
- Suggestion: rewrite to (1) summarize the three concrete results, (2) state the scope limit (Boolean-centric; algebraic types deferred), (3) forward-reference `algebraic_types`. Move it to a dedicated subfile.

**P5. Inconsistent error-rate symbols within `prediction.tex`.**
- Location: `prediction.tex`.
- Quoted: line 6 uses `\varepsilon(x)`; line 19 uses both `\varepsilon` and `\tau`; lines 53--59 (Table `tbl:second_order`) introduce `\tau, \eta, \varepsilon, \nu`; line 65 introduces `\epsilon` (note: `\epsilon`, not `\varepsilon`) as the first-order rate, then lines 75--76 mix `\epsilon` and `\varepsilon` in the same table ("`\tau = 1-\epsilon`", "`\varepsilon = \epsilon`").
- Problem: `\epsilon` and `\varepsilon` are visually distinct glyphs but are used as if interchangeable, and the relationship `\eta` (false negative) versus `\varepsilon` (false positive) versus the symmetric `\epsilon` is never tabulated. A reader must reverse-engineer the symbol set.
- Suggestion: add a short notation table (or a sentence) fixing: `\varepsilon` = FPR, `\eta` = FNR, `\tau` = TPR, `\nu` = TNR, and `\epsilon` = the common rate in the symmetric case. Then use them consistently. Align with `bernoulliSets`, which uses `\fprate`/`\fnrate`/`\tprate`/`\tnrate`; ideally adopt the same letters.

**P6. The abstract over-promises the body (presentation symptom of the novelty issue).**
- Location: abstract, `main.tex` lines 39--41.
- Problem: "Oblivious Data Types," "Count-Min sketches," and "$\mathcal{O}(1)$ time complexity ... probabilistic guarantees on accuracy" are all asserted but not delivered in the body (see novelty N3, N4). As prose, the abstract sets expectations the paper does not meet.
- Suggestion: trim the abstract to what the paper shows (type-indexed Bernoulli model, prediction, lifting, logic-gate error propagation, set indicators, Bernoulli maps as a basis).

**P7. Section ordering and naming.**
- Location: section sequence across subfiles.
- Problem: `order_and_channels.tex` contains *three* top-level `\section`s ("Order of Bernoulli Types", "Binary Channels", "False Positives and Negatives") inside one subfile, while `bernoulli_maps.tex` contains three top-level sections too. The 1-file-to-1-section convention used by the rest of the Bernoulli collection is not followed, which makes navigation and cross-referencing harder. Also "Lifting Unary Functions" (`lifting.tex`) precedes the binary `logic_gates.tex`, but `set_indicator.tex` (which uses the function-space confusion matrix introduced in `lifting.tex`) sits between them, so the confusion-matrix concept is introduced, dropped for set indicators, then resumed. The arc would read better as: order/channels -> prediction -> lifting (unary) -> logic gates (binary) -> set indicators -> Bernoulli maps.
- Suggestion: consider one section per subfile and reorder so the function-approximation thread is contiguous.

**P8. Minor wording / typographic issues.**
- `set_indicator.tex` line 17, 21: "less than `k`" should be "at most `k`" to match the predicate `<= k` (also a logic issue, L5).
- `prediction.tex` line 6: "assuming we know the error rate `\varepsilon(x)` is better than a random guess" is garbled; presumably "assuming the error rate `\varepsilon(x) < 0.5`, i.e. better than a random guess."
- `lifting.tex` line 38, 56: "degrees of freedom" is used where the rest of the family says "free parameters"; `order_and_channels.tex` Remark uses "free parameters" / "degrees of freedom" together. Standardize on one term (the family prefers "parameters" / "parameter count").
- `intro.tex` line 24: "`() ⊕ Bool` has three possible values" uses `⊕` for sum type; `logic_gates.tex` and elsewhere use prose. Fine, but define `⊕` (sum) and `×` (product) once.

## Narrative arc assessment

The intro motivates well (oblivious data types, Bloom filters, the void/unit/Bool ladder). The middle (order, channels, prediction, lifting) has a coherent thread. The arc weakens after `lifting.tex`: `set_indicator.tex` detours into an algorithm with errors, `bernoulli_maps.tex` re-treads function spaces, and the conclusion fizzles. With the notation unified, the blockquote and stub removed, the `hash_set` section repaired (see logic/methodology reports), and a real conclusion that hands off to `algebraic_types`, the paper would read as a finished foundational piece.

## Confidence

High on P1--P4 (directly observed in source). High on P5--P8 (directly observed). These are presentation defects, independent of the correctness questions raised by the logic and methodology reviewers.
