# Testing

Tests are plain C++ executables. CTest is the current runner integration layer only; test code must not depend on CTest APIs.

## Scripted Execution

Run the default test flow from the repository root:

```powershell
.\scripts\test.ps1
```

The script configures the build directory when needed, builds the `core_tests`, `math_tests`, `math_assertion_tests`,
and `math_policy_tests` targets, includes the custom fixture target when enabled, and runs CTest with failure output
enabled.

## Direct Execution

The generated test executable can also be run directly after building.

Common output locations:

- Single-config generators: `build\bin\core_tests.exe`, `build\bin\math_tests.exe`,
  `build\bin\math_assertion_tests.exe`, and `build\bin\math_policy_tests.exe` on Windows, or their counterparts under
  `build/bin/` on Unix-like systems.
- Multi-config generators: `build\bin\Debug\core_tests.exe`, `build\bin\Debug\math_tests.exe`,
  `build\bin\Debug\math_assertion_tests.exe`, and `build\bin\Debug\math_policy_tests.exe` on Windows, or their
  counterparts under `build/bin/Debug/` on Unix-like systems.

## Math Policy Test

`math_policy_tests` is a dependency-free executable. It verifies that the supported toolchain provides a 32-bit
`float` and reports IEC 60559 floating-point behavior, the implementation assumptions for the initial math slice.
It intentionally does not test value APIs because those are covered by `math_tests`. CTest runs it in every configured
build type, including the default Debug and Release quality gate.

## Math Test

`math_tests` imports `moldy.math` without importing `moldy.core`. It covers HLSL-named float, signed, and unsigned
vectors; field aliases; free swizzles; arithmetic; direct normalization; square matrix multiplication; generic
constants; colors; RGB/HSL/HSV/sRGB conversions; quaternion strong typing, shared vector operations, identity,
normalization, inverse, vector rotation, explicit Hamilton products, right-handed 90-degree and 180-degree cases,
non-commutative composition, quaternion/matrix equivalence, conversion round trips, canonical sign, and explicit matrix
tolerances. Equality remains exact and structural.

## Math Assertion Tests

`math_assertion_tests` runs each precondition violation in a child process so expected termination cannot end the
CTest driver. Its private `math_assertion_fixture` build uses a mock assertion backend that exits with a deterministic
status instead of invoking the production debug break or abort path. This avoids interactive crash dialogs while still
proving that each precondition reaches `MOLDY_MATH_ASSERT`. Focused CTest cases cover zero scalar division, zero vector
length, a zero axis, a zero quaternion, a non-orthonormal matrix, and a reflection matrix in every configuration.

Verify that math builds without any core include, module import, or link dependency by building only the math targets
with the standalone backend:

```powershell
cmake -S . -B build-math-standalone -DMOLDY_MATH_ASSERT_BACKEND=standalone
cmake --build build-math-standalone --config Debug --target math_tests math_assertion_tests
ctest --test-dir build-math-standalone --build-config Debug --output-on-failure -R "^math_(tests|assertion_)"
cmake --build build-math-standalone --config Release --target math_tests math_assertion_tests
ctest --test-dir build-math-standalone --build-config Release --output-on-failure -R "^math_(tests|assertion_)"
```

The focused custom fixture supplies the configured macro from a header and its implementation from a separately
linked target:

```powershell
cmake -S . -B build-math-custom `
    -DMOLDY_MATH_ASSERT_BACKEND=custom `
    -DMOLDY_MATH_ASSERT_HEADER="$PWD/tests/math_custom_assert/custom_assert.hpp" `
    -DMOLDY_MATH_BUILD_CUSTOM_ASSERT_FIXTURE=ON
cmake --build build-math-custom --config Debug --target math_tests math_custom_assert_tests
ctest --test-dir build-math-custom --build-config Debug --output-on-failure -R "^math_(tests|custom_assert_tests)$"
```

## Intended Quality Gate

1. Configure the project.
2. Run format check.
3. Run lint and static analysis.
4. Build default targets.
5. Run unit tests.
6. Build examples when they exist.
7. Run benchmarks only when relevant.

## Current Tooling State

The selected quality tools are `clang-format`, `clang-tidy`, and `cppcheck`. `scripts/check.ps1` first runs the focused
toolchain-policy tests, runs the selected quality tools once, then builds and tests both `Debug` and `Release` by
default. This covers policy validation, assertion-enabled behavior, and the compiled-out assertion paths in the normal
quality gate. When a generator does not provide compile commands, the `clang-tidy` module fallback and cppcheck pass
analyze `moldy.math` with the standalone assertion compile definition.

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

Failing assertion tests are not run in-process because production failure behavior terminates the process. The
subprocess precondition cases use the deterministic mock backend in every configuration; production `core`,
`standalone`, and `custom` policies remain independently selected for the normal `math` target.

## Cross-Platform Expectations

Cross-platform behavior should eventually be verified on Windows, macOS, and Linux desktop.

## Completion Standard

Code is not considered complete if it bypasses formatting, linting, or architecture quality expectations without an explicit reason.
