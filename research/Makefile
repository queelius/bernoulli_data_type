# Makefile for Bernoulli Data Types Research Materials

LATEX = pdflatex
BIBTEX = bibtex
LATEX_FLAGS = -interaction=nonstopmode -halt-on-error
MINTED_FLAGS = -shell-escape

# Papers
PAPER_DIR = papers
PAPERS = 01_foundations_bernoulli_types \
         02_bernoulli_sets_algebra \
         03_bernoulli_maps_universal \
         04_regular_types_bernoulli \
         05_bernoulli_search_retrieval \
         06_entropy_maps_implementation \
         07_statistical_analysis_bernoulli

# Textbook
TEXTBOOK_DIR = textbook
TEXTBOOK = main

# Output directory
BUILD_DIR = build

.PHONY: all papers textbook clean view

all: papers textbook

papers: $(PAPERS:%=$(BUILD_DIR)/%.pdf)

textbook: $(BUILD_DIR)/$(TEXTBOOK).pdf

# Rule for building papers
$(BUILD_DIR)/%.pdf: $(PAPER_DIR)/%.tex $(PAPER_DIR)/references.bib | $(BUILD_DIR)
	@echo "Building paper: $*"
	@cp $(PAPER_DIR)/references.bib $(BUILD_DIR)/
	cd $(PAPER_DIR) && $(LATEX) $(LATEX_FLAGS) $(MINTED_FLAGS) -output-directory=../$(BUILD_DIR) $*.tex
	cd $(BUILD_DIR) && $(BIBTEX) $*
	cd $(PAPER_DIR) && $(LATEX) $(LATEX_FLAGS) $(MINTED_FLAGS) -output-directory=../$(BUILD_DIR) $*.tex
	cd $(PAPER_DIR) && $(LATEX) $(LATEX_FLAGS) $(MINTED_FLAGS) -output-directory=../$(BUILD_DIR) $*.tex

# Rule for building textbook
$(BUILD_DIR)/$(TEXTBOOK).pdf: $(TEXTBOOK_DIR)/$(TEXTBOOK).tex $(TEXTBOOK_DIR)/notation.tex | $(BUILD_DIR)
	@echo "Building textbook"
	cd $(TEXTBOOK_DIR) && $(LATEX) $(LATEX_FLAGS) $(MINTED_FLAGS) -output-directory=../$(BUILD_DIR) $(TEXTBOOK).tex
	cd $(BUILD_DIR) && $(BIBTEX) $(TEXTBOOK)
	cd $(BUILD_DIR) && makeindex $(TEXTBOOK).idx
	cd $(TEXTBOOK_DIR) && $(LATEX) $(LATEX_FLAGS) $(MINTED_FLAGS) -output-directory=../$(BUILD_DIR) $(TEXTBOOK).tex
	cd $(TEXTBOOK_DIR) && $(LATEX) $(LATEX_FLAGS) $(MINTED_FLAGS) -output-directory=../$(BUILD_DIR) $(TEXTBOOK).tex

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# View PDFs
view: view-papers

view-papers:
	@for paper in $(PAPERS); do \
		if [ -f $(BUILD_DIR)/$$paper.pdf ]; then \
			echo "Opening $$paper.pdf"; \
			xdg-open $(BUILD_DIR)/$$paper.pdf 2>/dev/null || open $(BUILD_DIR)/$$paper.pdf 2>/dev/null || echo "Please open $(BUILD_DIR)/$$paper.pdf manually"; \
		fi; \
	done

view-textbook:
	@if [ -f $(BUILD_DIR)/$(TEXTBOOK).pdf ]; then \
		xdg-open $(BUILD_DIR)/$(TEXTBOOK).pdf 2>/dev/null || open $(BUILD_DIR)/$(TEXTBOOK).pdf 2>/dev/null || echo "Please open $(BUILD_DIR)/$(TEXTBOOK).pdf manually"; \
	fi

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(PAPER_DIR)/*.aux $(PAPER_DIR)/*.log $(PAPER_DIR)/*.out $(PAPER_DIR)/*.bbl $(PAPER_DIR)/*.blg
	rm -f $(TEXTBOOK_DIR)/*.aux $(TEXTBOOK_DIR)/*.log $(TEXTBOOK_DIR)/*.out $(TEXTBOOK_DIR)/*.bbl $(TEXTBOOK_DIR)/*.blg
	rm -f $(TEXTBOOK_DIR)/*.toc $(TEXTBOOK_DIR)/*.lof $(TEXTBOOK_DIR)/*.lot $(TEXTBOOK_DIR)/*.idx $(TEXTBOOK_DIR)/*.ind

# Individual paper targets
foundations: $(BUILD_DIR)/01_foundations_bernoulli_types.pdf
sets: $(BUILD_DIR)/02_bernoulli_sets_algebra.pdf
maps: $(BUILD_DIR)/03_bernoulli_maps_universal.pdf
regular: $(BUILD_DIR)/04_regular_types_bernoulli.pdf
search: $(BUILD_DIR)/05_bernoulli_search_retrieval.pdf
entropy: $(BUILD_DIR)/06_entropy_maps_implementation.pdf
stats: $(BUILD_DIR)/07_statistical_analysis_bernoulli.pdf

# Help
help:
	@echo "Bernoulli Data Types Research Build System"
	@echo ""
	@echo "Usage:"
	@echo "  make all       - Build all papers and textbook"
	@echo "  make papers    - Build all papers"
	@echo "  make textbook  - Build the textbook"
	@echo "  make foundations/sets/maps/regular/search/entropy - Build individual papers"
	@echo "  make view      - Open all built PDFs"
	@echo "  make clean     - Remove all build artifacts"
	@echo ""
	@echo "Requirements:"
	@echo "  - pdflatex with minted package support"
	@echo "  - bibtex"
	@echo "  - makeindex (for textbook)"
	@echo "  - Python with Pygments (for minted syntax highlighting)"