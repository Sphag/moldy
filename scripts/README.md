# Scripts

PowerShell scripts in this directory are the repository's current local workflow entry points.

- `configure.ps1`: configures the CMake build directory.
- `build.ps1`: configures, then builds the requested configuration.
- `test.ps1`: configures, builds `core_tests` and `math_policy_tests`, then runs CTest.
- `check.ps1`: validates the toolchain policy, then runs configure, format, and lint once before building and testing `Debug` and `Release` by default. Pass `-Configuration` with one or more values to select a different set.
- `format.ps1`: checks C++ formatting with `clang-format`; pass `-Fix` to apply formatting.
- `lint.ps1`: runs `clang-tidy` and `cppcheck`, excluding generated build trees. Generators with `compile_commands.json` get full clang-tidy coverage; Visual Studio-style generators use module-interface fallback coverage.
- `install-tools.ps1`: checks selected local C++ quality tools, rejects unhealthy tool binaries, and provides an explicit Windows install path.
- `toolchain-policy.ps1`: provides the side-effect-free `Import-ToolchainPolicy` validation helper for `config/toolchain.psd1`.
- `test-toolchain-policy.ps1`: runs dependency-free focused tests for the toolchain policy schema and constraints.
- `test-agent-skills.ps1`: validates the tracked foundational Agent Skills against the open-format metadata and repository safety contract, then runs dependency-free smoke cases.
- `test-ai-workflow.ps1`: dependency-free focused tests for the deterministic AI workflow support scripts.
- `doctor.ps1`: reports local repository, policy, command, and tool-health readiness without installing or modifying anything.
- `context.ps1`: emits bounded, redacted tracked-file context for explicit repository-relative paths.
- `check-changed.ps1`: selects a proven focused check for documentation-only or Agent-Skills-only changes, otherwise runs the full gate.
- `collect-diagnostics.ps1`: writes a reviewable, redacted local diagnostics artifact to an explicit ignored directory.
- `workflow-common.ps1`: internal shared validation, reporting, redaction, and exit-code helper for the AI workflow scripts.
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

## Agent Skills Validation

Validate the repository-owned foundational skills without installing a validator package:

```powershell
.\scripts\test-agent-skills.ps1
```

The script checks the approved skill-directory set, open-format `SKILL.md` frontmatter, matching names, required safety-contract sections, explicit denied-approval stop conditions, and failure reporting. Its built-in smoke cases cover a valid skill, a missing prerequisite section, a denied approval boundary, and missing failure reporting. `scripts/check.ps1` runs it before tool probing and CMake work.

## Deterministic AI Workflow Support

`doctor.ps1`, `context.ps1`, `check-changed.ps1`, and `collect-diagnostics.ps1` are dependency-free local helpers. They accept `-Format Text|Json` (`Text` by default) and a test-only `-RepositoryRoot` override. JSON is UTF-8 with stable top-level `schemaVersion`, `title`, `status`, `exitCode`, and `data` fields. Exit `0` means success, `1` means completed with failed or degraded checks, `2` means invalid or unsafe input, and `3` means a missing prerequisite.

```powershell
.\scripts\doctor.ps1 -Format Json
.\scripts\context.ps1 -Path docs\AI_WORKFLOW.md,scripts\README.md -Format Json
.\scripts\check-changed.ps1 -BaseRef origin/main
.\scripts\collect-diagnostics.ps1 -OutputDirectory temp\diagnostics -LogPath build\failure.log
```

`context.ps1` accepts only tracked, non-ignored, UTF-8 files within the repository. It refuses `.git` and generated build trees, redacts common credential forms, and limits output to 100 files and 64 KiB of content. `check-changed.ps1` includes committed, staged, and unstaged changes. With no base reference, untracked changes, or uncertain scope, it explicitly runs `check.ps1`; documentation-only changes use whitespace checks and Agent-Skills-only changes run `test-agent-skills.ps1`. Child checks do not call `check-changed.ps1`.

`collect-diagnostics.ps1` creates files only in an explicit, empty, Git-ignored, repository-contained output directory. Its `manifest.json` records Git state, policy and command evidence, environment identity without environment-variable values, copied redacted logs, and collection failures. It never installs tools, runs arbitrary commands, changes Git history, or writes to the network. `scripts/check.ps1` runs `test-ai-workflow.ps1` before native tool probing.
