# Core Module

`src/core` contains the current platform-neutral `core` library module.

## Current Public API

The public C++ module is `moldy.core` and is imported as:

```cpp
import moldy.core;
```

Available functions:

- `core::build_configuration()`: reports the CMake build configuration, with a fallback for missing or empty configuration data.
- `core::compiler_id()`: reports the compiler identifier supplied by CMake, with a fallback for missing or empty compiler data.
- `core::language_standard()`: reports the compiler's `__cplusplus` value.

The module interface lives in `core.cppm`. The implementation unit is `build_info.cpp`.

Keep this README aligned with the public module interface and current module responsibility. Do not add future subsystem responsibilities here until corresponding source exists.
