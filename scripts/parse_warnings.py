#!/usr/bin/env python3
"""
Run headless screenshots and parse stderr warnings into per-screen files.

Usage:
    python scripts/parse_warnings.py

Output:
    Creates audits/warnings/<screen_name>.md for each screen with warnings
    Creates audits/warnings/SUMMARY.md with overview
"""

import subprocess
import re
import os
from collections import defaultdict
from datetime import date

def run_headless_screenshots():
    """Run headless screenshots and capture output."""
    result = subprocess.run(
        ["./output/ui_tester.exe", "--headless-screenshots"],
        capture_output=True,
        text=False  # Get bytes
    )
    # Warnings go to stdout in headless mode, combine both
    combined = result.stdout + result.stderr
    # Decode with error handling for non-UTF8 bytes
    return combined.decode('utf-8', errors='replace')

def parse_warnings(stderr_output):
    """Parse stderr and group warnings by screen."""

    warnings_by_screen = defaultdict(list)
    pending_warnings = []  # Collect warnings until we see screen save

    # Patterns
    # Headless mode saves screenshots with "Saved: output/<screen>.png"
    screen_save = re.compile(r"\[Headless\] Saved: output/(\w+)\.png")

    # Warning patterns
    min_font = re.compile(r"MinFontSize: Font size ([\d.]+)px below minimum ([\d.]+)px \(entity: (\d+)\)")
    layout_overflow = re.compile(r"Layout overflow: '([^']+)' extends outside parent '([^']+)' bounds \(child_rel=\[([\d.,-]+)\], child_size=\[([\d.,-]+)\], child_end=\[([\d.,-]+)\], parent_size=\[([\d.,-]+)\)")
    layout_wrap = re.compile(r"Layout wrap: '([^']+)' in parent '([^']+)' - (.+?) \(child_size=([\d.]+), offset=([\d.]+), container=([\d.]+)\)")
    contrast_ratio = re.compile(r"ContrastRatio: .+ contrast ratio ([\d.]+):1 below minimum")
    container_too_small = re.compile(r"Container too small for text: container=([^,]+), margins=([^,]+), text='([^']+)'")
    theme_contrast = re.compile(r"Theme does not meet WCAG AA contrast requirements")

    # Strip ANSI escape codes
    ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')
    stderr_output = ansi_escape.sub('', stderr_output)

    for line in stderr_output.split('\n'):
        # Check for screen save (happens AFTER screen is rendered)
        match = screen_save.search(line)
        if match:
            screen_name = match.group(1)
            # Assign pending warnings to this screen
            for warning in pending_warnings:
                # Dedupe
                warning_key = (warning["type"], warning.get("message", ""))
                existing = [w for w in warnings_by_screen[screen_name] if (w["type"], w.get("message", "")) == warning_key]
                if not existing:
                    warnings_by_screen[screen_name].append(warning)
            pending_warnings = []
            continue

        # Check for warnings
        if "LOG_WARN" not in line:
            continue

        warning = None

        # MinFontSize
        match = min_font.search(line)
        if match:
            warning = {
                "type": "MinFontSize",
                "message": f"Font size {match.group(1)}px below minimum {match.group(2)}px",
                "entity": match.group(3)
            }

        # Layout overflow
        if not warning:
            match = layout_overflow.search(line)
            if match:
                warning = {
                    "type": "LayoutOverflow",
                    "message": f"'{match.group(1)}' extends outside parent '{match.group(2)}'",
                    "child": match.group(1),
                    "parent": match.group(2),
                    "child_size": match.group(4),
                    "parent_size": match.group(6)
                }

        # Layout wrap
        if not warning:
            match = layout_wrap.search(line)
            if match:
                warning = {
                    "type": "LayoutWrap",
                    "message": f"'{match.group(1)}' in parent '{match.group(2)}' - {match.group(3)}",
                    "child": match.group(1),
                    "parent": match.group(2),
                    "child_size": match.group(4),
                    "container_size": match.group(6)
                }

        # Contrast ratio
        if not warning:
            match = contrast_ratio.search(line)
            if match:
                warning = {
                    "type": "ContrastRatio",
                    "message": f"Contrast ratio {match.group(1)}:1 below minimum 4.5:1"
                }

        # Theme contrast
        if not warning:
            match = theme_contrast.search(line)
            if match:
                warning = {
                    "type": "ThemeContrast",
                    "message": "Theme does not meet WCAG AA contrast requirements"
                }

        # Container too small
        if not warning:
            match = container_too_small.search(line)
            if match:
                warning = {
                    "type": "ContainerTooSmall",
                    "message": f"Container too small for text: '{match.group(3)}'",
                    "container_size": match.group(1),
                    "margins": match.group(2),
                    "text": match.group(3)
                }

        # Skip singleton warnings (not layout related)
        if not warning and "Singleton map is missing" in line:
            continue

        # Generic warning fallback
        if not warning and "LOG_WARN" in line:
            # Extract message after LOG_WARN
            parts = line.split("LOG_WARN:")
            if len(parts) > 1:
                warning = {
                    "type": "Other",
                    "message": parts[1].strip()
                }

        if warning:
            pending_warnings.append(warning)

    # Remove screens with no warnings
    return {k: v for k, v in warnings_by_screen.items() if v}

def write_warning_files(warnings_by_screen, output_dir):
    """Write per-screen warning files."""
    os.makedirs(output_dir, exist_ok=True)
    today = date.today().isoformat()

    # Write per-screen files
    for screen, warnings in warnings_by_screen.items():
        filename = os.path.join(output_dir, f"{screen}.md")
        with open(filename, 'w') as f:
            f.write(f"# Layout Warnings: {screen}\n\n")
            f.write(f"**Generated:** {today}\n\n")
            f.write(f"**Total Warnings:** {len(warnings)}\n\n")
            f.write("---\n\n")

            # Group by type
            by_type = defaultdict(list)
            for w in warnings:
                by_type[w["type"]].append(w)

            for warning_type, type_warnings in by_type.items():
                f.write(f"## {warning_type} ({len(type_warnings)})\n\n")
                for w in type_warnings:
                    f.write(f"- {w['message']}\n")
                    if "child" in w and "parent" in w:
                        f.write(f"  - Child: `{w['child']}`, Parent: `{w['parent']}`\n")
                    if "child_size" in w:
                        f.write(f"  - Child size: {w['child_size']}\n")
                    if "parent_size" in w:
                        f.write(f"  - Parent size: {w['parent_size']}\n")
                    if "container_size" in w:
                        f.write(f"  - Container size: {w['container_size']}\n")
                f.write("\n")

        print(f"Wrote {filename} ({len(warnings)} warnings)")

    # Write summary
    summary_file = os.path.join(output_dir, "SUMMARY.md")
    with open(summary_file, 'w') as f:
        f.write(f"# Layout Warnings Summary\n\n")
        f.write(f"**Generated:** {today}\n\n")

        total = sum(len(w) for w in warnings_by_screen.values())
        f.write(f"**Total Screens with Warnings:** {len(warnings_by_screen)}\n")
        f.write(f"**Total Unique Warnings:** {total}\n\n")

        f.write("---\n\n")
        f.write("## Screens by Warning Count\n\n")

        sorted_screens = sorted(warnings_by_screen.items(), key=lambda x: -len(x[1]))
        for screen, warnings in sorted_screens:
            f.write(f"- **{screen}**: {len(warnings)} warnings\n")

    print(f"\nWrote {summary_file}")
    return summary_file

def main():
    print("Running headless screenshots...")
    stderr = run_headless_screenshots()

    print("Parsing warnings...")
    warnings = parse_warnings(stderr)

    if not warnings:
        print("No warnings found!")
        return

    output_dir = "audits/warnings"
    write_warning_files(warnings, output_dir)

    print(f"\nDone! {len(warnings)} screens with warnings.")
    print(f"Files written to: {output_dir}/")

if __name__ == "__main__":
    main()
