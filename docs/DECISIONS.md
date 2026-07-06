# Decisions

This file records lightweight architecture and workflow decisions.

## Decision Template

```text
Date:
Status:
Context:
Decision:
Consequences:
```

## 2026-07-07: Use C++ and CMake

Date: 2026-07-07
Status: Accepted
Context: The repository is being prepared for a cross-platform desktop game engine.
Decision: Use C++ as the implementation language and CMake as the build system.
Consequences: Future scaffold, tooling, compiler settings, and examples should align with C++ and CMake.

## 2026-07-07: Use English Repository Documentation

Date: 2026-07-07
Status: Accepted
Context: AI agents and future contributors need consistent repository artifacts.
Decision: Repository documentation, code identifiers, filenames, and public technical artifacts are written in English.
Consequences: User-facing conversation may use another language, but repository artifacts remain English.

## 2026-07-07: Target Desktop Platforms

Date: 2026-07-07
Status: Accepted
Context: The project is scoped to desktop platforms.
Decision: Target Windows, macOS, and Linux desktop.
Consequences: Platform-layer design must account for all three target platform families.

## 2026-07-07: Use Cautious Agent Mode

Date: 2026-07-07
Status: Accepted
Context: The repository is early-stage and major direction choices are not settled.
Decision: Agents must ask before dependencies, architecture pivots, broad refactors, destructive actions, or git history changes.
Consequences: Work should proceed in small, explicit slices with clear reports.

## 2026-07-07: Ask Before Adding Dependencies

Date: 2026-07-07
Status: Accepted
Context: Dependency policy affects portability, build complexity, licensing, and long-term maintenance.
Decision: Agents must ask before adding dependencies.
Consequences: Tooling and library choices remain explicit decisions rather than incidental additions.

## 2026-07-07: Maintain Strict Code Quality

Date: 2026-07-07
Status: Accepted
Context: Early code patterns tend to become long-term defaults.
Decision: Require consistent formatting, clean architecture, understandable APIs, and confident design decisions.
Consequences: Placeholder checks may fail once source files exist until formatter, linter, and testing decisions are recorded.

## 2026-07-07: Use C++23 Baseline

Date: 2026-07-07
Status: Accepted
Context: The initial build scaffold needs a clear language baseline without requiring newer experimental features.
Decision: Build project targets as C++23 through CMake target properties.
Consequences: The first scaffold avoids C++ modules and reflection requirements. C++ modules can be reconsidered later, and reflection requires a separate future decision because it is not part of C++23.

## 2026-07-07: Use Plain Executable Tests

Date: 2026-07-07
Status: Accepted
Context: The project needs a portable test foundation without adding third-party dependencies.
Decision: Tests are normal C++ executables with a tiny local assertion harness. CTest is only the current runner integration layer.
Consequences: Test code does not depend on GTest, Catch2, doctest, or CTest APIs. A third-party test framework remains deferred until explicitly approved.

## 2026-07-07: Defer Formatter and Linter Selection

Date: 2026-07-07
Status: Accepted
Context: Formatter and static-analysis tool choices affect workflow, editor setup, CI, and cross-platform availability.
Decision: Do not add formatter or linter dependencies yet. The current format and lint scripts must report explicit placeholder-pass status and must not claim that formatting or linting was performed.
Consequences: `scripts/check.ps1` can report the full current gate, but formatter and linter enforcement remains future work.

## 2026-07-07: Name the Project Moldy

Date: 2026-07-07
Status: Accepted
Context: The initial scaffold needed an explicit project identity before the first repository commit.
Decision: Use `Moldy` as the project name and `moldy` as the CMake project name.
Consequences: Public documentation and top-level build metadata should use this identity. Target names remain `core`, `project::core`, `smoke`, and `core_tests`.

## 2026-07-07: Use Honest Product-Facing Documentation

Date: 2026-07-07
Status: Accepted
Context: The README should present the project clearly while the repository is still an early scaffold.
Decision: Describe Moldy by name and current technical state, but avoid claims about renderer, editor, asset pipeline, platform layer, 2D, 3D, or completed engine systems until they exist.
Consequences: Documentation can be product-facing without implying unimplemented capabilities.

## 2026-07-07: Use MIT License

Date: 2026-07-07
Status: Accepted
Context: The repository needs a simple permissive open-source license for its initial commit.
Decision: License Moldy under the MIT License.
Consequences: The repository includes a top-level `LICENSE` file, and public documentation should identify MIT as the project license.


