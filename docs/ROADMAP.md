# Roadmap

This roadmap is intentionally generic and identity-free.

## Phase 0: AI Workflow and Project Infrastructure

- Establish AI-agent operating rules.
- Add placeholder project planning documents.
- Add safe quality script entry points.

## Phase 1: CMake Scaffold, Tool Choices, and Local Quality Gate

- Create a minimal C++ and CMake scaffold.
- Add compiler settings.
- Choose formatting and linting tools.
- Document formatting and linting decisions.
- Verify that `scripts/check.ps1` passes locally.

## Phase 2: Core Runtime Foundations

Current focus:

- Add the first explicit core error/status API.
- Keep new core APIs small, tested, and dependency-free.
- Continue updating source-map and architecture docs alongside public API changes.

Planned follow-up:

- Add logging.
- Add time utilities.
- Define application lifecycle primitives.

## Phase 3: Cross-Platform Platform Layer

- Add window abstraction.
- Add input abstraction.
- Add filesystem abstraction.
- Add timer abstraction.

## Phase 4: First Runnable Sandbox

- Build the first sandbox executable.
- Run on at least one desktop platform.
- Keep platform assumptions explicit.

## Phase 5: Rendering Direction

- Choose 2D or 3D direction.
- Choose renderer direction.
- Record decisions before implementation.
