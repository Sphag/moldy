# Core Module

`src/core` contains the current platform-neutral `core` library module.

## Current Public API

The public C++ module is `moldy.core` and is imported as:

```cpp
import moldy.core;
```

Available error types:

- `core::EErrorCode`: small set of recoverable error categories used by current core APIs.
- `core::Status`: no-value success or error status with `ok()`, `code()`, `message()`, `Status::success()`, and `Status::error(...)`.
- `core::ResultValue`: concept for movable, non-void value types accepted by `core::Result<TValue>`.
- `core::Result<TValue>`: value-or-error result for operations that return a value, with `success(...)`, `failure(...)`, `ok()`, `value_if()`, and `error_if()`.

Available logging primitives:

- `core::ELogLevel`: stable severity values from `Trace` through `Critical`.
- `core::LogSourceLocation`: owning source-location data captured by logging and assertion macros.
- `core::LogRecord`: owning log record that preserves level, category, message, steady timestamp, and source-location data.
- `core::log_level_name(...)`: returns stable lowercase names for log levels.
- `core::ILogSink`: interface for custom synchronous log sinks.
- `core::ConsoleLogSink`: synchronous standard-output or standard-error sink.
- `core::FileLogSink`: synchronous file sink.
- `core::InMemoryLogSink`: test-oriented sink that stores records for inspection.
- `core::Logger`: synchronous logger with minimum-level filtering, mutex-protected sink management, and fan-out dispatch that invokes sink callbacks outside the logger mutex.
- `core::initialize_logging(...)`, `core::reset_logging()`, `core::current_logger()`, and `core::is_logging_initialized()`: explicit global logger registry lifecycle.
- `core::log_message(...)`: global logging entry point used by macros; it is a no-op success before logger initialization.
- `core::ScopedLoggingOverride`: scoped test override for the process-wide logger.

Available macro primitives:

- `MOLDY_LOG_TRACE(category, message)` through `MOLDY_LOG_CRITICAL(category, message)`: source-location-capturing logging macros from `<moldy/core_macros.hpp>`.
- `MOLDY_ASSERT(expr)`, `MOLDY_ASSERT_MSG(expr, message)`, and `MOLDY_ASSERT_FAIL(message)`: assertion macros from `<moldy/core_macros.hpp>`.

Assertions are enabled in `Debug` and `RelWithDebInfo` through `MOLDY_ENABLE_ASSERTS`; `Release` compiles assertion macros out. Failed active assertions report expression/message/source location, attempt to log through initialized core logging, then trap or abort.

Available time primitives:

- `core::SteadyTimePoint`: alias for `std::chrono::steady_clock::time_point`.
- `core::Duration`: alias for `std::chrono::nanoseconds`.
- `core::steady_now()`: returns the current steady-clock time point.
- `core::elapsed_since(...)`: returns elapsed steady-clock duration from a start point.

Available lifecycle primitives:

- `core::EApplicationState`: explicit application lifecycle states: `Created`, `Running`, `StopRequested`, and `Stopped`.
- `core::ApplicationLifecycle`: small state machine for valid start, stop-request, and stop transitions using `core::Status` for invalid transitions.

Available build-information functions:

- `core::build_configuration()`: reports the CMake build configuration, with a fallback for missing or empty configuration data.
- `core::compiler_id()`: reports the compiler identifier supplied by CMake, with a fallback for missing or empty compiler data.
- `core::language_standard()`: reports the compiler's `__cplusplus` value.

The module interface lives in `core.cppm`. The macro header lives at `include/moldy/core_macros.hpp`. Current implementation units are `build_info.cpp`, `lifecycle.cpp`, `logging.cpp`, `status.cpp`, and `time.cpp`.

Keep this README aligned with the public module interface and current module responsibility. Do not add renderer, platform layer, event loop, ECS, asset, editor, or OS-integration responsibilities here until corresponding source exists.
