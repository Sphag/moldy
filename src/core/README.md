# Core Module

`src/core` contains the current platform-neutral `core` library.

## Current Public API

The public header is `build_info.hpp` and is included as:

```cpp
#include "core/build_info.hpp"
```

Available functions:

- `core::build_configuration()`: reports the CMake build configuration, with a fallback for missing or empty configuration data.
- `core::compiler_id()`: reports the compiler identifier supplied by CMake, with a fallback for missing or empty compiler data.
- `core::language_standard()`: reports the compiler's `__cplusplus` value.

Keep this README aligned with public headers and current module responsibility. Do not add future subsystem responsibilities here until corresponding source exists.
