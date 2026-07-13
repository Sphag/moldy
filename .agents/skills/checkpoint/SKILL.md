---
name: checkpoint
description: Capture a concise, redacted, resumable work state when a long task pauses, compacts, or moves to another context.
---

## Trigger

Use when active work is pausing, context is changing, or a handoff needs verified state. Do not use to publish private state or credentials.

## Inputs

- Goal, git status, decisions, changed files, completed checks, blockers, and next steps.

## Outputs

- Redacted resumable state with facts separated from assumptions and clear next actions.

## Mutations and approvals

- Read-only by default; write a local checkpoint only when repository convention permits it. External publication requires authorization.
- If approval is denied, stop.

## Prerequisites and failure behavior

- Inspect current status and redact credentials, private paths, dumps, and unrelated user data.
- **Failure:** If state cannot be verified, mark it unknown and identify the command or evidence needed; do not fill gaps with assumptions.

## Verification

- Compare the checkpoint with git status, changed files, completed checks, and issue state.
