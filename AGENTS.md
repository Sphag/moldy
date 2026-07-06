# AI Agent Instructions

This file is the primary source of truth for AI-agent behavior in this repository.

## Language

- Repository documentation, code identifiers, filenames, commit messages, and public technical artifacts must be written in English.
- User-facing conversation should be in Russian unless the user asks for another language.

## Project Boundaries

- The technical stack is C++ with CMake.
- Target platforms are Windows, macOS, and Linux desktop.
- Current focus is core-first workflow and infrastructure, not 2D or 3D engine implementation.
- Do not add concrete project identity details such as an engine name, product positioning, project status, or detailed product description unless a task explicitly asks for them.

## Cautious Mode

Agents must ask before:

- Adding dependencies.
- Making architecture pivots.
- Performing broad refactors.
- Running destructive actions.
- Creating commits, branches, pull requests, or other git history changes.

Do not initialize git unless the user explicitly requests it.

## Task Discipline

- Keep changes scoped to the task.
- Do not perform unrelated refactors during task work.
- Prefer small, reviewable vertical slices.
- Follow existing project patterns once they exist.
- If the requested work conflicts with these instructions, stop and clarify with the user.

## Code Quality

- Code must be readable, consistently formatted, locally coherent, and aligned with existing project patterns.
- Public APIs must be understandable and intentionally shaped.
- Architecture changes must be intentional, documented, and defensible.
- Avoid speculative architecture, unclear abstractions, and premature generalization.

## Documentation Maintenance

- When changing source files, build configuration, scripts, tests, applications, public APIs, or directory structure, update the nearest relevant documentation in the same task.
- If no documentation update is needed, the final report must explicitly say why.
- Keep documentation English, scoped, current, and free of speculative project identity.

## Completion Reports

Final reports must include:

- Changed files.
- Checks run.
- Checks not run.
- Known risks or follow-up work.

Run `scripts/check.ps1` before reporting completion when feasible.
