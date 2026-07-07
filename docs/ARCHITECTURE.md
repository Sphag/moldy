# Architecture

This is a living document. It describes intended architecture boundaries and records the current shape of the system without adding project identity or project status details.

## Current Modules

- `core`: Platform-neutral foundations. Current public surface includes build-information helpers plus basic explicit error/result values through `core::Status` and `core::Result<TValue>`.

## Planned High-Level Modules

- `core`: Platform-neutral foundations such as errors, logging, time, and basic utilities.
- `platform`: Operating-system integration for windows, input, filesystems, timers, and related desktop services.
- `runtime`: Application lifecycle and runtime coordination.
- `examples`: Small executable examples and sandboxes.
- `tools`: Developer tools, asset tools, and supporting command-line utilities.

## Architecture Principles

- Keep module boundaries simple.
- Make data flow explicit.
- Keep public surface area minimal.
- Avoid unclear abstractions.
- Prevent platform-specific behavior from leaking into platform-neutral modules.
- Record large architecture changes in `docs/DECISIONS.md` before or alongside implementation.
- Prefer direct designs that can be explained and tested.

## Future Decisions

The following areas are intentionally undecided:

- Renderer
- Assets
- Scene model
- Audio
- Physics
- Scripting
