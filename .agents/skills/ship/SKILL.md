---
name: ship
description: Deliver already reviewed local changes through an explicitly approved commit, push, draft pull request, or release handoff.
---

## Trigger

Use only when the user explicitly requests delivery of reviewed local changes. Do not use to create commits, branches, pushes, pull requests, releases, or dependency changes implicitly.

## Inputs

- Approved scope, current diff, target repository and branch, and local verification evidence.

## Outputs

- Scoped status, approved commit or remote result, and a handoff report with evidence.

## Mutations and approvals

- Commits, branches, pushes, pull requests, releases, dependency changes, and external writes each require the repository's existing explicit approval.
- If approval is denied, stop.

## Prerequisites and failure behavior

- Require a scoped diff, clean understanding of unrelated changes, authenticated remote access when needed, and feasible checks.
- **Failure:** On denial or command failure, preserve local work, stop without retries that change state, and report the precise local and remote state.

## Verification

- Recheck status and diff, record executed commands, and read back any created remote metadata.
