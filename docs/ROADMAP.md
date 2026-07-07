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

- Migrate actionable items from `tasks/current.md` and `tasks/backlog.md` into GitHub Issues.
- Use a PR-first workflow after commits are explicitly approved.
- Run GitHub Actions CI for pushed branch commits and pull request updates.
- Start CI on `windows-latest` with the existing PowerShell quality gate.
- Improve README onboarding for first-time readers without overstating implemented features.

Planned follow-up:

- Expand CI to macOS and Linux after runner toolchain feasibility is verified.

## Phase 3: Core Runtime Foundations

Current focus:

- Add the first explicit core error/status API.
- Keep new core APIs small, tested, and dependency-free.
- Continue updating source-map and architecture docs alongside public API changes.

Planned follow-up:

- Add logging.
- Add time utilities.
- Define application lifecycle primitives.

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
