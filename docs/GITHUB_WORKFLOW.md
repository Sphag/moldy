# GitHub Workflow

This document describes the expected GitHub feedback loop for CI, pull requests, and AI-assisted fixes.

## Prerequisites

- Git remote configured for this repository.
- GitHub CLI (`gh`) installed locally.
- GitHub CLI authenticated with access to this repository and GitHub Actions logs.

Known-good GitHub CLI access for this repository is:

- Host: `github.com`.
- Account: `Sphag`.
- Required scopes: `repo` and `workflow`.

Check local GitHub CLI access from a normal terminal with:

```powershell
gh auth status
```

If `gh auth status` fails or reports invalid authentication inside a restricted agent sandbox, retry the same check from a normal terminal or approved command context before refreshing credentials. Restricted sessions may misreport authentication even when the keyring token is valid.

Only refresh authentication if the normal terminal or approved command context still lacks the required account, host, or scopes:

```powershell
gh auth refresh -h github.com -s repo -s workflow
```

If approved `gh auth status` shows `github.com`, account `Sphag`, and scopes `repo` and `workflow`, do not re-login. Future GitHub write operations may still need an approved command context, but they do not need a fresh token.

## Local Pre-Push Check

Run the local quality gate before pushing when feasible:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\check.ps1 -BuildDir build-check
```

In restricted sandbox sessions, CMake or MSBuild may fail with local file access errors while normal terminals work. Treat those as local environment blockers, not CI results.

## Remote CI Loop

GitHub Actions cannot run on unpushed local commits. Remote CI starts after a branch is pushed or a pull request is opened or updated.

Expected loop:

1. Make the smallest reviewable local change.
2. Run the local quality gate when feasible.
3. Commit, push a branch, and open or update a pull request after explicit approval.
4. Let GitHub Actions run on the pushed branch or pull request update.
5. If CI fails, inspect the failing check logs.
6. Fix only the observed root cause.
7. Re-run the relevant local check.
8. Push the follow-up commit after explicit approval.

## Pull Request Descriptions

Use `.github/pull_request_template.md` when opening or updating pull requests. Keep the description compact: use bold inline section labels, short bullets, and checklists. Avoid large Markdown headings such as `#` or `##`, because they make generated pull request bodies visually noisy in GitHub.

Do not collapse generated pull request descriptions into one long line. Preserve real line breaks before sending the body to `gh pr create` or `gh pr edit`.

## AI-Assisted CI Fix Prompt

Use a prompt like this when GitHub Actions fails:

```text
Fix the failing GitHub Actions check for PR #<number>.
Read the failed CI logs, identify the root cause, propose a focused fix, and wait for approval before editing.
```

If there is no pull request yet, provide the branch name or the failing GitHub Actions run URL.

## Useful Commands

List PR checks:

```powershell
gh pr checks <pr-number>
```

Inspect a run:

```powershell
gh run view <run-id>
gh run view <run-id> --log
```

Watch the latest run:

```powershell
gh run watch
```

View current branch PR metadata:

```powershell
gh pr view --json number,url,headRefName,statusCheckRollup
```

For public repositories, the latest GitHub Actions runs can also be checked without local `gh` authentication:

```powershell
Invoke-RestMethod -Uri 'https://api.github.com/repos/Sphag/moldy/actions/runs?per_page=5' -Headers @{ 'User-Agent' = 'Codex' }
```

## Issue Tracking

GitHub Issues are the canonical source for actionable work, dependencies, acceptance criteria, ordering, and completion state. `docs/ROADMAP.md` summarizes track outcomes and gates; `tasks/current.md` and `tasks/backlog.md` contain only lightweight links.

The roadmap is organized by four track issues:

- [#23 Core Math, Memory, and Debuggability Foundations](https://github.com/Sphag/moldy/issues/23)
- [#24 First Rendered Editor Viewport](https://github.com/Sphag/moldy/issues/24)
- [#25 Editable and Persistent Scene](https://github.com/Sphag/moldy/issues/25)
- [#26 Advanced Graphics Foundations](https://github.com/Sphag/moldy/issues/26)

Each track has a matching milestone and `track:*` label. Every task issue must have exactly one track label, the matching milestone, and any relevant independent `area:*` labels. Track issues contain outcomes, close conditions, ordered linked checklists, and a separate completed-groundwork section; they do not copy task descriptions.

Closed groundwork remains closed and is assigned to the applicable milestone so milestone history includes completed work. Do not reopen completed issues to make roadmap progress appear sequential.

Nested umbrella issues own their child checklists. Track #23 links [#17](https://github.com/Sphag/moldy/issues/17) once; #17 alone maintains the ordered #18-#21 checklist. Do not duplicate nested children in a track checklist because the two progress views can diverge.

When roadmap state changes:

1. Update the task issue and its dependency links first.
2. Update the owning track checklist if an issue is added, removed, or reordered.
3. Keep its milestone and single track label aligned.
4. Update local roadmap or task files only when track outcomes, gates, or active links change.
5. Preserve completed-groundwork provenance separately from future work.

## CI Tool Bootstrap

The Windows job runs `scripts/install-tools.ps1` before the quality gate so runner-provided tool binaries are checked for basic health, not just presence on `PATH`. This matters when a hosted runner image exposes a broken tool earlier in `PATH` than the intended installation.

PR [#7](https://github.com/Sphag/moldy/pull/7) verified the PR-based Windows CI loop. Before merge on 2026-07-07, both `Windows quality gate` check runs completed successfully for the pull request.

## macOS And Linux CI Feasibility

macOS and Linux CI are feasible future additions, but they are intentionally deferred. The current tool bootstrap is Windows-specific: `.github/workflows/ci.yml` runs `scripts/install-tools.ps1 -InstallWindows`, relies on `winget` behavior for installation, and assumes a Windows runner for the required quality gate.

Before macOS or Linux jobs become required, define a cross-platform tool installation strategy for CMake, LLVM tools, and cppcheck. That strategy should avoid copying Windows runner assumptions into non-Windows jobs.

## Boundaries

- Agents must ask before creating commits, branches, pull requests, or other git history changes.
- Do not treat external checks as GitHub Actions unless their logs are available through GitHub Actions.
- Do not change source code only to work around local restricted-sandbox failures.
- Keep CI fixes tied to the failing log evidence.
