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

Available build-information functions:

- `core::build_configuration()`: reports the CMake build configuration, with a fallback for missing or empty configuration data.
- `core::compiler_id()`: reports the compiler identifier supplied by CMake, with a fallback for missing or empty compiler data.
- `core::language_standard()`: reports the compiler's `__cplusplus` value.

The module interface lives in `core.cppm`. Current implementation units are `build_info.cpp` and `status.cpp`.

Keep this README aligned with the public module interface and current module responsibility. Do not add future subsystem responsibilities here until corresponding source exists.
