# Format Validator Report

**Paper**: The Bernoulli Model: A Probabilistic Framework for Data Structures and Types
**Reviewer lens**: build verification, label resolution, venue formatting
**Date**: 2026-05-29
**Build command run**: `pdflatex main.tex` -> `bibtex main` -> `pdflatex main.tex` -> `pdflatex main.tex` (in `bernoulli_model/`)

## Summary

The document **builds cleanly**: all three `pdflatex` passes and the `bibtex` pass exit 0, the PDF is produced (21 pages), and there are **no undefined references, no undefined citations, and no multiply-defined labels**. The only build-log noise is 5 overfull `\hbox` warnings, four of which are negligible (< 5 pt) and one of which is moderate (~51 pt) and worth a manual line break. There is no target venue, so there is no venue-template compliance to check; the document uses the stock `article` class at 11pt on A4.

## Build result

| Pass | Exit code | Notes |
|---|---|---|
| `pdflatex` (1) | 0 | |
| `bibtex` | 0 | clean log, no warnings |
| `pdflatex` (2) | 0 | citations resolved |
| `pdflatex` (3) | 0 | cross-refs stable |

- **Output**: `main.pdf`, 21 pages, ~298 KB. (The committed `bernoulli_model.pdf` is the prior build; the state file's "~19 pages" is slightly stale, current build is 21 pages.)
- **Undefined references / citations**: none (grep of `main.log` for "undefined", "Citation", "Reference", "multiply" is empty).
- **LaTeX warnings**: none of the `LaTeX Warning:` class (no "There were undefined references", no "Label(s) may have changed").

## Findings

### MINOR

**F1. One moderate overfull `\hbox` (~51 pt).**
- Location: `main.log` reports "Overfull \hbox (51.08699pt too wide) in paragraph at lines 36--37", which corresponds to `bernoulli_maps.tex` around the type-signature prose (the `p^* sim B_{X->Y}(p)` / `X -> B_Y` discussion).
- Problem: 51 pt is roughly half an inch of overrun; it will visibly stick into the right margin.
- Suggestion: rewrap the line, or wrap the inline math in `\(...\)` with a discretionary break, or display the type signature. The other four overfull boxes (0.6 pt, 2.0 pt, 3.9 pt, 5.0 pt) are cosmetically negligible and can be left.

**F2. Cross-references all use raw `\ref`/`\cite`; no `cleveref`.**
- Location: throughout (e.g. `lifting.tex` "see Remark~\ref{rem:order_vs_dof}", `set_indicator.tex` "Table~\ref{tbl:set_indicator}").
- Problem: not a defect, but the foundation paper `bernoulli_sets` uses `cleveref` (`\cref`). If these sub-papers are ever merged or read together, the reference style will be inconsistent (manual "Table~\ref{}" vs `\cref{}`). All labels resolve, so this is purely stylistic.
- Suggestion: optional; for consistency with the family, consider `cleveref`. Low priority.

**F3. Single cross-subfile label dependency is satisfied, but is fragile under standalone subfile builds.**
- Location: `rem:order_vs_dof` is defined in `order_and_channels.tex` and referenced from `lifting.tex` (line 56), `bernoulli_maps.tex` (line 37). Under the full `main.tex` build this resolves correctly (verified: no undefined-reference warning).
- Problem: if any subfile is compiled standalone via the `subfiles` class, the cross-file `\ref` to `rem:order_vs_dof` will be undefined in that isolated build. This does not affect the main build.
- Suggestion: no action needed for the main build; just be aware when compiling a single section in isolation.

### Observations (not defects)

- **No venue template**: the paper targets no venue (per the task brief and state file). The stock `article`/11pt/A4 setup is fine for a working preprint. If a venue is later chosen, the title block (`\author{Alexander Towell \\ \texttt{atowell@siue.edu}}`) and class will need to be swapped; note the source email is `atowell@siue.edu` while the state file canonical email is `lex@metafunctor.com` (metadata, not a build issue).
- **No figures**: the paper is all tables (`booktabs`) and inline/display math; no `img/` directory, no external graphics, so no figure-path or graphics-inclusion risks.
- **Packages**: `algorithm`, `algpseudocode`, `listings`, `xcolor` are loaded (`main.tex` lines 9--12) but the body contains **no** `algorithm`, `lstlisting`, or color usage. These are unused includes. Harmless, but could be trimmed. (Relevant to the methodology note that the `hash_set` construction *should* be an `algorithm` environment but is currently prose; the package is already loaded for it.)

## Confidence

High. The build was executed in this review and the log inspected directly: clean compile, no reference/citation/label errors, 21 pages, one moderate overfull box.
