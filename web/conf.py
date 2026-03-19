#!/usr/bin/env python
"""Sphinx configuration for the Probabilistic Library documentation."""

import os
import shutil
import subprocess
import sys
from pathlib import Path

import sphinx_autosummary_accessors

# ---------------------------------------------------------------------------
# Python path
# ---------------------------------------------------------------------------

sys.path.insert(0, os.path.abspath("../sources/Deltares.Probabilistic.Python"))


# ---------------------------------------------------------------------------
# Environment setup
# ---------------------------------------------------------------------------

# Ensure the active conda env's Library/bin is on PATH (Windows only)
if sys.platform.startswith("win") and "CONDA_PREFIX" in os.environ:
    conda_bin = os.path.join(os.environ["CONDA_PREFIX"], "Library", "bin")
    os.environ["PATH"] = conda_bin + os.pathsep + os.environ.get("PATH", "")

# Verify pandoc is available — required by nbsphinx to render notebooks
try:
    subprocess.run(["pandoc", "--version"], check=True, capture_output=True)
except Exception as e:
    print(f"Warning: pandoc not found — nbsphinx will fail to render notebooks. ({e})")

# Verify mermaid CLI is available — required to render mermaid diagrams
mmdc_path = shutil.which("mmdc")
if mmdc_path is None:
    raise RuntimeError(
        "Mermaid CLI 'mmdc' not found. "
        "Install with: npm install -g @mermaid-js/mermaid-cli "
        "and ensure the npm global bin folder is on PATH."
    )


# ---------------------------------------------------------------------------
# Notebook helpers — incremental copy, no unnecessary rebuilds
# ---------------------------------------------------------------------------

def _copy_notebooks_if_changed(src_folder: str, dst_folder: str) -> None:
    """Copy notebooks from src to dst only when the source is newer.

    Notebooks with 'Case' in their filename are skipped here — they are
    handled separately under examples/ as case study pages.
    """
    dst = Path(dst_folder)
    dst.mkdir(parents=True, exist_ok=True)
    copied = skipped = 0

    for src_file in Path(src_folder).rglob("*.ipynb"):
        if "Case" in src_file.name:
            continue
        dst_file = dst / src_file.name
        if not dst_file.exists() or src_file.stat().st_mtime > dst_file.stat().st_mtime:
            shutil.copy2(src_file, dst_file)
            copied += 1
        else:
            skipped += 1

    print(f"Notebooks: {copied} copied, {skipped} up-to-date.")


def _remove_stale_notebooks(src_folder: str, dst_folder: str) -> None:
    """Remove notebooks from dst that no longer exist in src."""
    src_names = {f.name for f in Path(src_folder).rglob("*.ipynb")}
    for dst_file in Path(dst_folder).rglob("*.ipynb"):
        if dst_file.name not in src_names:
            dst_file.unlink()
            print(f"Removed stale notebook: {dst_file.name}")


def _wipe_dir(path: str) -> None:
    """Remove a directory and all its contents."""
    if os.path.isdir(path):
        shutil.rmtree(path)


# ---------------------------------------------------------------------------
# Build preparation
# ---------------------------------------------------------------------------

# Sphinx output is fully disposable — always rebuild clean
_wipe_dir("build")

# _examples is a prepared source cache — update incrementally, don't wipe
os.makedirs("_examples", exist_ok=True)
_remove_stale_notebooks("notebooks", "_examples")
_copy_notebooks_if_changed("notebooks", "_examples")


# ---------------------------------------------------------------------------
# Project metadata
# ---------------------------------------------------------------------------

project   = "Probabilistic Library"
copyright = "2026, Deltares"
author    = "Rob Brinkman, Karolina Wojciechowska, Edwin Spee, Matthias Hauth"


# ---------------------------------------------------------------------------
# Extensions
# ---------------------------------------------------------------------------

extensions = [
    "sphinx_design",
    "myst_parser",          # kept for Markdown support in notebook cells
    "sphinx.ext.autodoc",
    "sphinx.ext.viewcode",
    "sphinx.ext.githubpages",
    "sphinx.ext.napoleon",
    "sphinx.ext.autosummary",
    "sphinx_autosummary_accessors",
    "nbsphinx",
    "sphinx_autodoc_typehints",
    "sphinxcontrib.mermaid",
]


# ---------------------------------------------------------------------------
# Source & templates
# ---------------------------------------------------------------------------

source_suffix  = ".rst"
master_doc     = "index"
language       = "en"
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]
templates_path = ["_templates", sphinx_autosummary_accessors.templates_path]

autosummary_generate  = True   # auto-generate stub .rst files for API docs
autodoc_typehints     = "description"  # type hints in description, not signature
add_module_names      = False  # shorten class names in docs

suppress_warnings = ["ref.unknown_target"]


# ---------------------------------------------------------------------------
# nbsphinx
# ---------------------------------------------------------------------------

nbsphinx_execute         = "never"   # notebooks are pre-executed, not run at build
nbsphinx_allow_errors    = True      # a broken notebook won't abort the whole build
nbsphinx_prompt_width    = "0px"
nbsphinx_show_input_prompt  = False
nbsphinx_show_output_prompt = False


# ---------------------------------------------------------------------------
# Mermaid
# ---------------------------------------------------------------------------

mermaid_version       = "10.4.0"
mermaid_output_format = "png"
mermaid_cmd_args      = ["--width", "800"]
mermaid_cmd           = mmdc_path


# ---------------------------------------------------------------------------
# HTML output
# ---------------------------------------------------------------------------

html_theme = "pydata_sphinx_theme"
html_logo  = "_resources/PTK_logo.png"
html_static_path = ["_resources"]
html_css_files   = ["theme-deltares.css", "python-code-block-style.css", "custom.css"]

html_theme_options = {
    "show_nav_level": 2,
    "navbar_align": "content",
    "use_edit_page_button": True,
    "navbar_end": ["navbar-icon-links"],  # removes the dark-mode toggle
    "icon_links": [
        {
            "name": "Forum",
            "url": "https://github.com/Deltares/ProbabilisticLibrary/discussions",
            "icon": "../../_resources/PTK_small_logo.png",
            "type": "local",
        },
        {
            "name": "Repo",
            "url": "https://github.com/Deltares/ProbabilisticLibrary",
            "icon": "../../_resources/github.svg",
            "type": "local",
        },
        {
            "name": "Python",
            "url": "https://pypi.org/project/probabilistic-library",
            "icon": "../../_resources/pypi.svg",
            "type": "local",
        },
        {
            "name": "PTK",
            "url": "https://www.deltares.nl/en/software-and-data/products/probabilistic-toolkit-ptk",
            "icon": "../../_resources/deltares.jpg",
            "type": "local",
        },
    ],
    "logo": {
        "text": "Probabilistic Library",
    },
}

html_context = {
    "github_url": "https://github.com",
    "github_user": "Deltares",
    "github_repo": "ProbabilisticLibrary",
    "github_version": "master",  # update if default branch changes
    "doc_path": "docs",
    "default_mode": "light",
}


# ---------------------------------------------------------------------------
# LaTeX output (defaults kept, customise if needed)
# ---------------------------------------------------------------------------

latex_elements = {}
htmlhelp_basename = "ProbLibdoc"
pygments_style = "sphinx"