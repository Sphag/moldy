# Math Module

`src/math` contains the standalone, dependency-free `math` library module.

## Public API

Import the module independently of `moldy.core`:

```cpp
import moldy.math;
```

The API follows HLSL type naming: `float2` through `float4`, `int2` through `int4`, `uint2` through `uint4`, and the
matching square matrices from `float2x2` through `uint4x4`. Matrix columns are public `c0` through `c3` values.

Vectors expose the position fields `xyzw`, color aliases `rgba`, and texture-coordinate aliases `uv` where the
component exists. These aliases share component storage. Compose or reorder components with the free
`swizzle<...>(vector)` function; there are no swizzle proxy objects.

Vector and matrix arithmetic is provided by operators and free functions: `dot(...)`, `cross(...)` for `float3`,
`length_squared(...)`, `length(...)`, and `normalize(...)`. `normalize(...)` returns an empty `std::optional` for an
exactly zero float vector. Scalar division requires a non-zero scalar. Signed integer arithmetic must not overflow;
unsigned arithmetic follows `uint32_t` modulo behavior.

`zero<T>`, vector `one<T>`, and matrix `identity<T>` provide generic compile-time constants. `color` is a linear RGBA
value type; named colors are in `math::colors`. `hsv` and `hsl` explicitly represent hue/saturation/value and
hue/saturation/lightness values. RGB↔HSV/HSL and sRGB transfer functions are free functions. RGB uses sRGB primaries,
and only `srgb_*` functions apply the sRGB transfer curve.

Quaternions, transforms, rectangular matrices, global comparison tolerances, rendering integration, and third-party
math libraries are outside this module's current scope.

Follow [Math Conventions](../../docs/MATH_CONVENTIONS.md) for the binding coordinate and precision policy.
