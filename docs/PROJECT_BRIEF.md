# Project Brief

This brief records the current identity and scope for Moldy without claiming engine systems that do not exist yet.

## Project Name

Moldy.

## Current State

Moldy is currently a C++23/CMake scaffold with a small `core` library, a smoke executable, plain executable tests, and PowerShell workflow scripts.

## High-Level Goals

- Keep a portable desktop-first C++ foundation.
- Build core infrastructure before engine systems.
- Keep public APIs understandable and intentionally scoped.
- Record major technical decisions before or alongside implementation.

## Audience

- Contributors working on the repository.
- AI agents operating under `AGENTS.md`.
- Future users who need to understand the project state from source and docs.

## Non-Goals

- Do not claim completed renderer, editor, asset pipeline, scripting, physics, networking, or platform-layer systems before they exist.
- Do not choose 2D or 3D direction in this brief.
- Do not add dependencies without an explicit decision.
- Do not broaden the current scaffold into speculative architecture.

## License

MIT License.

## Target Platforms

- Windows desktop
- macOS desktop
- Linux desktop

## Open Decisions

- 2D vs 3D direction
- Future rendering backend criteria after the first D3D12 viewport
- Dependency versions, licensing, and acquisition for planned adapters
- Non-Windows platform implementation details
- Physics approach
- Audio approach
- Asset pipeline
- Scripting approach
- Networking scope

