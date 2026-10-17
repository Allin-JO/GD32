#!/usr/bin/env python3
"""Validate the GitHub repository structure and text encoding."""

from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
REQUIRED = [
    "README.md",
    "CHANGELOG.md",
    "THIRD_PARTY_NOTICES.md",
    ".gitignore",
    ".gitattributes",
    ".clang-format",
    "docs/submission-document.docx",
    "docs/submission-document.pdf",
    "docs/submission-copy.md",
    "docs/CODE_STYLE.md",
    "docs/HARDWARE.md",
    "docs/PROTOCOL.md",
    "docs/assets/system-architecture.png",
    "docs/assets/rtthread-tasks.png",
    "firmware/gd32f303-main/USER/GD32F303_RTThread.uvprojx",
    "firmware/stm32f103-spray/SPRAY_RTThread.uvprojx",
    "scripts/build-keil.ps1",
    "scripts/format-code.ps1",
]
FORBIDDEN_DIRS = {"OBJ", "Objects", "Listings", "DebugConfig", "__pycache__"}
TEXT_EXTS = {".c", ".h", ".s", ".S", ".md", ".txt", ".ps1", ".py", ".uvprojx"}

errors = []

for rel in REQUIRED:
    if not (ROOT / rel).is_file():
        errors.append(f"missing required file: {rel}")

for path in ROOT.rglob("*"):
    if path.is_dir() and path.name in FORBIDDEN_DIRS:
        errors.append(f"build output directory should not be committed: {path.relative_to(ROOT)}")
    if path.is_file() and path.suffix in TEXT_EXTS:
        try:
            path.read_text(encoding="utf-8")
        except UnicodeDecodeError:
            errors.append(f"text file is not UTF-8: {path.relative_to(ROOT)}")

if errors:
    print("Repository check failed:")
    for item in errors:
        print(" -", item)
    sys.exit(1)

print("Repository check passed.")
