# Moldy

<p align="center">
  <img src="docs/assets/icons/moldy-icon-readme.png" alt="Moldy icon" width="240">
</p>

Moldy is a C++23/CMake codebase for a cross-platform desktop game engine project.

The current repository state is a small, core-first scaffold: a `core` static library, a smoke executable, a plain C++ test executable, and scripted local workflow entry points. Renderer, editor, asset pipeline, platform layer, and concrete engine systems are not implemented yet.

## License

Moldy is licensed under the MIT License. See [LICENSE](LICENSE) for the full license text.

## Build And Test

Run the default local quality gate from the repository root:

```powershell
.\scripts\check.ps1
```

Useful individual entry points:

```powershell
.\scripts\configure.ps1
.\scripts\build.ps1
.\scripts\test.ps1
.\scripts\format.ps1
.\scripts\lint.ps1
```

Check whether the required local C++ quality tools are available:

```powershell
.\scripts\install-tools.ps1 -Check
```

`clang-format`, `clang-tidy`, and `cppcheck` are the selected local quality tools. `scripts/format.ps1` is check-only by default; pass `-Fix` to rewrite source formatting.

## Start Here

- [Project Brief](docs/PROJECT_BRIEF.md)
- [Source Map](docs/SOURCE_MAP.md)
- [AI Workflow](docs/AI_WORKFLOW.md)
- [Code Quality](docs/CODE_QUALITY.md)
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

## Current State

The repository contains a minimal C++23/CMake scaffold. Target platforms are Windows, macOS, and Linux desktop. Current work is infrastructure-first and does not claim completed 2D, 3D, rendering, editor, runtime, or platform-layer features.
