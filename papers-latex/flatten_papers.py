#!/usr/bin/env python3
"""
Script to flatten LaTeX papers for arXiv submission by:
1. Inlining all custom .sty files
2. Replacing subfile commands with actual content
3. Replacing minted with listings
"""

import os
import re
from pathlib import Path

def read_file(filepath):
    """Read a file and return its contents."""
    with open(filepath, 'r', encoding='utf-8') as f:
        return f.read()

def extract_sty_content(sty_file):
    """Extract the actual content from a .sty file (excluding header/footer)."""
    content = read_file(sty_file)
    lines = content.split('\n')

    # Skip header lines
    result = []
    in_body = False
    for line in lines:
        if not in_body:
            if line.startswith('\\ProvidesPackage') or line.startswith('\\NeedsTeXFormat'):
                continue
            elif line.startswith('\\RequirePackage') or line.startswith('\\usepackage'):
                continue
            elif line.startswith('\\ExecuteOptions') or line.startswith('\\ProcessOptions'):
                continue
            else:
                in_body = True

        if in_body and line.strip() != '\\endinput':
            result.append(line)

    return '\n'.join(result)

def inline_subfiles(content, base_dir):
    """Replace subfile commands with actual content."""
    pattern = r'\\subfile\{([^}]+)\}'

    def replace_subfile(match):
        subfile_path = match.group(1)
        # Add .tex extension if not present
        if not subfile_path.endswith('.tex'):
            subfile_path += '.tex'

        full_path = os.path.join(base_dir, subfile_path)
        if os.path.exists(full_path):
            subcontent = read_file(full_path)
            # Remove document wrapper from subfiles
            subcontent = re.sub(r'\\documentclass\[.*?\]\{subfiles\}', '', subcontent)
            subcontent = re.sub(r'\\providecommand\{\\mainx\}\{.*?\}', '', subcontent)
            subcontent = re.sub(r'\\begin\{document\}', '', subcontent)
            subcontent = re.sub(r'\\end\{document\}', '', subcontent)
            return subcontent.strip()
        else:
            print(f"Warning: Could not find subfile: {full_path}")
            return match.group(0)

    return re.sub(pattern, replace_subfile, content)

def replace_minted_with_listings(content):
    """Replace minted package and commands with listings."""
    # Replace package
    content = re.sub(r'\\usepackage(\[.*?\])?\{minted\}',
                     r'\\usepackage{listings}\n\\lstset{language=C++,basicstyle=\\ttfamily\\small,breaklines=true}',
                     content)

    # Replace mintinline
    content = re.sub(r'\\mintinline\{[^}]+\}\{([^}]+)\}', r'\\lstinline{\1}', content)

    # Replace minted environments
    content = re.sub(r'\\begin\{minted\}(\[.*?\])?\{[^}]+\}', r'\\begin{lstlisting}', content)
    content = re.sub(r'\\end\{minted\}', r'\\end{lstlisting}', content)

    # Clean up lstlisting blocks with minted options
    def clean_lstlisting(match):
        block = match.group(0)
        # Remove minted-specific options
        lines = block.split('\n')
        result = []
        skip_next = False
        for i, line in enumerate(lines):
            if skip_next:
                skip_next = False
                continue
            if i == 0:
                result.append(line)
            elif i == 1 and line.strip() == '[':
                # Skip until we find the closing ]
                for j in range(i+1, len(lines)):
                    if lines[j].strip().endswith(']{c++}') or lines[j].strip() == ']{c++}':
                        skip_next = True
                        break
            elif not (line.strip().startswith('mathescape') or
                     line.strip().startswith('frame') or
                     line.strip().startswith('framesep') or
                     line.strip().startswith('baselinestretch') or
                     line.strip().startswith('bgcolor') or
                     line.strip().startswith('fontsize') or
                     line.strip().startswith('linenos') or
                     line.strip() == '[' or
                     line.strip().endswith(']{c++}')):
                # Clean up indentation
                if line.startswith('\t'):
                    result.append(line[1:])
                else:
                    result.append(line)
        return '\n'.join(result)

    content = re.sub(r'\\begin\{lstlisting\}.*?\\end\{lstlisting\}',
                     clean_lstlisting, content, flags=re.DOTALL)

    return content

def flatten_paper(paper_dir, output_name, main_name='main.tex'):
    """Flatten a complete paper directory."""
    main_file = os.path.join(paper_dir, main_name)
    if not os.path.exists(main_file):
        print(f"Error: No {main_name} found in {paper_dir}")
        return False

    content = read_file(main_file)

    # Collect all custom .sty files
    sty_files = [
        'functionnotation.sty', 'setnotation.sty', 'approxsetnotation.sty',
        'approx_data_type_notation.sty', 'probabilitynotation.sty',
        'relationnotation.sty', 'approxrelationnotation.sty', 'envnotation.sty',
        'algorithmnotation.sty', 'matrixnotation.sty', 'obliviousnotation.sty',
        'alexmisc.sty', 'statistics.sty'
    ]

    sty_content = []
    sty_content.append("\n% ===== BEGIN CUSTOM MACRO DEFINITIONS FROM .sty FILES =====\n")

    # Track defined commands to avoid duplicates
    defined_commands = set()

    for sty in sty_files:
        sty_path = os.path.join(paper_dir, sty)
        if os.path.exists(sty_path):
            sty_raw = extract_sty_content(sty_path)
            # Only add if it has unique content not already defined
            if sty_raw.strip():
                # Check for duplicate \newcommand definitions
                new_content = []
                for line in sty_raw.split('\n'):
                    # Simple check for newcommand/renewcommand
                    if '\\newcommand' in line or '\\renewcommand' in line:
                        # Extract command name
                        import re
                        match = re.search(r'\\(?:new|renew)command\{?\\(\w+)', line)
                        if match:
                            cmd_name = match.group(1)
                            if cmd_name in defined_commands:
                                # Replace newcommand with renewcommand for duplicates
                                line = line.replace('\\newcommand', '\\renewcommand')
                            defined_commands.add(cmd_name)
                    new_content.append(line)

                sty_content.append(f"\n% From {sty}\n")
                sty_content.append('\n'.join(new_content))

    sty_content.append("\n% ===== END CUSTOM MACRO DEFINITIONS =====\n")

    # Remove custom package includes
    for sty in sty_files:
        sty_base = sty.replace('.sty', '')
        content = re.sub(f'\\\\usepackage(\\[.*?\\])?\\{{{sty_base}\\}}\\s*\n?', '', content)

    # Find where to insert custom definitions (after standard packages)
    insert_pos = content.rfind('\\usepackage')
    if insert_pos != -1:
        # Find the end of this line
        insert_pos = content.find('\n', insert_pos) + 1
        content = content[:insert_pos] + ''.join(sty_content) + content[insert_pos:]

    # Replace subfiles package
    content = re.sub(r'\\usepackage\{subfiles\}\\s*\n?', '', content)

    # Inline all subfiles
    content = inline_subfiles(content, paper_dir)

    # Replace minted with listings
    content = replace_minted_with_listings(content)

    # Write output
    output_path = os.path.join(paper_dir, output_name)
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(content)

    print(f"Created flattened document: {output_path}")
    return True

def main():
    """Main function to flatten all papers."""
    base_dir = Path(__file__).parent

    papers = [
        ('bernoulli_sets', 'main_flattened.tex', 'main.tex'),
        ('bernoulli_maps', 'main_flattened.tex', 'bernoulli-maps.tex')
    ]

    for paper_info in papers:
        if len(paper_info) == 3:
            paper_dir, output_name, main_name = paper_info
        else:
            paper_dir, output_name = paper_info
            main_name = 'main.tex'

        full_dir = base_dir / paper_dir
        if full_dir.exists():
            print(f"\nFlattening {paper_dir}...")
            if flatten_paper(str(full_dir), output_name, main_name):
                # Try to compile
                os.system(f"cd {full_dir} && pdflatex -interaction=nonstopmode {output_name} > /dev/null 2>&1")
                pdf_name = output_name.replace('.tex', '.pdf')
                if os.path.exists(full_dir / pdf_name):
                    print(f"  ✓ PDF generated successfully: {pdf_name}")
                else:
                    print(f"  ⚠ Warning: PDF generation may have issues")

if __name__ == "__main__":
    main()