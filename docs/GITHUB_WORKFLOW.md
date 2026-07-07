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

GitHub Issues should become the canonical backlog after issue write access is available and actionable local tasks have been migrated. Until that migration is complete, `tasks/current.md` and `tasks/backlog.md` remain the local session notes and pending-work summary.

Created issue set:

- [#1 Remote Windows CI loop verification](https://github.com/Sphag/moldy/issues/1).
- [#2 GitHub Issues migration and local task-file slimming](https://github.com/Sphag/moldy/issues/2).
- [#3 macOS and Linux CI runner feasibility](https://github.com/Sphag/moldy/issues/3).
- [#4 Core logging primitives](https://github.com/Sphag/moldy/issues/4).
- [#5 Core time utilities](https://github.com/Sphag/moldy/issues/5).
- [#6 Application lifecycle primitives](https://github.com/Sphag/moldy/issues/6).

## CI Tool Bootstrap

The Windows job runs `scripts/install-tools.ps1` before the quality gate so runner-provided tool binaries are checked for basic health, not just presence on `PATH`. This matters when a hosted runner image exposes a broken tool earlier in `PATH` than the intended installation.

## Boundaries

- Agents must ask before creating commits, branches, pull requests, or other git history changes.
- Do not treat external checks as GitHub Actions unless their logs are available through GitHub Actions.
- Do not change source code only to work around local restricted-sandbox failures.
- Keep CI fixes tied to the failing log evidence.
