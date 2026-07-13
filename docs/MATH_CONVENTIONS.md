# Math Conventions And Precision Policy

This document is the binding, backend-neutral contract for project math APIs. The current math slice implements scalar
vectors, square matrices, and linear color values; quaternion, transform, and geometry APIs remain future work.

## Scope

The initial math slice is dependency-free and project-owned. It has no third-party math dependency and no rendering
backend dependency. `moldy.math` currently provides HLSL-named vectors, square matrices, and linear colors; future
implementations must follow this policy unless a later accepted decision explicitly supersedes it.

## Coordinates And Units

- World coordinates are right-handed: positive X points right and positive Y points up.
- A camera faces along negative Z in its local coordinate system.
- Linear quantities in public math APIs use meters.
- Angular quantities in public math APIs use radians.
- Degree conversions must be explicit in public names, parameters, or documentation. APIs must not silently accept
  degrees where radians are required, or the reverse.

## Matrices And Transforms

- Transform matrices are 4 by 4 and stored in column-major order.
- Matrices operate on column vectors: `result = M * vector`.
- Matrix multiplication composes right to left: `A * B` applies `B` first, then `A`.
- In an affine transform, translation occupies the fourth column. The affine translation column is
  `(translation.x, translation.y, translation.z, 1)`.

## Homogeneous Coordinates And Rotation

- A position uses a homogeneous `w` of `1`, so affine translation affects it.
- A direction uses a homogeneous `w` of `0`, so affine translation does not affect it.
- Positive rotations follow the right-hand rule: curl the fingers of the right hand in the positive rotation
  direction while the thumb points along the positive rotation axis.

## Scalar And Comparison Policy

- The initial public scalar type is `float`. Supported toolchains must provide a 32-bit `float` with IEC 60559
  floating-point behavior; `math_policy_tests` verifies these assumptions.
- Exact equality is reserved for structural or known-exact cases, such as checking an identity representation,
  comparing an explicitly stored zero, or comparing values whose exact construction guarantees equality.
- Numerical comparisons must use explicit absolute and relative tolerances appropriate to the operation and its
  documented input range. A comparison must not rely on an implicit project-wide epsilon.
- A global default epsilon is intentionally not defined. Each future operation that compares computed floating-point
  values must choose and document its tolerance policy.
- Current vectors, matrices, and colors use exact structural equality only. `normalize(...)` reports an exactly
  zero-length float vector with an empty result instead of applying a tolerance.
- Signed integer vector and matrix arithmetic has a no-overflow precondition. Unsigned vector and matrix arithmetic
  follows the modulo behavior of `uint32_t`.

## Colors

- `math::color` stores normalized linear RGBA values using sRGB primaries. It does not perform implicit color-space
  conversion.
- `rgb_to_hsv` returns `hsv` and `rgb_to_hsl` returns `hsl`; their inverse functions accept the corresponding explicit
  type and return linear RGB `float3`. Hue, saturation, lightness, and value components use the normalized range
  `[0, 1]`; hue wraps cyclically.
- `srgb_to_rgb` and `rgb_to_srgb` apply the standard sRGB transfer curve. The sRGB/HSV/HSL helper pairs explicitly
  decode or encode through linear RGB.

## Deferred Backend Rules

Clip-space handedness, depth range, shader matrix packing, transpose rules, and conversion rules for rendering
backends are deliberately outside this policy. They will be specified at RHI and backend-adapter boundaries, including
the future D3D12 work. Backend requirements must not change the public math conventions above.

## Related Documents

- [Architecture](ARCHITECTURE.md) describes the planned math boundary.
- [Decisions](DECISIONS.md) records acceptance of this policy.
- [Testing](TESTING.md) describes the focused toolchain-assumption and vector tests.
- [`src/math/README.md`](../src/math/README.md) documents the current public vector API.
