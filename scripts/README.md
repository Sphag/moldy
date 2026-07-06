# Scripts

PowerShell scripts in this directory are the repository's current local workflow entry points.

- `configure.ps1`: configures the CMake build directory.
- `build.ps1`: configures, then builds the requested configuration.
- `test.ps1`: configures, builds `core_tests`, then runs CTest.
- `check.ps1`: runs configure, format, lint, build, and test in order.
- `format.ps1`: placeholder-pass script until a formatter is selected.
- `lint.ps1`: placeholder-pass script until a linter or static-analysis tool is selected.
- `bench.ps1`: placeholder-pass script until benchmarks are configured.

Scripts accept relative build directories from the repository root unless their parameters document otherwise. Keep this file current when script behavior, parameters, or expected checks change.
