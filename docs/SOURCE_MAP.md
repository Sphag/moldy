# Source Map

This document maps the current repository scaffold. Keep it factual and update it when source layout, build targets, scripts, tests, or public APIs change.

## Repository Layout

- `CMakeLists.txt`: top-level CMake project, target definitions, compiler settings, module file set, and test registration.
- `src/core/`: platform-neutral `core` library module source.
- `apps/smoke/`: small executable used to verify that the core library links and reports build information.
- `tests/core/`: plain C++ executable tests for the current core API.
- `scripts/`: PowerShell entry points for configure, build, test, check, format, lint, tool setup, and benchmark placeholder flows.
- `docs/`: project workflow, quality, style, testing, decisions, architecture, and source navigation documents.
- `tasks/`: task planning notes.
- `build*/`: generated build trees. These are not source locations.

## CMake Project

The top-level CMake project is named `moldy` and requires CMake 3.28 or newer.

## Core Library

The `core` static library is defined from:

- `src/core/core.cppm`: public C++ module interface for `moldy.core`.
- `src/core/build_info.cpp`: module implementation unit for build-information functions.
- `src/core/status.cpp`: module implementation unit for `core::Status` behavior.

Current public API is imported with:

```cpp
import moldy.core;
```

Current public error API:

- `core::EErrorCode`: error categories for current recoverable core failures.
- `core::Status`: no-value success/error return type.
- `core::ResultValue`: concept for movable, non-void value types accepted by `core::Result<TValue>`.
- `core::Result<TValue>`: value-or-error return type for value-producing operations.

Current public build-information functions:

- `core::build_configuration()`: returns the CMake build configuration string, or a fallback when unavailable.
- `core::compiler_id()`: returns the CMake compiler identifier, or a fallback when unavailable.
- `core::language_standard()`: returns the `__cplusplus` value observed by the compiler.

The current API is intentionally limited to build-information reporting and basic error/result values. Do not document broader engine or runtime responsibilities until they exist in source.

## Applications

The `smoke` executable is defined from `apps/smoke/main.cpp`. It links `project::core`, imports `moldy.core`, and prints the current compiler, build configuration, and C++ language standard.

## Tests

The `core_tests` executable is defined from `tests/core/test_main.cpp`. It links `project::core`, imports `moldy.core`, uses a tiny local assertion harness, and is registered with CTest as `core_tests`.

Current checks verify that:

- The build configuration string is available.
- The compiler identifier string is available.
- The reported language standard is at least C++20.
- `core::Status::success()` reports success with an empty message.
- Error statuses preserve code and message.
- `core::Result<TValue>::success(...)` exposes a value and no error.
- `core::Result<TValue>::failure(...)` exposes an error and no value.
- `core::Result<TValue>::failure(core::Status::success())` normalizes to an internal error.
- `core::Result<core::Status>` can store a status payload.

## Scripts

- `scripts/configure.ps1`: configures the CMake build directory and handles single-config versus multi-config generators.
- `scripts/build.ps1`: configures, then builds the project for the requested configuration.
- `scripts/test.ps1`: configures, builds `core_tests`, then runs CTest with failure output enabled.
- `scripts/check.ps1`: runs configure, format, lint, build, and test in order.
- `scripts/format.ps1`: checks C++ formatting with `clang-format`; pass `-Fix` to apply formatting.
- `scripts/lint.ps1`: runs `clang-tidy` and `cppcheck` over source files, excluding generated build trees.
- `scripts/install-tools.ps1`: checks required tool availability and provides an explicit Windows install path.
- `scripts/bench.ps1`: reports placeholder-pass status because benchmarks are not configured yet.

## CMake Targets

- `core`: static library built as C++23 with a public `CXX_MODULES` file set.
- `project::core`: alias target for consumers of `core`.
- `smoke`: executable linked against `project::core`.
- `core_tests`: executable linked against `project::core` and registered as the current CTest test.

All project targets go through `configure_project_target`, which sets C++23 and disables compiler extensions. MSVC builds use `/W4 /permissive- /Zc:__cplusplus`; other compilers use `-Wall -Wextra -Wpedantic`.

The `core` target receives these private compile definitions from CMake:

- `CORE_BUILD_CONFIGURATION`: generated from `$<CONFIG>`.
- `CORE_COMPILER_ID`: generated from `CMAKE_CXX_COMPILER_ID`.

Runtime outputs are directed to `${CMAKE_BINARY_DIR}/bin`. Multi-config generators place configuration-specific executables under paths such as `build/bin/Debug/` or `build/bin/Release/`.
