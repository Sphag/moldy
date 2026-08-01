# Testing

Tests are plain C++ executables. CTest is the current runner integration layer only; test code must not depend on CTest APIs.

## Scripted Execution

Run the default test flow from the repository root:

```powershell
.\scripts\test.ps1
```

The script configures the build directory when needed, builds the `core_tests`, `math_tests`, and `math_policy_tests`
targets, and runs CTest with failure output enabled.

## Direct Execution

The generated test executable can also be run directly after building.

Common output locations:

- Single-config generators: `build\bin\core_tests.exe`, `build\bin\math_tests.exe`, and
  `build\bin\math_policy_tests.exe` on Windows, or their counterparts under `build/bin/` on Unix-like systems.
- Multi-config generators: `build\bin\Debug\core_tests.exe`, `build\bin\Debug\math_tests.exe`, and
  `build\bin\Debug\math_policy_tests.exe` on Windows, or their counterparts under `build/bin/Debug/` on Unix-like
  systems.

## Math Policy Test

`math_policy_tests` is a dependency-free executable. It verifies that the supported toolchain provides a 32-bit
`float` and reports IEC 60559 floating-point behavior, the implementation assumptions for the initial math slice.
It intentionally does not test vector, quaternion, matrix, or geometry APIs because those APIs are not introduced
by the policy work. CTest runs it in every configured build type, including the default Debug and Release quality gate.

## Math Test

`math_tests` imports `moldy.math` without importing or linking `moldy.core`. It covers HLSL-named float, signed, and
unsigned vectors; field aliases; free swizzles; arithmetic; dot and cross products; optional normalization; square
matrix multiplication; generic constants; colors; and RGB/HSL/HSV/sRGB conversions. It also verifies that zero vectors
return an empty normalized result and that equality does not introduce an implicit tolerance.

## Intended Quality Gate

1. Configure the project.
2. Run format check.
3. Run lint and static analysis.
4. Build default targets.
5. Run unit tests.
6. Build examples when they exist.
7. Run benchmarks only when relevant.

## Current Tooling State

The selected quality tools are `clang-format`, `clang-tidy`, and `cppcheck`. `scripts/check.ps1` first runs the focused toolchain-policy tests, runs the selected quality tools once, then builds and tests both `Debug` and `Release` by default. This covers policy validation, assertion-enabled behavior, and the compiled-out assertion paths in the normal quality gate.

Before probing the native toolchain, `scripts/check.ps1` also runs `scripts/test-agent-skills.ps1`. This dependency-free check validates the approved foundational skill set, their open-format metadata and safety boundaries, and smoke cases for valid content, missing prerequisites, denied approvals, and missing failure reporting.

`scripts/check.ps1` next runs `scripts/test-ai-workflow.ps1`, also before native tool probing. The focused PowerShell tests cover normal JSON output, malformed and outside-repository paths, ignored and generated paths, unsupported encodings, redaction, the 64 KiB content bound, changed-check fallback selection, unsafe diagnostics output, partial diagnostics manifests, and missing-prerequisite classification. The tests create and remove only `temp/workflow-script-tests`, which is ignored.

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
