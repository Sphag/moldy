# Current Task

Current focus: project identity, README, MIT licensing, and Git initialization are complete.

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
- Updated quality scripts so formatter and linter gaps are explicit.

## Next Recommended Tasks

- Choose formatter and linter/static-analysis tools.
- Define the initial C++ style guide.
- Decide whether plain executable tests remain sufficient before adding any test framework dependency.
- Revisit C++ modules after the basic scaffold is stable on target compilers.

