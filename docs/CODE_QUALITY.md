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

## Line Ending Policy

- Tracked source files, CMake files, PowerShell scripts, Markdown documentation, and repository configuration files use LF line endings through `.gitattributes`.
- Generated build trees and binary outputs are excluded from line ending normalization.

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
- `scripts/install-tools.ps1` checks or installs the selected local C++ quality tools on Windows and rejects unhealthy binaries that cannot run the repository probes.
- `scripts/check.ps1` runs configure, format, and lint once, then builds and tests Debug and Release by default.

The selected C++ quality tools are required for the default quality gate. Use `scripts/install-tools.ps1 -Check` to verify local availability and basic tool health before running the full gate.

## Supported Toolchain Policy

`config/toolchain.psd1` is the single source of supported development-tool versions. An `Exact` constraint accepts only its `Version`; a `Range` constraint includes both `MinVersion` and `MaxVersion`. The current policy pins LLVM 19.1.7 and cppcheck, and defines tested ranges for CMake, Ninja, and MSVC. LLVM 19 is required because supported recent MSVC toolsets ship STL headers that reject older Clang-based analysis binaries.

CMake 3.28 remains the minimum version that can interpret the project files. It is not the supported development range, which is recorded separately in the manifest. The supported MSVC range starts at 19.34.0.0, corresponding to Visual Studio 2022 17.4 and the compiler baseline used for CMake C++ module scanning.

When changing supported versions, update the manifest and its documentation together. Confirm the local quality gate and Windows CI before accepting the change, and do not widen a range beyond versions that have passed those checks. Installed-tool version enforcement is intentionally handled separately from this policy definition.

## Known Local Sandbox Limitation

Some AI-agent command sandboxes on Windows can create and write files but cannot delete, replace, or rename them. CMake, MSBuild, Ninja, and `clang-format -i` all rely on those operations for generated files or in-place formatting.

Known symptoms:

- `CMake Error: Cannot restore timestamp ".../CMakeFiles/generate.stamp": Access is denied.`
- CMake `try_compile` failures that mention generated `.tmp` files that could not be removed.
- `clang-format -i` failing with `permission denied` even though the same file can be rewritten directly.

When these symptoms appear in a restricted agent session, first verify whether a simple create/delete probe also fails in the workspace. If deletion is blocked, rerun the quality gate from a normal terminal or through an approved elevated command run. Do not treat this as a source, CMake, or toolchain regression unless the same command also fails outside the restricted sandbox.
