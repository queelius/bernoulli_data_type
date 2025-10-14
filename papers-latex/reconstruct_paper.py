#!/usr/bin/env python3
r"""
Script to reconstruct a complete paper.tex from main.tex and its subfiles.
"""

import argparse
import os
import re
import shutil
from pathlib import Path


def find_subfiles(main_tex_content):
    """
    Find all \subfile{} commands in main.tex content.
    Returns list of tuples: (subfile_path, is_commented)
    """
    subfiles = []
    
    # Match both commented and uncommented subfile commands
    # Pattern explanation:
    # - ^\\s* : start of line with optional whitespace
    # - (%*)  : capture group for comment characters (%)
    # - \\s*  : optional whitespace after comments
    # - \\\\subfile\\{([^}]+)\\} : \subfile{filename}
    pattern = r'^\s*(%*)\s*\\subfile\{([^}]+)\}'
    
    for line in main_tex_content.split('\n'):
        match = re.match(pattern, line)
        if match:
            comment_chars = match.group(1)
            subfile_path = match.group(2)
            is_commented = len(comment_chars) > 0
            subfiles.append((subfile_path, is_commented))
    
    return subfiles


def read_subfile_content(subfile_path, base_dir):
    """Read content of a subfile, handling the subfiles package structure."""
    full_path = base_dir / f"{subfile_path}.tex"
    
    if not full_path.exists():
        missing_info = f"""% ===============================================================================
% WARNING: MISSING FILE - {subfile_path}.tex
% 
% The subfile '{subfile_path}.tex' was referenced in main.tex but could not be found.
% Expected location: {full_path}
% 
% Based on the filename, this section might have contained:
{_guess_content_from_filename(subfile_path)}%
% You may need to:
% 1. Check if the file was moved or renamed
% 2. Create the missing file with appropriate content
% 3. Update the \\subfile{{}} reference in main.tex
% ===============================================================================
"""
        return missing_info
    
    try:
        with open(full_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Remove subfiles package boilerplate if present
        # Remove \documentclass[main.tex]{subfiles}
        content = re.sub(r'\\documentclass\[[^\]]*\]\{subfiles\}\s*\n?', '', content)
        
        # Remove \begin{document} and \end{document}
        content = re.sub(r'\\begin\{document\}\s*\n?', '', content)
        content = re.sub(r'\\end\{document\}\s*\n?', '', content)
        
        # Clean up extra newlines at start/end
        content = content.strip()
        
        return content + '\n'
        
    except Exception as e:
        error_info = f"""% ===============================================================================
% ERROR: Could not read {subfile_path}.tex
% 
% File exists at: {full_path}
% But encountered error: {e}
% 
% Please check file permissions and encoding.
% ===============================================================================
"""
        return error_info


def _guess_content_from_filename(filename):
    """Provide hints about what content might be expected based on filename."""
    filename_lower = filename.lower()
    guesses = []
    
    if 'intro' in filename_lower:
        guesses.append("% - Introduction or overview content")
    if 'abstract' in filename_lower:
        guesses.append("% - Abstract section")
    if 'conclusion' in filename_lower:
        guesses.append("% - Conclusion or summary")
    if 'method' in filename_lower or 'approach' in filename_lower:
        guesses.append("% - Methodology or approach description")
    if 'result' in filename_lower:
        guesses.append("% - Results and analysis")
    if 'related' in filename_lower or 'background' in filename_lower:
        guesses.append("% - Related work or background information")
    if 'experiment' in filename_lower:
        guesses.append("% - Experimental setup and results")
    if 'eval' in filename_lower:
        guesses.append("% - Evaluation or assessment")
    if 'impl' in filename_lower or 'implementation' in filename_lower:
        guesses.append("% - Implementation details")
    if 'future' in filename_lower:
        guesses.append("% - Future work section")
    if 'appendix' in filename_lower:
        guesses.append("% - Appendix material")
    if 'algo' in filename_lower or 'algorithm' in filename_lower:
        guesses.append("% - Algorithm descriptions")
    if 'math' in filename_lower or 'proof' in filename_lower:
        guesses.append("% - Mathematical proofs or derivations")
    
    if not guesses:
        guesses.append("% - Content related to: " + filename.replace('_', ' ').replace('-', ' '))
    
    return '\n'.join(guesses) + '\n'


def find_custom_packages(main_tex_content, base_dir):
    """
    Find custom .sty packages referenced in the LaTeX file.
    Returns list of package names that might be custom.
    """
    packages = []
    
    # Find all \usepackage{} commands
    package_pattern = r'\\usepackage(?:\[[^\]]*\])?\{([^}]+)\}'
    
    for match in re.finditer(package_pattern, main_tex_content):
        package_name = match.group(1)
        
        # Skip standard LaTeX packages (common ones)
        standard_packages = {
            'amsmath', 'amssymb', 'amsthm', 'babel', 'booktabs', 'caption', 'cleveref',
            'enumitem', 'fontenc', 'geometry', 'graphicx', 'hyperref', 'inputenc',
            'mathtools', 'microtype', 'natbib', 'pgfplots', 'siunitx', 'subfiles',
            'tikz', 'tikzscale', 'algorithm2e', 'commath', 'minted', 'enumerate',
            'appendix', 'subcaption', 'array', 'tabu', 'placeins', 'comment',
            'lmodern', 'nth'
        }
        
        if package_name not in standard_packages:
            packages.append(package_name)
    
    return packages


def find_style_files(package_names, style_search_paths):
    """
    Find .sty files for the given package names in the search paths.
    Returns dict mapping package_name -> full_path
    """
    found_styles = {}
    
    for package_name in package_names:
        style_filename = f"{package_name}.sty"
        
        for search_path in style_search_paths:
            style_path = Path(search_path) / style_filename
            if style_path.exists():
                found_styles[package_name] = style_path
                break
    
    return found_styles


def copy_style_files(style_files, output_dir, dry_run=False):
    """Copy style files to the output directory."""
    if not style_files:
        return
    
    output_path = Path(output_dir)
    if not dry_run:
        output_path.mkdir(parents=True, exist_ok=True)
    
    print(f"\nCustom style files to copy:")
    for package_name, source_path in style_files.items():
        dest_path = output_path / f"{package_name}.sty"
        print(f"  - {package_name}.sty from {source_path}")
        
        if not dry_run:
            try:
                shutil.copy2(source_path, dest_path)
                print(f"    ✓ Copied to {dest_path}")
            except Exception as e:
                print(f"    ✗ Failed to copy: {e}")
        else:
            print(f"    [DRY RUN] Would copy to {dest_path}")


def copy_all_style_files(style_search_paths, output_dir, dry_run=False):
    """Copy ALL .sty files from the search paths to output directory."""
    output_path = Path(output_dir)
    if not dry_run:
        output_path.mkdir(parents=True, exist_ok=True)
    
    all_styles = {}
    
    # Find all .sty files in search paths
    for search_path in style_search_paths:
        search_dir = Path(search_path)
        if search_dir.exists():
            for sty_file in search_dir.glob("*.sty"):
                package_name = sty_file.stem
                if package_name not in all_styles:  # Don't overwrite if found in earlier path
                    all_styles[package_name] = sty_file
    
    if not all_styles:
        print("No .sty files found in search paths")
        return
    
    print(f"\nCopying ALL {len(all_styles)} style files found:")
    for package_name, source_path in all_styles.items():
        dest_path = output_path / f"{package_name}.sty"
        print(f"  - {package_name}.sty from {source_path}")
        
        if not dry_run:
            try:
                shutil.copy2(source_path, dest_path)
                print(f"    ✓ Copied to {dest_path}")
            except Exception as e:
                print(f"    ✗ Failed to copy: {e}")
        else:
            print(f"    [DRY RUN] Would copy to {dest_path}")


def comment_out_content(content):
    """Wrap content in a comment environment."""
    # Use the comment package's comment environment
    return f"\\begin{{comment}}\n{content}\\end{{comment}}\n"


def fix_latex_errors(content):
    """Apply common LaTeX error fixes to content."""
    fixes_applied = []
    
    # Fix 1: Add missing control sequences that appear in errors
    undefined_commands = [
        r'\AFun', r'\BFun', r'\CFun'  # Add more as needed
    ]
    
    for cmd in undefined_commands:
        if cmd in content and f'\\newcommand{{{cmd}}}' not in content:
            # Add a generic definition for missing commands
            definition = f'\\newcommand{{{cmd}}}[1]{{\\mathrm{{AF}}(#1)}}  % Auto-generated definition\n'
            # Insert after \begin{document} or near the beginning
            if '\\begin{document}' in content:
                content = content.replace('\\begin{document}', definition + '\\begin{document}')
            else:
                # Insert after packages
                lines = content.split('\n')
                insert_pos = 0
                for i, line in enumerate(lines):
                    if line.strip().startswith('\\usepackage'):
                        insert_pos = i + 1
                lines.insert(insert_pos, definition.strip())
                content = '\n'.join(lines)
            fixes_applied.append(f'Added definition for {cmd}')
    
    # Fix 2: Remove duplicate equation labels by making them unique
    import re
    equation_labels = []
    def replace_duplicate_label(match):
        label = match.group(1)
        if label in equation_labels:
            new_label = f"{label}_dup{len([l for l in equation_labels if l.startswith(label)])}"
            equation_labels.append(new_label)
            fixes_applied.append(f'Renamed duplicate label {label} -> {new_label}')
            return f'\\label{{{new_label}}}'
        else:
            equation_labels.append(label)
            return match.group(0)
    
    content = re.sub(r'\\label\{([^}]+)\}', replace_duplicate_label, content)
    
    # Fix 3: Comment out undefined references to prevent errors
    undefined_refs = re.findall(r'\\ref\{([^}]+)\}', content)
    for ref in undefined_refs:
        if ref == 'dummyref' or ref.startswith('dummy'):
            content = content.replace(f'\\ref{{{ref}}}', f'[REF-MISSING: {ref}]')
            fixes_applied.append(f'Commented out dummy reference {ref}')
    
    # Fix 4: Add missing packages that are commonly needed
    missing_packages = []
    if '\\AFun' in content or '\\BFun' in content:
        missing_packages.append('amsmath')
    
    for pkg in missing_packages:
        if f'\\usepackage{{{pkg}}}' not in content:
            content = content.replace('\\begin{document}', f'\\usepackage{{{pkg}}}\n\\begin{document}')
            fixes_applied.append(f'Added missing package {pkg}')
    
    return content, fixes_applied


def reconstruct_paper(main_tex_path, output_path, include_commented=False, remove_included=False, 
                     dry_run=False, copy_styles=False, copy_all_styles=False, fix_errors=False, 
                     style_search_paths=None):
    """
    Reconstruct a complete paper.tex from main.tex and subfiles.
    """
    main_tex_path = Path(main_tex_path)
    base_dir = main_tex_path.parent
    
    # Read main.tex
    try:
        with open(main_tex_path, 'r', encoding='utf-8') as f:
            main_content = f.read()
    except Exception as e:
        print(f"Error reading {main_tex_path}: {e}")
        return False
    
    # Find all subfiles
    subfiles = find_subfiles(main_content)
    print(f"Found {len(subfiles)} subfile references:")
    
    files_to_remove = []
    for subfile_path, is_commented in subfiles:
        status = "commented" if is_commented else "active"
        subfile_full_path = base_dir / f"{subfile_path}.tex"
        exists = subfile_full_path.exists()
        
        print(f"  - {subfile_path}.tex ({status}{', exists' if exists else ', MISSING'})")
        
        # Track files that will be included and potentially removed
        if exists and (not is_commented or include_commented):
            if remove_included:
                files_to_remove.append(subfile_full_path)
    
    # Show what files would be removed if --remove-included is used
    if remove_included and files_to_remove:
        print(f"\nFiles to be removed after inclusion:")
        for file_path in files_to_remove:
            print(f"  - {file_path}")
        
        if dry_run:
            print("  [DRY RUN - files will NOT actually be deleted]")
    
    # Handle custom style files if requested
    style_files = {}
    if copy_styles or copy_all_styles:
        if style_search_paths is None:
            # Default search paths
            style_search_paths = [
                os.path.expanduser("~/github/repos/latex/texmf/tex/latex"),
                base_dir,  # Also check the paper's directory
                "."  # Current directory
            ]
        
        output_dir = Path(output_path).parent
        
        if copy_all_styles:
            # Copy ALL .sty files from search paths
            copy_all_style_files(style_search_paths, output_dir, dry_run)
        elif copy_styles:
            # Only copy detected custom packages
            custom_packages = find_custom_packages(main_content, base_dir)
            if custom_packages:
                print(f"\nFound {len(custom_packages)} potential custom packages: {', '.join(custom_packages)}")
                style_files = find_style_files(custom_packages, style_search_paths)
                
                if style_files:
                    copy_style_files(style_files, output_dir, dry_run)
                else:
                    print("  No custom style files found in search paths")
    
    if dry_run:
        print(f"\n[DRY RUN] Would write reconstructed paper to: {output_path}")
        print("[DRY RUN] No files will be modified or deleted.")
    
    # Build the reconstructed content
    output_lines = []
    
    for line in main_content.split('\n'):
        # Check if this line is a subfile command
        match = re.match(r'^\s*(%*)\s*\\subfile\{([^}]+)\}', line)
        
        if match:
            comment_chars = match.group(1)
            subfile_path = match.group(2)
            is_commented = len(comment_chars) > 0
            
            # Skip commented subfiles unless --include-commented is specified
            if is_commented and not include_commented:
                output_lines.append(line)  # Keep the commented \subfile line
                continue
            
            # Add a comment indicating the source
            output_lines.append(f"% === Content from {subfile_path}.tex ===")
            
            # Read and include the subfile content
            subfile_content = read_subfile_content(subfile_path, base_dir)
            
            if is_commented and include_commented:
                # Comment out the entire subfile content
                commented_content = comment_out_content(subfile_content)
                output_lines.append(commented_content.rstrip())
            else:
                # Include content as-is
                output_lines.append(subfile_content.rstrip())
            
            output_lines.append(f"% === End of {subfile_path}.tex ===")
            output_lines.append("")  # Add blank line for readability
        else:
            # Regular line, keep as-is
            output_lines.append(line)
    
    # Add comment package if we're including commented sections
    if include_commented and any(is_commented for _, is_commented in subfiles):
        # Insert \usepackage{comment} after other packages
        for i, line in enumerate(output_lines):
            if line.strip().startswith('\\usepackage') and i > 0:
                # Find a good place to insert the comment package
                continue
            elif line.strip().startswith('\\begin{document}'):
                # Insert before \begin{document} if we haven't found packages
                output_lines.insert(i, '\\usepackage{comment}')
                break
        else:
            # If no \begin{document} found, add near the top
            output_lines.insert(1, '\\usepackage{comment}')
    
    # Write the output
    output_content = '\n'.join(output_lines)
    
    # Apply error fixes if requested
    if fix_errors:
        print("\nApplying LaTeX error fixes...")
        output_content, fixes_applied = fix_latex_errors(output_content)
        if fixes_applied:
            for fix in fixes_applied:
                print(f"  ✓ {fix}")
        else:
            print("  No fixes needed")
    
    if not dry_run:
        try:
            with open(output_path, 'w', encoding='utf-8') as f:
                f.write(output_content)
            print(f"Successfully wrote reconstructed paper to {output_path}")
            
            # Remove included files if requested
            if remove_included and files_to_remove:
                print(f"\nRemoving {len(files_to_remove)} included files:")
                for file_path in files_to_remove:
                    try:
                        os.unlink(file_path)
                        print(f"  ✓ Removed {file_path}")
                    except Exception as e:
                        print(f"  ✗ Failed to remove {file_path}: {e}")
            
            return True
        except Exception as e:
            print(f"Error writing to {output_path}: {e}")
            return False
    else:
        print(f"\n[DRY RUN] Content preview (first 20 lines):")
        preview_lines = output_content.split('\n')[:20]
        for i, line in enumerate(preview_lines, 1):
            print(f"  {i:2d}: {line}")
        if len(output_content.split('\n')) > 20:
            print(f"  ... ({len(output_content.split('\n')) - 20} more lines)")
        return True


def main():
    parser = argparse.ArgumentParser(
        description='Reconstruct a complete paper.tex from main.tex and subfiles'
    )
    parser.add_argument(
        'main_tex',
        help='Path to main.tex file'
    )
    parser.add_argument(
        '-o', '--output',
        default='paper.tex',
        help='Output filename (default: paper.tex)'
    )
    parser.add_argument(
        '--include-commented',
        action='store_true',
        help='Include commented-out subfiles (wrapped in comment environment)'
    )
    parser.add_argument(
        '--remove-included',
        action='store_true',
        help='Delete original subfiles after including them in the output'
    )
    parser.add_argument(
        '--dry-run',
        action='store_true',
        help='Show what would be done without actually modifying any files'
    )
    parser.add_argument(
        '--copy-styles',
        action='store_true',
        help='Copy custom .sty files to output directory (detected from \\usepackage commands)'
    )
    parser.add_argument(
        '--copy-all-styles',
        action='store_true',
        help='Copy ALL .sty files found in search paths (more comprehensive)'
    )
    parser.add_argument(
        '--fix-errors',
        action='store_true',
        help='Automatically fix common LaTeX compilation errors'
    )
    parser.add_argument(
        '--style-path',
        action='append',
        help='Additional paths to search for .sty files (can be used multiple times)'
    )
    
    args = parser.parse_args()
    
    if not os.path.exists(args.main_tex):
        print(f"Error: {args.main_tex} does not exist")
        return 1
    
    # Build style search paths
    style_search_paths = None
    if args.copy_styles or args.copy_all_styles:
        style_search_paths = [
            os.path.expanduser("~/github/repos/latex/texmf/tex/latex"),
        ]
        if args.style_path:
            style_search_paths.extend(args.style_path)
    
    success = reconstruct_paper(args.main_tex, args.output, args.include_commented, 
                               args.remove_included, args.dry_run, args.copy_styles,
                               args.copy_all_styles, args.fix_errors, style_search_paths)
    return 0 if success else 1


if __name__ == '__main__':
    exit(main())