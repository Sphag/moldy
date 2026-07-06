# Tests

`tests` contains plain C++ test executables.

## `core_tests`

`tests/core/test_main.cpp` builds the `core_tests` executable. It uses a small local assertion harness, links `project::core`, and is registered with CTest.

Run the current scripted test flow with:

```powershell
.\scripts\test.ps1
```

Keep tests and this README aligned when test executables, test layout, or runner behavior changes.
