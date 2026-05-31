# Citation Verifier Report

**Paper**: The Bernoulli Model: A Probabilistic Framework for Data Structures and Types
**Reviewer lens**: citation accuracy, missing references, bibliography integrity
**Date**: 2026-05-29

## Summary

The bibliography is small (7 entries) and clean: all citation keys used in the body resolve, BibTeX runs without warnings, and the four external references that I can check (Shannon 1948, Cover and Thomas 2006, Bloom 1970, Warner 1965) have accurate metadata. The two issues are: one **defined-but-uncited** entry (`warner1965`), and several places where a claim is made that *should* carry a citation but does not (notably Count-Min sketch and Miller and Rabin). The three companion-paper citations are `@Unpublished` with `note={Companion paper}` and no further locator, which is acceptable for an internal preprint family but will need real coordinates before any external submission.

## Bibliography integrity

Verified by inspecting `main.aux` (citations issued), `main.bbl` (entries emitted), and `references.bib`:

- Citation keys used in body: `bernoulliComposition`, `bernoulliMaps`, `bernoulliSets`, `bloom1970`, `coverThomas`, `shannonBSC`. All six **resolve** (present in `.bbl`). No undefined-citation warnings in `main.log`.
- BibTeX log: clean (no warnings, no errors).

## Findings

### MINOR

**C1. `warner1965` is defined in `references.bib` but never cited.**
- Location: `references.bib` lines 50--58; no `\cite{warner1965}` anywhere in `sections/` (confirmed by grep, and by its absence from `main.aux`).
- Problem: dangling bibliography entry. With `bibstyle{plain}` and `\bibliography`, an uncited entry simply does not appear in the rendered references (so it is harmless to the PDF), but it is dead weight in the `.bib` and signals an intended-but-missing citation. Notably, the *foundation* paper `bernoulli_sets` cites Warner 1965 for exactly the per-element randomized-response interpretation that this paper's `prediction.tex` and `order_and_channels.tex` rely on.
- Suggestion: either cite Warner where the per-element binary-channel / randomized-response interpretation is introduced (`order_and_channels.tex` "Binary Channels", or `prediction.tex`), mirroring `bernoulli_sets`, or remove the entry. Citing it is preferable and strengthens the per-element framing.

**C2. Count-Min sketch is named three times with no citation.**
- Location: abstract (`main.tex` line 40); `intro.tex` line 8; `logic_gates.tex` line 116.
- Problem: "Count-Min sketch" is attributed no reference. The canonical citation is Cormode and Muthukrishnan, "An improved data stream summary: the count-min sketch and its applications," J. Algorithms 55(1):58--75, 2005. Naming a named structure without a citation is a standard referee flag.
- Suggestion: add the Cormode and Muthukrishnan 2005 entry and cite it, OR (per novelty N4) drop Count-Min from the paper since it is not analyzed.

**C3. Miller and Rabin primality test is described in detail with no citation.**
- Location: `bernoulli_maps.tex` lines 75--83.
- Quoted: "we can use randomized algorithms like the Miller-Rabin primality test ... based on Fermat's Little Theorem."
- Problem: the Miller and Rabin test is presented as a worked example over a full paragraph, including its theoretical basis, but neither Miller (1976) nor Rabin (1980) is cited. (Minor secondary point: the description leans on Fermat's Little Theorem, which is the *Fermat* test; Miller and Rabin strengthens it with the square-root-of-unity check. The prose conflates the two slightly, though this is a content nuance for the methodology reviewer rather than a citation defect.)
- Suggestion: add citations to Rabin (1980), "Probabilistic algorithm for testing primality," J. Number Theory 12(1):128--138, and/or Miller (1976). If keeping the Fermat-test phrasing, cite accordingly.

**C4. Companion-paper entries lack locators.**
- Location: `references.bib` lines 19--38 (`bernoulliSets`, `bernoulliMaps`, `bernoulliComposition`), all `@Unpublished` with `note={Companion paper}`.
- Problem: acceptable for an internal family preprint, but there is no year-disambiguation, URL, or repository pointer. All three are `year={2026}` by the same author with similar titles; a reader outside the family cannot locate them.
- Suggestion: before any external submission, add `howpublished`/`url`/`note` with arXiv or repository coordinates. For now, internally consistent.

## Accuracy spot-checks (external references)

- `shannonBSC` (Shannon 1948, BSTJ 27(3):379--423): metadata correct. Title, journal, volume, issue, pages, year all accurate. (The companion paper cites the same entry identically.)
- `coverThomas` (Cover and Thomas, *Elements of Information Theory*, 2nd ed., Wiley-Interscience, 2006): correct.
- `bloom1970` (Bloom, CACM 13(7):422--426, 1970): correct.
- `warner1965` (Warner, JASA 60(309):63--69, 1965): correct (though uncited; see C1).

## Confidence

High. The bibliography is small enough to check entry-by-entry; all keys resolve, the build is clean, and the external metadata is accurate. The findings are about *missing* citations for named results (Count-Min, Miller and Rabin) and one dangling entry, not about inaccurate ones.
