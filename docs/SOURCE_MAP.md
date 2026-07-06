# Source Map

This document maps the current repository scaffold. Keep it factual and update it when source layout, build targets, scripts, tests, or public APIs change.

## Repository Layout

- `CMakeLists.txt`: top-level CMake project, target definitions, compiler settings, and test registration.
- `src/core/`: platform-neutral `core` library source and public headers.
- `apps/smoke/`: small executable used to verify that the core library links and reports build information.
- `tests/core/`: plain C++ executable tests for the current core API.
- `scripts/`: PowerShell entry points for configure, build, test, check, formatting placeholder, lint placeholder, and benchmark placeholder flows.
- `docs/`: project workflow, quality, testing, decisions, architecture, and source navigation documents.
- `tasks/`: task planning notes.
- `build*/`: generated build trees. These are not source locations.

## CMake Project

The top-level CMake project is named `moldy`.

## Core Library

The `core` static library is defined from `src/core/build_info.cpp` and exposes public headers through the `src` include root.

Current public API:

- `core::build_configuration()`: returns the CMake build configuration string, or a fallback when unavailable.
- `core::compiler_id()`: returns the CMake compiler identifier, or a fallback when unavailable.
- `core::language_standard()`: returns the `__cplusplus` value observed by the compiler.

The current API is intentionally limited to build-information reporting. Do not document broader engine or runtime responsibilities until they exist in source.

## Applications

The `smoke` executable is defined from `apps/smoke/main.cpp`. It links `project::core` and prints the current compiler, build configuration, and C++ language standard.

## Tests

The `core_tests` executable is defined from `tests/core/test_main.cpp`. It uses a tiny local assertion harness and is registered with CTest as `core_tests`.

Current checks verify that:

- The build configuration string is available.
- The compiler identifier string is available.
- The reported language standard is at least C++20.

## Scripts

- `scripts/configure.ps1`: configures the CMake build directory and handles single-config versus multi-config generators.
- `scripts/build.ps1`: configures, then builds the project for the requested configuration.
- `scripts/test.ps1`: configures, builds `core_tests`, then runs CTest with failure output enabled.
- `scripts/check.ps1`: runs configure, format, lint, build, and test in order.
- `scripts/format.ps1`: reports placeholder-pass status because no formatter is selected yet.
- `scripts/lint.ps1`: reports placeholder-pass status because no linter or static-analysis tool is selected yet.
- `scripts/bench.ps1`: reports placeholder-pass status because benchmarks are not configured yet.

## CMake Targets

- `core`: static library built as C++23.
- `project::core`: alias target for consumers of `core`.
- `smoke`: executable linked against `project::core`.
- `core_tests`: executable linked against `project::core` and registered as the current CTest test.

All project targets go through `configure_project_target`, which sets C++23 and disables compiler extensions. MSVC builds use `/W4 /permissive- /Zc:__cplusplus`; other compilers use `-Wall -Wextra -Wpedantic`.

The `core` target receives these private compile definitions from CMake:

- `CORE_BUILD_CONFIGURATION`: generated from `$<CONFIG>`.
- `CORE_COMPILER_ID`: generated from `CMAKE_CXX_COMPILER_ID`.

Runtime outputs are directed to `${CMAKE_BINARY_DIR}/bin`. Multi-config generators place configuration-specific executables under paths such as `build/bin/Debug/` or `build/bin/Release/`.
