# Backlog

GitHub Issues are the canonical backlog for actionable work. This file is a lightweight local index for the current development session.

## PR-Scoped Completion

- PR [#8](https://github.com/Sphag/moldy/pull/8) merged Phase 2 issue cleanup for #1, #2, and #3.
- PR [#9](https://github.com/Sphag/moldy/pull/9) addresses Phase 3 core primitives for #4, #5, and #6.

## Diagnostics Runtime Issues

- [#10 Document core diagnostics runtime spec](https://github.com/Sphag/moldy/issues/10).
- [#11 Add core assertion macros and assertion-enabled RelWithDebInfo checks](https://github.com/Sphag/moldy/issues/11).
- [#12 Add global core logger registry and logging macros](https://github.com/Sphag/moldy/issues/12).
- [#13 Add composable core logging sinks](https://github.com/Sphag/moldy/issues/13).
- [#14 Add steady timestamps to core log records](https://github.com/Sphag/moldy/issues/14).
- [#15 Plan async logging queue](https://github.com/Sphag/moldy/issues/15).

## Follow-Ups Without Dedicated Issues

- Decide whether to keep plain executable tests or approve a test framework dependency.
- Evaluate `import std` later as a Ninja-only experiment until required generators support it cleanly.
- Decide any future reflection approach separately from the C++23 scaffold.
- Add more core APIs only when driven by concrete needs.
