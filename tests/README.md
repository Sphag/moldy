# Tests

`tests` contains plain C++ test executables.

## `core_tests`

`tests/core/test_main.cpp` builds the `core_tests` executable. It uses a small local assertion harness, links `project::core`, imports `moldy.core`, and is registered with CTest.

## `math_policy_tests`

`tests/math_policy/test_main.cpp` builds the dependency-free `math_policy_tests` executable. It verifies the
supported initial scalar assumptions: a 32-bit `float` and IEC 60559 floating-point behavior. It does not expose
math value types or a math module.

## `math_tests`

`tests/math/test_main.cpp` builds the `math_tests` executable. It links `project::math`, imports `moldy.math`, and
covers HLSL-named vectors, square matrices, generic constants, the shared color carrier, explicit
RGB/HSL/HSV/sRGB conversions, round trips, and alpha preservation.

Run the current scripted test flow with:

```powershell
.\scripts\test.ps1
```

Keep tests and this README aligned when test executables, test layout, or runner behavior changes.
