# Code Quality

This project uses strict code quality expectations even before implementation begins.

## Code Expectations

- Consistent formatting is mandatory.
- Clear naming is mandatory.
- Public APIs must be small, intentional, and documented when non-obvious.
- Public API, configuration behavior, script behavior, test layout, and source layout changes must update the nearest relevant documentation in the same task.
- Prefer simple, explicit architecture over clever abstractions.
- Avoid speculative generalization.
- Keep module boundaries clear.
- Avoid hidden global state unless explicitly justified.
- Prefer deterministic behavior for core systems.
- Keep ownership, lifetime, and error handling obvious.
- Avoid platform-specific behavior leaking into platform-neutral modules.

## Architecture Expectations

- Every subsystem must have a clear responsibility.
- Cross-module dependencies must be intentional.
- Cross-platform assumptions must be explicit.
- Architectural decisions must be recorded in `docs/DECISIONS.md`.
- If an approach feels uncertain, the agent must stop and propose alternatives instead of pushing a shaky design.

## Formatting Policy

- Use `clang-format` with the repository `.clang-format` file.
- `scripts/format.ps1` is check-only by default.
- Use `scripts/format.ps1 -Fix` to apply formatting.
- Do not mix style within the same module.
- Formatting changes should not be bundled with unrelated logic changes unless explicitly requested.

## Static Analysis Policy

- Use `clang-tidy` with the repository `.clang-tidy` file.
- Use `cppcheck` with the repository `.cppcheck-suppressions` file.
- `scripts/lint.ps1` runs both tools and fails on findings. With generators that do not export `compile_commands.json`, `clang-tidy` falls back to module interface analysis and `cppcheck` still scans the source set.
- Generated build trees are excluded from formatting and linting.
- Keep suppressions narrow and only for confirmed false positives.

## Current Quality Scripts

- `scripts/configure.ps1` configures the CMake build directory.
- `scripts/build.ps1` builds the configured CMake project.
- `scripts/test.ps1` builds the current test executable and runs CTest.
- `scripts/format.ps1` checks formatting with `clang-format`; pass `-Fix` to rewrite formatting.
- `scripts/lint.ps1` runs `clang-tidy` and `cppcheck`.
- `scripts/install-tools.ps1` checks or installs the selected local C++ quality tools on Windows.
- `scripts/check.ps1` runs configure, format, lint, build, and test in order.

The selected C++ quality tools are required for the default quality gate. Use `scripts/install-tools.ps1 -Check` to verify local availability.
