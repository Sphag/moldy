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
3. Run lint or static analysis.
4. Build default targets.
5. Run unit tests.
6. Build examples when they exist.
7. Run benchmarks only when relevant.

## Current Tooling State

Formatter and linter/static-analysis tools are not selected yet. Their scripts report explicit placeholder-pass status and do not claim that formatting or linting was performed.

## Cross-Platform Expectations

Cross-platform behavior should eventually be verified on Windows, macOS, and Linux desktop.

## Completion Standard

Code is not considered complete if it bypasses formatting, linting, or architecture quality expectations without an explicit reason. Until formatter and linter tools are selected, missing tooling must remain explicit in script output and project decisions.
