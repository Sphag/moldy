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

## Phase 2: GitHub Collaboration and CI Workflow

Operational:

- GitHub Actions runs the Windows quality gate on pushed branch commits and pull request updates.
- The workflow uses `windows-latest` with the existing PowerShell quality gate.
- README onboarding describes the current scaffold and local quality-gate path.
- The latest verified remote Windows run on `main` succeeded for commit `c520e16`.

Remaining closure:

- Use [GitHub issue #2](https://github.com/Sphag/moldy/issues/2) to slim local task files now that the initial issue set exists.
- Keep local task files as lightweight session notes after GitHub Issues are active.
- Continue using a PR-first workflow after commits are explicitly approved.

Planned follow-up:

- Expand CI to macOS and Linux after runner toolchain feasibility is verified.

## Phase 3: Core Runtime Foundations

Current focus:

- Add core diagnostics runtime foundations: logging, assertions, and steady timestamps.
- Treat `RelWithDebInfo` as the optimized-with-asserts check configuration.
- Define application lifecycle primitives.
- Keep new core APIs small, tested, and dependency-free.
- Continue updating source-map and architecture docs alongside public API changes.

Planned follow-up:

- Plan async logging queue requirements before implementing queueing or background worker behavior.

## Phase 4: Cross-Platform Platform Layer

- Add window abstraction.
- Add input abstraction.
- Add filesystem abstraction.
- Add timer abstraction.

## Phase 5: First Runnable Sandbox

- Build the first sandbox executable.
- Run on at least one desktop platform.
- Keep platform assumptions explicit.

## Phase 6: Rendering Direction

- Choose 2D or 3D direction.
- Choose renderer direction.
- Record decisions before implementation.
