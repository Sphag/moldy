# AI Workflow

This repository uses a cautious, reviewable AI-agent workflow.

## Working Loop

1. Clarify intent.
2. Plan the smallest useful slice.
3. Implement the slice.
4. Verify behavior and quality.
5. Review the diff.
6. Update documentation that is affected by the change.
7. Document decisions and usage.
8. Checkpoint long-running work.

Architecture-sensitive work must start with a short design note or plan before implementation.

Platform-layer changes must consider impact on Windows, macOS, and Linux before code is changed.

## Documentation Maintenance

When a task changes source files, build configuration, scripts, tests, applications, public APIs, or directory structure, update the nearest relevant documentation in the same task.

Use the closest document first:

- Module behavior and public headers: nearest module README, such as `src/core/README.md`.
- Script behavior and parameters: `scripts/README.md` and relevant quality or testing docs.
- Source layout, targets, aliases, compile definitions, and output paths: `docs/SOURCE_MAP.md`.
- Workflow rules for agents: `AGENTS.md` and this file.
- Quality expectations: `docs/CODE_QUALITY.md`.

If no documentation update is needed, the completion report must state why.

## Standard Task Prompt Template

```text
Goal:
- What should be true when this task is complete?

Constraints:
- What must not change?
- Which dependencies, platforms, or design boundaries matter?

Acceptance Criteria:
- How will completion be recognized?

Quality Expectations:
- Formatting:
- Naming:
- Architecture:
- Public API clarity:
- Documentation:

Verification:
- Commands to run:
- Manual checks:
- Known checks that are intentionally skipped:
```

## Skill Usage

- Use `plan-eng-review` for architecture planning and architecture review.
- Use `investigate` for bugs, regressions, and root-cause work.
- Use `review` for diff and code review before landing meaningful changes.
- Use `health` for periodic quality checks.
- Use `benchmark` once performance-sensitive systems exist.
- Use `checkpoint` after long sessions or before switching context.
- Use `document-release` after completed subsystems need documentation updates.

## Completion Standard

Prefer `scripts/check.ps1` as the default local verification command once project tooling exists. If checks cannot run, report exactly why.
