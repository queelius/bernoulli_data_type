# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This LaTeX paper repository contains the manuscript "The algebra of the random approximate map model" which explores theoretical foundations of approximate maps, their compositions, and applications. The paper is structured as a multi-section document with custom notation styles.

## Build Commands

### Compiling the Paper
```bash
# Standard compilation (produces bernoulli-maps.pdf)
pdflatex bernoulli-maps.tex

# Full compilation with bibliography and cross-references
pdflatex bernoulli-maps.tex
bibtex bernoulli-maps
pdflatex bernoulli-maps.tex
pdflatex bernoulli-maps.tex

# Clean auxiliary files
rm -f *.aux *.log *.out *.toc *.maf *.mtc *.mtc0
```

## Document Structure

### Main Document
- **bernoulli-maps.tex** - Main document that includes all sections and defines document class/packages

### Content Sections (in sections/)
- **intro.tex** - Introduction and motivation
- **approxmaps.tex** / **approx_maps.tex** - Core definitions of approximate maps
- **composition.tex** - Higher-order maps and compositions
- **special_functions.tex** - Predicate functions and indicators
- **disc_fun.tex** - Discrete partial functions
- **singular_hash_map.tex** - First-order rate-distorted map implementation
- **adt.tex**, **random_algebraic.tex**, **rank_ordered_search.tex**, **appendix.tex** - Additional sections (currently commented out)

### Custom Style Files
Mathematical notation and environments are defined in custom .sty files:
- **approxsetnotation.sty** - Approximate set notation
- **functionnotation.sty** - Function notation
- **setnotation.sty** - Set theory notation
- **relationnotation.sty** - Relation notation
- **algorithmnotation.sty** - Algorithm notation
- **probabilitynotation.sty** - Probability notation

### Key Mathematical Concepts

The paper uses specialized notation for:
- Approximate maps: `\AFun{f}` with false positive/negative rates `\fprate`, `\fnrate`
- Random variables and distributions (Bernoulli, geometric)
- Set operations with approximate semantics
- Rate-distortion theory applied to discrete functions
- Singular hash maps as practical implementations

### Dependencies

- LaTeX packages: lmodern, minitoc, microtype, algorithm2e, amsmath, amsthm, tikz, minted, and others
- BibTeX for references (references.bib)
- Custom notation packages (all .sty files in directory)

### Important Notes

- The document uses both inline section content (intro.tex, approxmaps.tex, etc.) and the main file
- Several sections are currently commented out in the main file using `%\subfile{}`
- The paper explores theoretical CS concepts related to probabilistic data structures