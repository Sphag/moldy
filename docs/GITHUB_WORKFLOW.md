# GitHub Workflow

This document describes the expected GitHub feedback loop for CI, pull requests, and AI-assisted fixes.

## Prerequisites

- Git remote configured for this repository.
- GitHub CLI (`gh`) installed locally.
- GitHub CLI authenticated with access to this repository and GitHub Actions logs.

Check local GitHub CLI access with:

```powershell
gh auth status
```

If the required scopes are missing, refresh authentication with:

```powershell
gh auth refresh -s repo -s workflow
```

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

## Boundaries

- Agents must ask before creating commits, branches, pull requests, or other git history changes.
- Do not treat external checks as GitHub Actions unless their logs are available through GitHub Actions.
- Do not change source code only to work around local restricted-sandbox failures.
- Keep CI fixes tied to the failing log evidence.
