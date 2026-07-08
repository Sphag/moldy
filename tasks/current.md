# Current Task

Current focus: Phase 3 core primitives for logging, time, and application lifecycle.

## Phase 3 Core Runtime Primitives

- [x] Add minimal dependency-free logging primitives for [#4](https://github.com/Sphag/moldy/issues/4).
- [x] Add steady-clock time helpers for [#5](https://github.com/Sphag/moldy/issues/5).
- [x] Add application lifecycle state primitives for [#6](https://github.com/Sphag/moldy/issues/6).
- [x] Update focused core tests for logging, time, and lifecycle behavior.
- [x] Update core API documentation and architecture notes.
- [x] Run the local quality gate with `scripts/check.ps1 -BuildDir build-check`.
- [x] Prepare PR `core/phase3-primitives` to close #4, #5, and #6.

## Active Issue Links

- [#1 Remote Windows CI loop verification](https://github.com/Sphag/moldy/issues/1)
- [#2 GitHub Issues migration and local task-file slimming](https://github.com/Sphag/moldy/issues/2)
- [#3 macOS and Linux CI runner feasibility](https://github.com/Sphag/moldy/issues/3)

## Notes

- No renderer, platform layer, ECS, assets, editor, event loop, OS integration, or third-party dependency was added for this slice.
- Issues #4, #5, and #6 should close through the `core/phase3-primitives` pull request.
