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

## GitHub Collaboration

GitHub Issues are the canonical backlog for actionable work. Keep `tasks/current.md` and `tasks/backlog.md` as lightweight session notes that point to active issues and recent completions instead of duplicating issue descriptions.

After commits are explicitly approved, prefer branch-based pull requests for review and validation. Agents must still ask before creating commits, branches, pull requests, or other git history changes.

GitHub Actions cannot run on purely local, unpushed commits. Remote CI runs when commits are pushed to a branch and when pull requests are opened or updated. Local verification remains the required pre-push check when feasible.

See [docs/GITHUB_WORKFLOW.md](GITHUB_WORKFLOW.md) for the expected pull request and CI feedback loop.

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

## Capability-Aware Routing

Use [docs/AI_TOOLING.md](AI_TOOLING.md) as the capability inventory and adoption blueprint.

1. Route work through repository instructions and `Available` scripts or integrations first.
2. Before using an `Approved Optional` capability, verify its admitted version, configuration, permissions, and task-specific prerequisites. Use the documented fallback when it is absent.
3. Treat `Evaluate` capabilities as plans, not installed tools. Work on them only through their implementation or evaluation issue.
4. Do not use `Deferred` or `Rejected` capabilities unless a new decision explicitly changes their status.
5. Preserve cautious-mode approvals regardless of capability status.

The foundational `spec`, `plan-eng-review`, `investigate`, `review`, `ship`, `ci-triage`, `dependency-review`, `checkpoint`, `health`, and `document-release` skills are currently `Evaluate` under issue #95. Until they become available, follow the equivalent working loop in this document and use existing scripts directly. Roadmap-specific GPU capture, scene round-trip, benchmark, render-graph, and performance workflows are also unavailable until their owning track issues are complete.

## Completion Standard

Prefer `scripts/check.ps1` as the default local verification command once project tooling exists. If checks cannot run, report exactly why.
