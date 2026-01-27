#!/usr/bin/env python3
"""
Tree-sitter Highlights Fetcher
Fetches highlights.scm files for C, C++, Go, TypeScript, JavaScript, and Rust
"""

import os
import urllib.request
import urllib.error
import ssl
from pathlib import Path

# Configuration
LANGUAGES = {
    "c": {
        "url": "https://raw.githubusercontent.com/tree-sitter/tree-sitter-c/master/queries/highlights.scm",
        "branch": "master"
    },
    "cpp": {
        "url": "https://raw.githubusercontent.com/tree-sitter/tree-sitter-cpp/master/queries/highlights.scm",
        "branch": "master"
    },
    "go": {
        "url": "https://raw.githubusercontent.com/tree-sitter/tree-sitter-go/master/queries/highlights.scm",
        "branch": "master"
    },
    "javascript": {
        "url": "https://raw.githubusercontent.com/tree-sitter/tree-sitter-javascript/master/queries/highlights.scm",
        "branch": "master"
    },
    "typescript": {
        "url": "https://raw.githubusercontent.com/tree-sitter/tree-sitter-typescript/master/queries/highlights.scm",
        "branch": "master"
    },
    "rust": {
        "url": "https://raw.githubusercontent.com/tree-sitter/tree-sitter-rust/master/queries/highlights.scm",
        "branch": "master"
    }
}

# Alternative: nvim-treesitter versions (uncomment to use instead)
# BASE_URL = "https://raw.githubusercontent.com/nvim-treesitter/nvim-treesitter/master/queries"
# LANGUAGES = {lang: {"url": f"{BASE_URL}/{lang}/highlights.scm"} for lang in ["c", "cpp", "go", "javascript", "typescript", "rust"]}

OUTPUT_DIR = Path("queries")


def create_ssl_context():
    """Create SSL context that allows us to connect to GitHub"""
    context = ssl.create_default_context()
    return context


def download_file(url: str, destination: Path) -> bool:
    """Download a file from URL to destination"""
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36'
    }

    request = urllib.request.Request(url, headers=headers)

    try:
        with urllib.request.urlopen(request, context=create_ssl_context(), timeout=30) as response:
            content = response.read()
            destination.write_bytes(content)
            return True
    except urllib.error.HTTPError as e:
        print(f"  HTTP Error {e.code}: {e.reason}")
        return False
    except urllib.error.URLError as e:
        print(f"  URL Error: {e.reason}")
        return False
    except Exception as e:
        print(f"  Error: {e}")
        return False


def fetch_highlights():
    """Main function to fetch all highlight files"""
    print("Tree-sitter Highlights Fetcher")
    print("=" * 50)

    # Create output directory
    OUTPUT_DIR.mkdir(exist_ok=True)
    print(f"Output directory: {OUTPUT_DIR.absolute()}")
    print()

    success_count = 0
    failed_languages = []

    for lang, config in LANGUAGES.items():
        print(f"Fetching {lang}...")

        # Create language directory
        lang_dir = OUTPUT_DIR / lang
        lang_dir.mkdir(exist_ok=True)

        # Create queries subdirectory (tree-sitter standard structure)
        queries_dir = lang_dir
        queries_dir.mkdir(exist_ok=True)

        destination = queries_dir / "highlights.scm"
        url = config["url"]

        if download_file(url, destination):
            # Get file size
            size = destination.stat().st_size
            print(f"  ✓ Saved to {destination} ({size} bytes)")
            success_count += 1
        else:
            print(f"  ✗ Failed to download {lang}")
            failed_languages.append(lang)

        print()

    # Summary
    print("=" * 50)
    print(f"Downloaded: {success_count}/{len(LANGUAGES)} languages")

    if failed_languages:
        print(f"Failed: {', '.join(failed_languages)}")
        print("\nTroubleshooting:")
        print("1. Check your internet connection")
        print("2. The repository might have changed branches (main vs master)")
        print("3. Try accessing the URLs manually in a browser")
        return 1

    print("\nDirectory structure created:")
    for lang in LANGUAGES:
        print(f"  {OUTPUT_DIR}/{lang}/queries/highlights.scm")

    return 0


def create_gitignore():
    """Create a .gitignore to ignore downloaded files if needed"""
    gitignore = OUTPUT_DIR / ".gitignore"
    content = """# Tree-sitter query files (auto-downloaded)
*/queries/*.scm

# Except if you want to track them, comment out the line above
"""
    gitignore.write_text(content)
    print(f"\nCreated {gitignore}")


if __name__ == "__main__":
    exit_code = fetch_highlights()
    create_gitignore()
    exit(exit_code)