# Core Diagnostics Runtime

This document defines the current core diagnostics runtime slice.

## Scope

Core diagnostics currently covers:

- Assertion macros for invariant checks.
- Synchronous logging through an explicitly initialized global logger.
- Composable logging sinks.
- Steady-clock timestamps on log records.
- Tests and documentation for the supported behavior.

The slice remains dependency-free and platform-neutral. It does not add renderer, platform layer, event loop, scheduler, asset, editor, OS integration, or broader engine runtime responsibilities.

## Build Configurations

Assertions are enabled when `MOLDY_ENABLE_ASSERTS` is defined. The CMake project defines it for these configurations:

- `Debug`
- `RelWithDebInfo`

`RelWithDebInfo` is the optimized-with-asserts configuration. Use it when checking behavior that should remain optimized while still validating invariants:

```powershell
.\scripts\check.ps1 -BuildDir build-check-relwithdebinfo -Configuration RelWithDebInfo
```

`Release` does not define `MOLDY_ENABLE_ASSERTS`, so assertion macros compile out.

## Assertion API

The public macro header is:

```cpp
#include <moldy/core_macros.hpp>
```

Callers must import `moldy.core` before using macros that call into the `core` namespace:

```cpp
import moldy.core;

#include <moldy/core_macros.hpp>
```

Public assertion macros:

- `MOLDY_ASSERT(expr)`
- `MOLDY_ASSERT_MSG(expr, message)`
- `MOLDY_ASSERT_FAIL(message)`

When assertions are enabled, failed assertions report the expression, optional message, and source location through `core::handle_assertion_failure(...)`. The handler attempts to log a critical record through the initialized global logger, then traps or aborts the process.

When assertions are disabled, all assertion macros compile out and do not evaluate their arguments.

Failing assertion behavior is not tested in-process by `core_tests` because a correct failure terminates the process. `core_tests` covers passing assertions and the compiled-out `MOLDY_ASSERT_FAIL(...)` path in non-assert builds.

## Logging API

The module keeps direct APIs for tests and non-macro callers:

- `core::Logger`
- `core::ILogSink`
- `core::ConsoleLogSink`
- `core::FileLogSink`
- `core::InMemoryLogSink`
- `core::initialize_logging(...)`
- `core::reset_logging()`
- `core::current_logger()`
- `core::is_logging_initialized()`
- `core::log_message(...)`
- `core::ScopedLoggingOverride`

The macro header provides:

- `MOLDY_LOG_TRACE(category, message)`
- `MOLDY_LOG_DEBUG(category, message)`
- `MOLDY_LOG_INFO(category, message)`
- `MOLDY_LOG_WARNING(category, message)`
- `MOLDY_LOG_ERROR(category, message)`
- `MOLDY_LOG_CRITICAL(category, message)`

Logging through `core::log_message(...)` or logging macros before initialization is a no-op success. This keeps early startup and tests safe before diagnostics are explicitly configured.

## Logger Lifecycle

The global logger registry is process-wide and explicit:

1. Create a `std::shared_ptr<core::Logger>`.
2. Configure its minimum level and sinks.
3. Call `core::initialize_logging(logger)`.
4. Log through direct APIs or macros.
5. Call `core::reset_logging()` during shutdown or test cleanup.

`core::ScopedLoggingOverride` is the test-oriented override API. It stores the current logger, installs a replacement for the scope, and restores the previous logger in its destructor.

## Sink Model

`core::Logger` owns no sinks directly. It stores shared pointers to `core::ILogSink` implementations and dispatches each accepted record to every configured sink.

Sink management and dispatch are mutex-protected. `core::InMemoryLogSink` and `core::FileLogSink` also protect their own mutable state.

Supported sinks:

- `core::ConsoleLogSink`: writes formatted records to standard output or standard error.
- `core::FileLogSink`: writes formatted records to a file path.
- `core::InMemoryLogSink`: stores records for tests and inspection.
- Custom sinks: implement `core::ILogSink::write(...)`.

`core::Logger::log(...)` continues dispatching when one sink fails. It returns the first sink failure status after all sinks have been attempted.

## Timestamp Policy

`core::LogRecord` stores a `core::SteadyTimePoint` captured from `core::steady_now()` by default. Callers may also pass an explicit steady timestamp for tests or controlled records.

Timestamps are monotonic and runtime-oriented only. This slice intentionally does not provide wall-clock time, timezone handling, calendar formatting, or log timestamp text formatting.

## Non-Goals

The following work is intentionally out of scope for this slice:

- Async queueing or background logging.
- Flush or shutdown worker semantics.
- Backpressure policies.
- Platform-specific logging integration.
- Structured log serialization formats.
- Wall-clock timestamps.
- Renderer, platform layer, event loop, asset, editor, ECS, or broader engine systems.
