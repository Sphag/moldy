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

## Pull Request Descriptions

- Use `.github/pull_request_template.md` for pull request descriptions.
- Keep pull request descriptions compact and scannable.
- Do not use large Markdown headings such as `#` or `##` in pull request descriptions.
- Prefer bold inline section labels, short bullets, and checklists.
- Keep generated pull request bodies formatted with real line breaks; do not collapse sections into one long line.

## Generated Artifact Cleanup

- Prefer short-lived, task-specific build directories such as `build-check` only while they are needed.
- At task completion, remove generated build directories such as `build/`, `build-*`, and `out/` when they are untracked or ignored and no longer needed.
- Also remove temporary local directories such as `.tmp/`, `tmp/`, and `temp/` when they are untracked or ignored and no longer needed.
- Before deleting generated artifacts, inspect a dry-run preview such as `git clean -ndX` or an explicit directory listing.
- If deletion fails because of the Windows restricted sandbox limitation, do not change CMake configuration, source code, or build scripts as a workaround; ask for an approved elevated command run or instruct the user to clean up from a normal terminal.
- Never delete staged files, tracked files, source directories, documentation, scripts, or user-created unignored directories as part of routine cleanup.

## Known Local Command Sandbox Limitation

- In restricted Codex sandbox sessions on Windows, commands may be able to create and write files but fail when deleting, replacing, or renaming files.
- Symptoms include CMake errors such as `Cannot restore timestamp ... Access is denied`, failed removal of generated `.tmp` files, or `clang-format -i` reporting `permission denied` while direct file writes still work.
- `gh` commands that contact GitHub may fail or misreport authentication in restricted sessions even when the keyring token works in a normal terminal or approved command context.
- Before refreshing GitHub CLI authentication or logging in again, verify `gh auth status` from a normal terminal or approved command context.
- If approved `gh auth status` for `github.com` account `Sphag` shows `repo` and `workflow`, treat GitHub CLI authentication as valid and do not re-login.
- Treat this as a command sandbox limitation after confirming that the same operation succeeds in a normal shell or an approved elevated command run.
- Do not change CMake configuration, source code, or build scripts only to work around this sandbox behavior.
- For `scripts/check.ps1`, CMake configure/build/test, or other tools that need delete/rename access, use a normal terminal or ask for an approved elevated command run.
