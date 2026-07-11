# Testing

Tests are plain C++ executables. CTest is the current runner integration layer only; test code must not depend on CTest APIs.

## Scripted Execution

Run the default test flow from the repository root:

```powershell
.\scripts\test.ps1
```

The script configures the build directory when needed, builds the `core_tests` target, and runs CTest with failure output enabled.

## Direct Execution

The generated test executable can also be run directly after building.

Common output locations:

- Single-config generators: `build\bin\core_tests.exe` on Windows or `build/bin/core_tests` on Unix-like systems.
- Multi-config generators: `build\bin\Debug\core_tests.exe` on Windows or `build/bin/Debug/core_tests` on Unix-like systems.

## Intended Quality Gate

1. Configure the project.
2. Run format check.
3. Run lint and static analysis.
4. Build default targets.
5. Run unit tests.
6. Build examples when they exist.
7. Run benchmarks only when relevant.

## Current Tooling State

The selected quality tools are `clang-format`, `clang-tidy`, and `cppcheck`. `scripts/check.ps1` runs them once, then builds and tests both `Debug` and `Release` by default. This covers assertion-enabled behavior and the compiled-out assertion paths in the normal quality gate.

Pass one or more explicit configurations to narrow or extend a run:

```powershell
.\scripts\check.ps1 -BuildDir build-check -Configuration Debug
.\scripts\check.ps1 -BuildDir build-check -Configuration Debug,Release,RelWithDebInfo
```

Core assertions are enabled in `Debug` and `RelWithDebInfo`. `RelWithDebInfo` is the optimized-with-asserts check path:

```powershell
.\scripts\check.ps1 -BuildDir build-check-relwithdebinfo -Configuration RelWithDebInfo
```

Failing assertion tests are not run in-process because correct failure behavior terminates the process. The default Debug and Release checks cover passing assertion macros, assert-enabled diagnostics, and compiled-out macro/error-return paths where feasible.

## Cross-Platform Expectations

Cross-platform behavior should eventually be verified on Windows, macOS, and Linux desktop.

## Completion Standard

Code is not considered complete if it bypasses formatting, linting, or architecture quality expectations without an explicit reason.
