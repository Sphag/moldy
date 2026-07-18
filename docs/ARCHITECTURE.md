# Architecture

This is a living document. It describes current and planned boundaries. Planned modules are not implementation claims, dependency approvals, or permission for broad architecture changes.

## Current Modules

- `core`: Platform-neutral foundations. The current public surface includes build information, explicit status/result values, logging, assertions, steady time, and application lifecycle primitives.
- `math`: Standalone, dependency-free math values. The current public surface includes HLSL-named float, signed, and
  unsigned vectors; square matrices; a shared color carrier; and explicit RGB/HSL/HSV/sRGB conversions.

## Planned Module Boundaries

- `math`: Project-owned quaternions, transforms, and geometry, extending the current vector, matrix, and color slice.
  Its binding conventions are defined in [Math Conventions](MATH_CONVENTIONS.md); it remains independent of rendering
  and third-party math types.
- `memory`: Allocator contracts, allocation tags, tracking, budgets, and leak reporting. Consumers depend on project-owned allocation contracts rather than a concrete allocator.
- `diagnostics`: Trace events, breadcrumbs, captures, stacks, crash reports, sanitizers, and shared GPU diagnostic records. Tracy and vendor GPU tools are optional adapters.
- `platform`: Desktop process, window, input, crash, and operating-system services. Win32 is the first planned host adapter; platform-neutral consumers do not expose Win32 types.
- `rhi`: Backend-neutral resources, commands, queues, synchronization, descriptors, pipelines, and diagnostic hooks.
- `rhi-d3d12`: D3D12 device, queues, swap chain, resources, descriptors, DRED, and related adapters. D3D12MA, DXC, Aftermath, and AMD GPU Detective remain isolated integrations.
- `render`: Static geometry, frame execution, render-graph compilation, transient resources, parallel command recording, and GPU statistics.
- `scene`: Project-owned entity, component, hierarchy, identity, transform, and runtime reconstruction contracts. EnTT remains behind an adapter.
- `serialization`: Versioned JSON scene source, deterministic binary cache, compilation, validation, and migration. nlohmann/json remains behind an adapter.
- `editor`: Viewport, hierarchy, inspector, undo/redo, diagnostics panels, and safe open/save workflows. Dear ImGui and ImGuizmo remain behind platform and renderer/editor adapters.
- `examples`: Small executable examples and sandboxes.
- `tools`: Developer, asset, symbol, and supporting command-line tools.

## Dependency Direction

- `math`, core allocation contracts, and base trace contracts are foundations for later tracks.
- `platform` and backend adapters may depend on platform-neutral contracts; platform-neutral modules must not depend on Win32 or D3D12 types.
- `rhi-d3d12` implements `rhi`; `render` consumes `rhi` and must not bypass it for ordinary rendering work.
- `scene` owns semantic data. `serialization` translates scene source and cache formats, while `editor` performs validated scene operations through scene contracts.
- Optional SDKs and libraries are contained at adapter boundaries. Their types, allocation rules, and failure behavior do not become public project contracts.
- A roadmap issue naming a dependency does not approve adding it. Dependency addition still requires explicit approval and a licensing/build review when its implementation task starts.

## Planned Data Flows

Rendered viewport:

```text
Win32 host/input -> editor -> render -> RHI -> D3D12 adapter -> GPU or WARP
                                  |                    |
                                  +---- diagnostics ---+
```

Persistent scene:

```text
JSON source -> validated adapter -> scene compiler -> binary cache -> runtime scene
     ^                                                        |
     +--------------- editor save/open workflow <-------------+
```

## Architecture Principles

- Keep module boundaries simple and data flow explicit.
- Keep public surfaces minimal and project-owned.
- Prevent platform, backend, vendor SDK, and third-party library types from leaking across adapter boundaries.
- Make ownership, lifetime, synchronization, failure, and versioning rules explicit.
- Record large architecture changes in `docs/DECISIONS.md` before or alongside implementation.
- Verify contracts with deterministic focused tests; use WARP where D3D12 hardware is not required.

## Deferred Decisions

- A future rendering backend is not selected. Issue #91 defines the evidence required for that decision.
- Dependency versions, acquisition, licensing, and enablement defaults are decided in their implementation issues.
- Audio, physics, scripting, asset-pipeline scope, and non-Windows platform implementations remain outside the four current tracks.
