# Moldy

<p align="center">
  <img src="docs/assets/icons/moldy-icon-readme.png" alt="Moldy icon" width="240">
</p>

Moldy is a C++23/CMake codebase for a cross-platform desktop game engine project.

The current repository state is a small, core-first scaffold: a `core` static library, a smoke executable, a plain C++ test executable, and scripted local workflow entry points. Renderer, editor, asset pipeline, platform layer, and concrete engine systems are not implemented yet.

## Current State

This repository currently provides infrastructure for building and checking a small C++23 scaffold:

- `core`: a platform-neutral static library with the current `moldy.core` C++ module API.
- `smoke`: a minimal executable that links the core library and reports build information.
- `core_tests`: a plain C++ executable registered with CTest.
- `scripts/`: PowerShell entry points for configure, build, test, format, lint, and full local checks.

The repository does not yet contain renderer, editor, asset pipeline, platform layer, runtime system, or concrete 2D/3D engine implementation.

## License

Moldy is licensed under the MIT License. See [LICENSE](LICENSE) for the full license text.

## Prerequisites

- CMake 3.28 or newer is the project-file minimum. Supported development environments use CMake 4.3.4 through 4.4.0.
- A C++23-capable compiler and CMake generator with support for the repository's C++ module setup.
- PowerShell for the local workflow scripts.
- `clang-format`, `clang-tidy`, and `cppcheck` for the default quality gate.

The complete supported toolchain policy is stored in `config/toolchain.psd1`. Exact constraints are pinned versions; range constraints include both endpoints. On Windows, MSVC 19.34.0.0 (Visual Studio 2022 17.4) is the minimum supported compiler toolset.

Check whether the selected local quality tools are available:

```powershell
.\scripts\install-tools.ps1 -Check
```

On Windows, missing selected tools can be installed with explicit opt-in:

```powershell
.\scripts\install-tools.ps1 -InstallWindows
```

## Quick Start

From a fresh checkout, run the default local quality gate from the repository root:

```powershell
.\scripts\install-tools.ps1 -Check
.\scripts\check.ps1 -BuildDir build-check
```

`scripts/check.ps1` validates the toolchain policy, configures the project, checks formatting, runs lint/static analysis, then builds and tests both Debug and Release by default.

For an optimized build that keeps core assertions enabled, run:

```powershell
.\scripts\check.ps1 -BuildDir build-check-relwithdebinfo -Configuration RelWithDebInfo
```

To run the main steps individually:

```powershell
.\scripts\configure.ps1 -BuildDir build-check -Configuration Debug
.\scripts\build.ps1 -BuildDir build-check -Configuration Debug
.\scripts\test.ps1 -BuildDir build-check -Configuration Debug
```

Useful focused checks:

```powershell
.\scripts\format.ps1 -BuildDir build-check
.\scripts\lint.ps1 -BuildDir build-check
```

`clang-format`, `clang-tidy`, and `cppcheck` are the selected local quality tools. `scripts/format.ps1` is check-only by default; pass `-Fix` to rewrite source formatting.

## Generated Files

Build outputs are generated under directories such as `build/`, `build-check/`, or other `build-*` directories. They are not source files and should stay out of commits.

Before cleaning generated artifacts, preview ignored files:

```powershell
git clean -ndX
```

Remove only generated, ignored build or temporary directories that are no longer needed. Do not delete tracked files, source directories, documentation, scripts, or user-created unignored directories as part of routine cleanup.

## Start Here

- [Project Brief](docs/PROJECT_BRIEF.md)
- [Source Map](docs/SOURCE_MAP.md)
- [AI Workflow](docs/AI_WORKFLOW.md)
- [Scripts](scripts/README.md)
- [Code Quality](docs/CODE_QUALITY.md)
- [Core Diagnostics Runtime](docs/CORE_DIAGNOSTICS.md)
- [C++ Style](docs/CPP_STYLE.md)
- [Testing](docs/TESTING.md)
- [Roadmap](docs/ROADMAP.md)

## Repository Map

- `CMakeLists.txt`: top-level CMake project and target definitions.
- `src/core/`: current platform-neutral `core` library module.
- `apps/smoke/`: executable used to verify the library links and reports build information.
- `tests/core/`: plain C++ executable tests for the current core API.
- `scripts/`: PowerShell workflow entry points.
- `docs/`: project brief, decisions, source map, testing, style, and workflow documents.
- `tasks/`: current and future task notes.
