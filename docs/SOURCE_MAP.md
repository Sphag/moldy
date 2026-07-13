# Source Map

This document maps the current repository scaffold. Keep it factual and update it when source layout, build targets, scripts, tests, or public APIs change.

## Repository Layout

- `CMakeLists.txt`: top-level CMake project, target definitions, compiler settings, module file set, and test registration.
- `src/core/`: platform-neutral `core` library module source.
- `apps/smoke/`: small executable used to verify that the core library links and reports build information.
- `tests/core/`: plain C++ executable tests for the current core API.
- `scripts/`: PowerShell entry points for configure, build, test, check, format, lint, tool setup, and benchmark placeholder flows.
- `.github/pull_request_template.md`: compact pull request description template for agents and humans.
- `.github/workflows/`: GitHub Actions workflow definitions for pushed branch and pull request validation.
- `docs/`: project workflow, AI tooling status and adoption, GitHub workflow, quality, style, testing, decisions, architecture, core diagnostics, and source navigation documents.
- `tasks/`: task planning notes.
- `build*/`: generated build trees. These are not source locations.

## GitHub Actions

- `.github/workflows/ci.yml`: runs the PowerShell quality gate on `windows-latest` for pushed branches and pull request updates.
- `docs/GITHUB_WORKFLOW.md`: documents the local pre-push check, remote CI feedback loop, and commands for investigating failing GitHub Actions runs.

The first workflow intentionally mirrors the local `scripts/check.ps1` gate. macOS and Linux jobs are deferred until runner toolchain feasibility is verified.

## Workflow Documents

- `AGENTS.md`: authoritative repository rules for AI agents.
- `docs/AI_WORKFLOW.md`: cautious working loop, GitHub collaboration, documentation maintenance, and capability-aware routing.
- `docs/AI_TOOLING.md`: status model, current capability inventory, implementation blueprints, MCP admission policy, security model, and roadmap mapping.
- `docs/GITHUB_WORKFLOW.md`: local pre-push checks and the remote pull request and CI feedback loop.

## CMake Project

The top-level CMake project is named `moldy` and requires CMake 3.28 or newer.

## Core Library

The `core` static library is defined from:

- `src/core/core.cppm`: public C++ module interface for `moldy.core`.
- `src/core/include/moldy/core_macros.hpp`: public logging and assertion macro header.
- `src/core/build_info.cpp`: module implementation unit for build-information functions.
- `src/core/lifecycle.cpp`: module implementation unit for `core::ApplicationLifecycle` behavior.
- `src/core/logging.cpp`: module implementation unit for logging primitives, sink dispatch, global logger registry, and assertion failure handling.
- `src/core/status.cpp`: module implementation unit for `core::Status` behavior.
- `src/core/time.cpp`: module implementation unit for steady-clock helpers.

Current public API is imported with:

```cpp
import moldy.core;
```

Current public error API:

- `core::EErrorCode`: error categories for current recoverable core failures.
- `core::Status`: no-value success/error return type.
- `core::ResultValue`: concept for movable, non-void value types accepted by `core::Result<TValue>`.
- `core::Result<TValue>`: value-or-error return type for value-producing operations.

Current public logging API:

- `core::ELogLevel`: stable log severity enum.
- `core::LogSourceLocation`: owning source-location metadata captured by macros.
- `core::LogRecord`: owning log record with level, category, message, steady timestamp, and source-location accessors.
- `core::log_level_name(...)`: returns stable lowercase severity names.
- `core::ILogSink`: synchronous sink interface for built-in and custom sinks.
- `core::ConsoleLogSink`: standard output or standard error sink.
- `core::FileLogSink`: file sink with open-status and write-status reporting.
- `core::InMemoryLogSink`: unbounded record storage, alongside bounded, ring-buffer, and external-buffer sink variants.
- `core::Logger`: synchronous logger with minimum-level filtering, mutex-protected sink management, and fan-out dispatch.
- `core::initialize_logging(...)`, `core::reset_logging()`, `core::current_logger()`, and `core::is_logging_initialized()`: explicit global logger registry lifecycle.
- `core::log_message(...)`: global logging entry point that reports missing initialization as an error.
- `core::ScopedLoggingOverride`: scoped test override for the global logger.
- `MOLDY_LOG_TRACE(...)`, `MOLDY_LOG_DEBUG(...)`, `MOLDY_LOG_INFO(...)`, `MOLDY_LOG_WARNING(...)`, `MOLDY_LOG_ERROR(...)`, and `MOLDY_LOG_CRITICAL(...)`: source-location-capturing logging macros from `<moldy/core_macros.hpp>`.
- `MOLDY_ASSERT(...)`, `MOLDY_ASSERT_MSG(...)`, and `MOLDY_ASSERT_FAIL(...)`: assertion macros from `<moldy/core_macros.hpp>`.

Current public time API:

- `core::SteadyTimePoint`: steady-clock time-point alias.
- `core::Duration`: nanosecond duration alias.
- `core::steady_now()`: returns the current steady-clock time point.
- `core::elapsed_since(...)`: returns elapsed steady-clock duration from a start point.

Current public lifecycle API:

- `core::EApplicationState`: `Created`, `Running`, `StopRequested`, and `Stopped` lifecycle states.
- `core::ApplicationLifecycle`: dependency-free lifecycle state machine with explicit status returns for invalid transitions.

Current public build-information functions:

- `core::build_configuration()`: returns the CMake build configuration string, or a fallback when unavailable.
- `core::compiler_id()`: returns the CMake compiler identifier, or a fallback when unavailable.
- `core::language_standard()`: returns the `__cplusplus` value observed by the compiler.

The current API is intentionally limited to build information, error/result values, and minimal logging, time, and lifecycle primitives. Do not document broader engine or runtime responsibilities until they exist in source.

## Applications

The `smoke` executable is defined from `apps/smoke/main.cpp`. It links `project::core`, imports `moldy.core`, and prints the current compiler, build configuration, and C++ language standard.

## Tests

The `core_tests` executable is defined from `tests/core/test_main.cpp`. It links `project::core`, imports `moldy.core`, uses a tiny local assertion harness, and is registered with CTest as `core_tests`.

Current checks verify that:

- The build configuration string is available.
- The compiler identifier string is available.
- The reported language standard is at least C++20.
- `core::Status::success()` reports success with an empty message.
- Error statuses preserve code and message.
- `core::Result<TValue>::success(...)` exposes a value and no error.
- `core::Result<TValue>::failure(...)` exposes an error and no value.
- `core::Result<TValue>::failure(core::Status::success())` normalizes to an internal error.
- `core::Result<core::Status>` can store a status payload.
- Log levels return stable lowercase names.
- `core::LogRecord` preserves level, category, message, steady timestamp, and source location, including empty strings.
- `core::Logger` records to in-memory sinks, filters by minimum level, fans out to multiple sinks, and continues dispatching after sink failures while returning the first failure.
- Global logging is safe before initialization, macros reach initialized sinks, and scoped overrides restore previous logger state.
- `core::FileLogSink` writes formatted records to a file path.
- Passing assertion macros do not terminate, and disabled assertion macros compile out where feasible.
- Steady-clock helpers return non-negative elapsed durations for captured and synthetic older starts.
- `core::ApplicationLifecycle` accepts valid state transitions and rejects invalid transitions without mutating state.

## Scripts

- `scripts/configure.ps1`: configures the CMake build directory and handles single-config versus multi-config generators.
- `scripts/build.ps1`: configures, then builds the project for the requested configuration.
- `scripts/test.ps1`: configures, builds `core_tests`, then runs CTest with failure output enabled.
- `scripts/check.ps1`: validates the toolchain policy, then runs configure, format, and lint once before building and testing Debug and Release by default.
- `scripts/format.ps1`: checks C++ formatting with `clang-format`; pass `-Fix` to apply formatting.
- `scripts/lint.ps1`: runs `clang-tidy` and `cppcheck` over source files, excluding generated build trees.
- `scripts/install-tools.ps1`: checks required tool availability and health, rejects broken tool binaries, and provides an explicit Windows install path.
- `scripts/bench.ps1`: reports placeholder-pass status because benchmarks are not configured yet.

## CMake Targets

- `core`: static library built as C++23 with a public `CXX_MODULES` file set.
- `project::core`: alias target for consumers of `core`.
- `smoke`: executable linked against `project::core`.
- `core_tests`: executable linked against `project::core` and registered as the current CTest test.

All project targets go through `configure_project_target`, which sets C++23 and disables compiler extensions. MSVC builds use `/W4 /permissive- /Zc:__cplusplus`; other compilers use `-Wall -Wextra -Wpedantic`.

The `core` target receives these private compile definitions from CMake:

- `CORE_BUILD_CONFIGURATION`: generated from `$<CONFIG>`.
- `CORE_COMPILER_ID`: generated from `CMAKE_CXX_COMPILER_ID`.

The `core` target also publishes `MOLDY_ENABLE_ASSERTS` for `Debug` and `RelWithDebInfo`. `RelWithDebInfo` is the optimized-with-asserts configuration used for focused diagnostics checks.

Runtime outputs are directed to `${CMAKE_BINARY_DIR}/bin`. Multi-config generators place configuration-specific executables under paths such as `build/bin/Debug/` or `build/bin/Release/`.
