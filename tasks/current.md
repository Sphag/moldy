# Current Task

Current focus: GitHub CI feedback loop setup.

## GitHub CI Feedback Loop Setup

- [x] Re-check current Git status before editing.
- [x] Add a GitHub workflow feedback-loop document.
- [x] Link the feedback-loop document from AI workflow docs.
- [x] Update source-map documentation for the new workflow document.
- [x] Check whether GitHub CLI is available locally.
- [x] Install GitHub CLI locally.
- [ ] Authenticate GitHub CLI with repository and workflow access.

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

## Next Recommended Tasks

- Authenticate GitHub CLI locally with repository and workflow access.
- Create GitHub Issues for current and backlog items.
- Push a branch and verify the first GitHub Actions run.
- Continue core APIs after the GitHub workflow slice is verified remotely.
- Decide whether plain executable tests remain sufficient before adding any test framework dependency.
