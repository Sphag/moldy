# Current Task

Current focus: Phase 2 closure and Phase 3 core API readiness.

## Phase 2 Closure

- [x] Re-check current Git status before editing.
- [x] Confirm the local repository remote points to `Sphag/moldy`.
- [x] Confirm GitHub CLI is installed locally.
- [x] Attempt to refresh GitHub CLI authentication with repository and workflow scopes.
- [x] Verify the latest remote Windows GitHub Actions run for `main`.
- [x] Resolve the apparent writable GitHub authentication path blocker by approved-context verification.
- [x] Document stable GitHub CLI authentication handling for restricted agent sessions.
- [x] Create GitHub Issues for Phase 2 closure items and Phase 3 follow-ups.
- [x] Update local task files with created issue numbers or URLs after issues exist.

Notes:

- Restricted agent sessions may report invalid GitHub CLI authentication even when the keyring token works in a normal terminal or approved command context.
- Approved-context `gh auth status` verified `github.com` account `Sphag` with `repo` and `workflow`; do not re-login unless a normal terminal or approved command context lacks those scopes.
- GitHub Issues were created through approved `gh issue create` commands.
- The GitHub connector can read `Sphag/moldy`, but issue creation previously returned `403 Resource not accessible by integration`.
- The latest public Actions run on `main` succeeded for commit `c520e16`: https://github.com/Sphag/moldy/actions/runs/28897748834

Created issues:

- [#1 Remote Windows CI loop verification](https://github.com/Sphag/moldy/issues/1)
- [#2 GitHub Issues migration and local task-file slimming](https://github.com/Sphag/moldy/issues/2)
- [#3 macOS and Linux CI runner feasibility](https://github.com/Sphag/moldy/issues/3)
- [#4 Core logging primitives](https://github.com/Sphag/moldy/issues/4)
- [#5 Core time utilities](https://github.com/Sphag/moldy/issues/5)
- [#6 Application lifecycle primitives](https://github.com/Sphag/moldy/issues/6)

## GitHub CI Cppcheck Runner Fix

- [x] Inspect the failing GitHub Actions run for the latest commit.
- [x] Identify the failing workflow job and log root cause.
- [x] Update Windows tool bootstrap to reject unhealthy tool binaries.
- [x] Update documentation for the changed tool bootstrap behavior.
- [x] Run the local quality gate.
- [x] Publish the fix after explicit git-history approval.
- [x] Re-check the GitHub Actions run after publishing.

## GitHub CI Feedback Loop Setup

- [x] Re-check current Git status before editing.
- [x] Add a GitHub workflow feedback-loop document.
- [x] Link the feedback-loop document from AI workflow docs.
- [x] Update source-map documentation for the new workflow document.
- [x] Check whether GitHub CLI is available locally.
- [x] Install GitHub CLI locally.
- [x] Verify GitHub CLI authentication with repository and workflow access in an approved command context.

## README Onboarding Slice

- [x] Re-check current Git status before editing.
- [x] Clarify the current scaffold state in `README.md`.
- [x] Add first-run prerequisites and tool-check guidance.
- [x] Document the fresh-checkout quality gate path.
- [x] Document individual configure, build, and test commands.
- [x] Explain generated build directories and normal cleanup expectations.
- [x] Keep README links focused on the most relevant docs.
- [x] Update backlog tracking for the completed README onboarding item.

## Iteration 2: Core Error Handling Foundation

- [x] Re-check current Git status and diff before editing.
- [x] Add the core status/result API.
- [x] Constrain the current result template API with C++ concepts.
- [x] Add focused core tests.
- [x] Update build configuration.
- [x] Update documentation and backlog.
- [x] Run format and lint checks successfully.
- [x] Re-run build and tests outside the restricted sandbox after confirming the generated-file access blocker.

## Iteration 1: Quality Gate Stabilization

- [x] Add explicit repository line ending policy.
- [x] Restore the format gate to a clean state.
- [x] Verify selected tooling availability, including `cppcheck`.
- [x] Run the full local quality gate.
- [x] Keep documentation and task tracking synchronized.

## Completed

- Named the project Moldy.
- Added the MIT License.
- Updated README and project documentation for the current identity and scaffold state.
- Renamed the top-level CMake project to `moldy` while preserving existing target names.
- Added `.gitignore` for generated build artifacts and local noise.
- Initialized Git on `main` and created the initial commit.
- Added a minimal C++23 CMake scaffold.
- Added the `core` static library with a tiny build-info API.
- Added the `smoke` executable linked against `core`.
- Added a plain C++ `core_tests` executable and registered it with CTest.
- Selected `clang-format`, `clang-tidy`, and `cppcheck` as the C++ quality toolchain.
- Added formatter, linter, cppcheck suppression, and tool setup configuration.
- Defined the initial C++ style guide.
- Converted the current `core` public API from a header to the `moldy.core` C++ module.
- Updated quality scripts so format and lint checks are enforced.
- Added explicit repository LF line ending policy.
- Added `cppcheck` to user PATH after verifying the installed binary location.
- Adjusted the module interface export block syntax so `cppcheck` can parse the file without changing the exported API.
- Verified the full local quality gate with `scripts/check.ps1 -BuildDir build-check`.
- Added the initial Windows GitHub Actions CI workflow for pushed branches and pull requests.
- Documented the planned GitHub Issues and PR-first workflow.
- Improved README onboarding for first-time readers without overstating implemented features.
- Added the GitHub CI feedback-loop guide for AI-assisted CI fixes.
- Verified the latest remote Windows GitHub Actions run on `main` succeeded.
- Documented stable GitHub CLI auth handling for restricted agent sessions.
- Created GitHub Issues for Phase 2 closure items and Phase 3 follow-ups.

## Next Recommended Tasks

- Use [#2](https://github.com/Sphag/moldy/issues/2) to slim local task files now that GitHub Issues exist.
- Continue core APIs through [#4](https://github.com/Sphag/moldy/issues/4), [#5](https://github.com/Sphag/moldy/issues/5), and [#6](https://github.com/Sphag/moldy/issues/6).
- Decide whether plain executable tests remain sufficient before adding any test framework dependency.
