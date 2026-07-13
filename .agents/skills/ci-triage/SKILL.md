---
name: ci-triage
description: Triage a failing or inconclusive GitHub Actions check using job-log evidence and report the smallest evidence-backed next action.
---

## Trigger

Use when a GitHub Actions check fails or is inconclusive. Do not assume an external provider is a GitHub Actions failure.

## Inputs

- Repository, pull request or run, failing job, logs, local environment, and recent patch.

## Outputs

- Failing step, cited log evidence, local reproduction status, root cause, and proposed next action.

## Mutations and approvals

- Reading logs is allowed; reruns, fixes, pushes, and workflow edits require task authorization and applicable approvals.
- If approval is denied, stop.

## Prerequisites and failure behavior

- Require authenticated access and the actual job logs before drawing conclusions.
- **Failure:** If logs or access are unavailable, report that limitation and stop; do not infer a failure cause.

## Verification

- Correlate the remote failing step with the patch and, when feasible, a focused local reproduction.
