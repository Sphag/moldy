# Scripts

PowerShell scripts in this directory are the repository's current local workflow entry points.

- `configure.ps1`: configures the CMake build directory.
- `build.ps1`: configures, then builds the requested configuration.
- `test.ps1`: configures, builds `core_tests`, then runs CTest.
- `check.ps1`: validates the toolchain policy, then runs configure, format, and lint once before building and testing `Debug` and `Release` by default. Pass `-Configuration` with one or more values to select a different set.
- `format.ps1`: checks C++ formatting with `clang-format`; pass `-Fix` to apply formatting.
- `lint.ps1`: runs `clang-tidy` and `cppcheck`, excluding generated build trees. Generators with `compile_commands.json` get full clang-tidy coverage; Visual Studio-style generators use module-interface fallback coverage.
- `install-tools.ps1`: checks selected local C++ quality tools, rejects unhealthy tool binaries, and provides an explicit Windows install path.
- `toolchain-policy.ps1`: provides the side-effect-free `Import-ToolchainPolicy` validation helper for `config/toolchain.psd1`.
- `test-toolchain-policy.ps1`: runs dependency-free focused tests for the toolchain policy schema and constraints.
- `bench.ps1`: placeholder-pass script until benchmarks are configured.

Scripts accept relative build directories from the repository root unless their parameters document otherwise. Keep this file current when script behavior, parameters, or expected checks change.

Use `RelWithDebInfo` for optimized local checks that keep core assertions enabled:

```powershell
.\scripts\check.ps1 -BuildDir build-check-relwithdebinfo -Configuration RelWithDebInfo
```

To extend the default configuration set instead of replacing it:

```powershell
.\scripts\check.ps1 -BuildDir build-check -Configuration Debug,Release,RelWithDebInfo
```

## Tool Setup

Check required local tools with:

```powershell
.\scripts\install-tools.ps1 -Check
```

On Windows, install missing selected tools with explicit opt-in:

```powershell
.\scripts\install-tools.ps1 -InstallWindows
```

The install path uses `winget` and installs LLVM tools plus cppcheck when missing or when an existing binary fails the script health probe. In GitHub Actions, healthy tool directories are written to `GITHUB_PATH` so later steps use the validated binaries.

## Toolchain Policy

Load and validate the repository policy without probing installed binaries:

```powershell
. .\scripts\toolchain-policy.ps1
$policy = Import-ToolchainPolicy -Path .\config\toolchain.psd1
```

Run the focused schema tests with:

```powershell
.\scripts\test-toolchain-policy.ps1
```

The helper validates schema version 1, required tool entries, exact/range field rules, parseable versions, and ordered inclusive range bounds. `scripts/check.ps1` runs these focused tests before configure, so the local and CI quality gates continuously cover the policy validator. Installed binary version checks are not part of this helper.
