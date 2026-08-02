# Math Module

`src/math` contains the project-owned, third-party-free `math` library module. Its public API is platform-neutral and
does not expose `moldy.core` types.

## Public API

Import the module without importing `moldy.core`:

```cpp
import moldy.math;
```

The API follows HLSL type naming: `float2` through `float4`, `int2` through `int4`, `uint2` through `uint4`, and the
matching square matrices from `float2x2` through `uint4x4`. Matrix columns are public `c0` through `c3` values.
`quaternion` is a strong type that stores its vector part in `x/y/z` and scalar part in `w`.

Vectors expose the position fields `xyzw`, color aliases `rgba`, and texture-coordinate aliases `uv` where the
component exists. These aliases share component storage. Compose or reorder components with the free
`swizzle<...>(vector)` function; there are no swizzle proxy objects.

Vector and matrix arithmetic is provided by operators and free functions: `dot(...)`, `cross(...)` for `float3`,
`length_squared(...)`, `length(...)`, and `normalize(...)`. `normalize(...)` returns the normalized value directly;
zero length is an assertion-backed precondition violation. Scalar division likewise requires a non-zero scalar.
Signed integer arithmetic must not overflow; unsigned arithmetic follows `uint32_t` modulo behavior.

Through its vector traits, `quaternion` reuses vector equality, arithmetic, swizzles, `dot(...)`, `length_squared(...)`,
`length(...)`, and `normalize(...)` without becoming interchangeable with `float4`. `hamilton_product(A, B)` applies
`B` first and then `A`, matching matrix composition. `conjugate(...)`, `inverse(...)`, and `rotate(...)` provide the
remaining basic rotation operations. `quaternion_from_axis_angle(...)` accepts radians and a non-zero axis. Positive
rotations follow the right-hand rule.

`quaternion_to_float3x3(...)` accepts any non-zero quaternion and normalizes it before conversion.
`float3x3_to_quaternion(...)` requires explicit absolute and relative tolerances and accepts only a proper
orthonormal rotation matrix. The returned quaternion is normalized and uses a deterministic sign: `w` is positive
when non-zero; at exactly zero `w`, the first non-zero component in `x/y/z` is positive.

`zero<T>`, vector `one<T>`, and matrix or quaternion `identity<T>` provide generic compile-time constants. `color` is the
project-owned carrier for every color representation: RGB and sRGB use `r/g/b`, HSV uses `r=h, g=s, b=v`, and HSL uses
`r=h, g=s, b=l`. Components are normalized, hue wraps cyclically, and every conversion preserves alpha unchanged.
Conversions are explicit free functions that take and return `color`. Named colors live in `math::colors`; their RGB
channels use sRGB primaries, and only `srgb_*` functions apply the sRGB transfer curve.

Transforms, geometry, rectangular matrices, interpolation APIs, global comparison tolerances, rendering integration,
and third-party math libraries are outside this module's current scope.

## Assertion Backend

Math preconditions use the private compile-time contract `MOLDY_MATH_ASSERT(expression, message)`. Select its backend
when configuring the library with `MOLDY_MATH_ASSERT_BACKEND`:

- `core` is the repository default. It maps to `MOLDY_ASSERT_MSG` and privately imports and links `moldy.core`.
- `standalone` writes the precondition message and uses `std::abort` in Debug and RelWithDebInfo; it does not include,
  import, or link `moldy.core`. Its Release checks compile out.
- `custom` includes the header named by the required `MOLDY_MATH_ASSERT_HEADER` setting. That header must define
  `MOLDY_MATH_ASSERT(expression, message)` with two arguments.

A custom header may be header-only or call symbols supplied by a target that the embedding build links to `math`.
No core types or runtime callback interface are imposed. The selected macro, custom header, and core types are not
published to consumers. The backend is fixed when the `math` library and its module interface are compiled; a
consumer cannot select a different backend for an already compiled module target.

Standalone example:

```powershell
cmake -S . -B build-math-standalone -DMOLDY_MATH_ASSERT_BACKEND=standalone
cmake --build build-math-standalone --target math
```

Custom example:

```powershell
cmake -S . -B build-math-custom `
    -DMOLDY_MATH_ASSERT_BACKEND=custom `
    -DMOLDY_MATH_ASSERT_HEADER=C:/path/to/math_assert.hpp
```

Follow [Math Conventions](../../docs/MATH_CONVENTIONS.md) for the binding coordinate and precision policy.
