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



## 2026-07-07: Select C++ Formatting And Static Analysis Tools

Date: 2026-07-07
Status: Accepted
Context: The repository needs enforced C++ formatting and static-analysis checks instead of placeholder scripts.
Decision: Use `clang-format` for formatting, `clang-tidy` for linting, and `cppcheck` for additional static analysis. Keep suppressions in `.cppcheck-suppressions` and require explicit local tool checks through `scripts/install-tools.ps1`.
Consequences: `scripts/check.ps1` now depends on the selected tools. Contributors must install LLVM tools and cppcheck locally before running the default quality gate.

## 2026-07-07: Use Unreal-Inspired Native C++ Style

Date: 2026-07-07
Status: Accepted
Context: The project needs a readable C++ style before more public APIs are added.
Decision: Use spaces, Allman braces, PascalCase type names, namespaces for organization, `T` for template type parameters, `I` for abstract interfaces, and `E` for enum types. Do not adopt Unreal reflection macros, property macros, type aliases, or `U`/`A`/`F` prefixes for ordinary repository classes and structs.
Consequences: Style is documented in `docs/CPP_STYLE.md` and enforced where possible by `clang-format` and `clang-tidy`.

## 2026-07-07: Publish Core As A C++ Module

Date: 2026-07-07
Status: Accepted
Context: The current `core` API is small enough to move to a project module before broader API compatibility concerns exist.
Decision: Replace the include-based `core/build_info.hpp` public API with the `moldy.core` C++ module while keeping the public namespace `core`, target names, and current functions unchanged.
Consequences: Consumers now use `import moldy.core;`. CMake minimum version is 3.28 to use the `CXX_MODULES` file set. The retired compatibility header is removed.

## 2026-07-07: Defer `import std` For Required Builds

Date: 2026-07-07
Status: Accepted
Context: CMake and generator support for standard library module imports is still uneven across required desktop workflows.
Decision: Do not use `import std` in required builds yet. Module units may include standard library headers in the global module fragment.
Consequences: `import std` remains a future Ninja-only experiment until the required build generators support it cleanly.
## 2026-07-07: Prefer Explicit Status And Result Returns For Recoverable Errors

Date: 2026-07-07
Status: Accepted
Context: Core APIs need a small, dependency-free way to report recoverable failures while keeping control flow explicit.
Decision: Prefer explicit `core::Status` and `core::Result<TValue>` returns for recoverable errors instead of exceptions as the default public API contract.
Consequences: Core APIs should reserve exceptions for truly exceptional or dependency-imposed cases, and tests should cover success, failure, and invariant-preserving behavior for status/result values.

## 2026-07-08: Keep Initial Runtime Primitives In Core Minimal And Dependency-Free

Date: 2026-07-08
Status: Accepted
Context: Phase 3 needs basic logging, time, and lifecycle primitives before broader engine systems exist.
Decision: Add only dependency-free core primitives: owning log records and severity names, steady-clock helpers, and an explicit application lifecycle state machine. Do not add global loggers, sinks, file output, formatting policy, async logging, wall-clock handling, timers, scheduling, platform abstractions, event loops, renderer, ECS, assets, editor, or OS integration in this slice.
Consequences: The core API gains small testable building blocks while broader runtime and platform responsibilities remain future decisions.
