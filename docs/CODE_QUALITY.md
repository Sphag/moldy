# Code Quality

This project uses strict code quality expectations even before implementation begins.

## Code Expectations

- Consistent formatting is mandatory once the formatter is selected.
- Clear naming is mandatory.
- Public APIs must be small, intentional, and documented when non-obvious.
- Public API, configuration behavior, script behavior, test layout, and source layout changes must update the nearest relevant documentation in the same task.
- Prefer simple, explicit architecture over clever abstractions.
- Avoid speculative generalization.
- Keep module boundaries clear.
- Avoid hidden global state unless explicitly justified.
- Prefer deterministic behavior for core systems.
- Keep ownership, lifetime, and error handling obvious.
- Avoid platform-specific behavior leaking into platform-neutral modules.

## Architecture Expectations

- Every subsystem must have a clear responsibility.
- Cross-module dependencies must be intentional.
- Cross-platform assumptions must be explicit.
- Architectural decisions must be recorded in `docs/DECISIONS.md`.
- If an approach feels uncertain, the agent must stop and propose alternatives instead of pushing a shaky design.

## Formatting Policy

- Use one project-wide formatter once selected.
- Do not mix style within the same module.
- Formatting changes should not be bundled with unrelated logic changes unless explicitly requested.
- Until a formatter is selected, agents should preserve local style and keep formatting simple.

## Current Quality Scripts

- `scripts/configure.ps1` configures the CMake build directory.
- `scripts/build.ps1` builds the configured CMake project.
- `scripts/test.ps1` builds the current test executable and runs CTest.
- `scripts/format.ps1` reports explicit placeholder-pass status because no formatter is selected yet.
- `scripts/lint.ps1` reports explicit placeholder-pass status because no linter or static-analysis tool is selected yet.
- `scripts/check.ps1` runs configure, format, lint, build, and test in order.

The formatter and linter placeholder scripts do not perform formatting or static analysis. Selecting and enforcing those tools remains a future decision.
