# Bernoulli data type manuscripts

Alexander Towell.

> **Frozen, 2026-10-21.** This is the final version of a preprint. It is superseded by the monograph *Bernoulli Types: Two-Sided Error in Approximate Membership, Retrieval, and Computation* ([read online](https://metafunctor.com/latex/bernoulli-monograph/book.html); [research hub](https://metafunctor.com/research/bernoulli/)), developed in the [bernoulli monorepo](https://github.com/queelius/bernoulli). This repository is archived read-only; corrections and new material go into the monograph.


## The Bernoulli Model: A Probabilistic Framework for Data Structures and Types (`bernoulli_model/`)

- **Final version DOI:** [10.5281/zenodo.23531335](https://doi.org/10.5281/zenodo.23531335) (all versions: [10.5281/zenodo.19105398](https://doi.org/10.5281/zenodo.19105398))
- **Superseded by:** ch. 1 (The Noisy Bit), ch. 2 (Binary Channels and Model Order), ch. 3 (Boolean Algebra Under Noise), ch. 4 (Two Axioms and Kronecker Factorization), ch. 13 (The Algebra of Approximate Types)
- **Errata:** [`bernoulli_model/ERRATA.md`](bernoulli_model/ERRATA.md)
- **Build** (in `bernoulli_model`): `pdflatex main.tex && bibtex main && pdflatex main.tex && pdflatex main.tex`

## Random approximate values over algebraic types (`algebraic_types/`)

- **Final version DOI:** [10.5281/zenodo.23531336](https://doi.org/10.5281/zenodo.23531336) (all versions: [10.5281/zenodo.19105395](https://doi.org/10.5281/zenodo.19105395))
- **Superseded by:** ch. 13 (The Algebra of Approximate Types)
- **Errata:** [`algebraic_types/ERRATA.md`](algebraic_types/ERRATA.md)
- **Build** (in `algebraic_types`): `TEXINPUTS=".:./sty:" pdflatex -shell-escape main.tex && bibtex main && TEXINPUTS=".:./sty:" pdflatex -shell-escape main.tex && TEXINPUTS=".:./sty:" pdflatex -shell-escape main.tex`

## License

CC BY 4.0; see `LICENSE`.
