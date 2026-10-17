# Code style

Project-owned C sources use the repository-level `.clang-format`:

- 4-space indentation, no tabs
- 120-column limit
- attached braces
- pointer alignment to the right
- includes are not reordered automatically

Vendor directories (`rt-thread/`, `GD32F30x_FWLib/`, `Library/`, `Start/`) are
kept in their upstream formatting and are not reformatted.

Run `scripts/format-code.ps1` after installing `clang-format` locally.
