# Format Validator Report

**Paper**: Random approximate values over algebraic types
**Date**: 2026-05-29
**Scope**: Build verification, label resolution, venue formatting. Build performed by the reviewer.

## Verdict

The paper **builds cleanly to an 18-page PDF**, but **only with `TEXINPUTS` pointed at the local `sty/` directory**. The documented build command (state file and `bernoulli_data_type/CLAUDE.md`) omits that step, so following the docs verbatim produces a **fatal error and no PDF**. This is the most important format finding. Cross-references all resolve; warnings are minor. Confidence: **high** (reproduced both the failure and the success).

## Build verification

**As documented** (state file: `pdflatex -shell-escape main.tex && pdflatex -shell-escape main.tex`, run from the paper dir):
```
! LaTeX Error: File `functionnotation.sty' not found.
! Emergency stop.
==> Fatal error occurred, no output PDF file produced!
```
The `.sty` packages live in `algebraic_types/sty/` but the documented command does not put that directory on the LaTeX search path, so the very first `\usepackage{functionnotation}` fails.

**Corrected build** (what actually works):
```
cd algebraic_types
TEXINPUTS="./sty:$TEXINPUTS" pdflatex -shell-escape main.tex
TEXINPUTS="./sty:$TEXINPUTS" bibtex main
TEXINPUTS="./sty:$TEXINPUTS" pdflatex -shell-escape main.tex
TEXINPUTS="./sty:$TEXINPUTS" pdflatex -shell-escape main.tex
```
Result: **exit 0, main.pdf, 18 pages**, no undefined references, no undefined citations.

### MAJOR (build): documented build command does not produce a PDF
- **Problem**: Every consumer of the docs (including this review pipeline on its first attempt) will hit a fatal stop. The state file also claims the PDF is "~9 pages"; the current source builds to **18 pages**, so the recorded artifact size is stale too.
- **Suggestion (pick one)**:
  1. Add `TEXINPUTS="./sty:"` to the documented build command in `.papermill/state.md` and `bernoulli_data_type/CLAUDE.md`, OR
  2. Add a `latexmkrc` / `Makefile` that sets `TEXINPUTS`, OR
  3. Move the nine `.sty` files from `sty/` up next to `main.tex` (simplest; the other family papers that use modular packages keep them in `sty/` with a configured path, so option 1 or 2 is more consistent).
  Also update the state file's page count (9 to 18) and the build note.

## Label and cross-reference resolution

Checked all `\label` definitions against all `\cref`/`\Cref` uses in the included sections:
- **Every `\cref`/`\Cref` target resolves.** No "undefined reference" warnings in the final pass. Specifically verified: `thm:product_parsimony` (x4), `thm:approx_and` (x3), `rem:sum_nonfactor` (x2), `sec:composite`/`sec:error_prop` cross-links, `def:violation_prob`. All defined.
- The state file's note about "broken cross-references" is **stale**: there are none in the current build.
- One dangling label, `lst:valuector`, is defined inside a **commented-out** `\begin{listing}` block in cpp.tex; it neither registers nor is referenced, so it is inert. Harmless, but delete the dead label/caption comments when rewriting cpp.tex.

## Warnings (minor)

| Warning | Count | Location | Action |
|---|---|---|---|
| `Command \textless invalid in math mode` / `\textgreater invalid in math mode` | 4 | cpp.tex line ~166 (`\mathtt{approx\textless bool\textgreater}`) | Use `<`/`>` via `\langle`-style or `\texttt{approx<bool>}` outside math, or `\mathtt{approx{<}bool{>}}`. Renders acceptably now but is fragile. |
| Overfull/Underfull \hbox | 2 | (typesetting) | Cosmetic; address in final polish. |

No `minted` failures (shell-escape worked; `_minted-main/` cache populated). No `hyperref`, `cleveref`, `natbib`, or `siunitx` errors.

## Venue formatting
- **No target venue** (state file: `venue.target: null`). Document class is plain `article`, 11pt, 1-inch margins. There is no venue style to validate against, so formatting compliance is N/A at this stage.
- `\maketitle` present, abstract present, `\tableofcontents` present (reasonable for an internal draft; most venues would drop the TOC). `\listoffigures`/`\listofalgorithms` are commented out. Author block uses `atowell@siue.edu`; the state file's canonical contact is `lex@metafunctor.com` (cosmetic; align when choosing a venue).

## Orphaned-file hygiene (from the area-chair note)
The prompt referenced an "extensive `orphaned/` directory." **No `orphaned/` directory exists** anywhere under `bernoulli_data_type/`. What exists instead is three **orphaned section files inside `sections/`**, correctly excluded from the build via comments in main.tex:
- `sections/bernoulli_type_2.tex` (subsumed by approx_values.tex)
- `sections/bernoulli_type.tex` (near-duplicate of cpp.tex) -- note: this file is named in main.tex's comment but is **not present** in `sections/` (only `bernoulli_type_2.tex` is). The comment is slightly inaccurate.
- `sections/bernoulli_prolog.tex` -- also named in the comment but **not present** in `sections/`.
- **Suggestion**: The main.tex trailing comment lists three orphans; only one (`bernoulli_type_2.tex`) actually exists. Reconcile the comment with reality and delete `bernoulli_type_2.tex` once its content is confirmed fully absorbed. There is no large orphan directory to clean; the cleanup task is smaller than the brief implied.

## Summary
The paper compiles to a clean 18-page PDF, all cross-references resolve, and warnings are cosmetic. The one real defect is that the **documented build command is wrong** (missing `TEXINPUTS=./sty`), which makes the paper appear not to build at all. Fix the documented command and update the stale page count; everything else here is polish.
