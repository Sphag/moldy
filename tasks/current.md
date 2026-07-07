# Current Task

Current focus: C++ tooling, style, and the first project module are in place.

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

## Next Recommended Tasks

- Add CI once the local quality gate is stable across the target developer environments.
- Decide whether plain executable tests remain sufficient before adding any test framework dependency.
- Add more core APIs only when driven by concrete needs.
