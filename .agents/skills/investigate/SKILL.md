---
name: investigate
description: Investigate a bug, regression, crash, flaky check, or unexplained behavior and report evidence-based root cause or bounded hypotheses.
---

## Trigger

Use when observed behavior needs root-cause analysis. Do not use to make a speculative fix without reproduction or evidence.

## Inputs

- Reproduction, logs or artifacts, suspected range, environment, and known-good evidence.

## Outputs

- Reproduction result, evidence chain, root cause or ranked hypotheses, and the smallest proposed fix.

## Mutations and approvals

- Read-only by default; scoped diagnostic artifacts are allowed. Do not implement a fix, run destructive reproduction steps, or publish results externally without approval.
- If approval is denied, stop.

## Prerequisites and failure behavior

- Preserve the available environment and evidence before changing variables.
- **Failure:** If reproduction is unavailable, report the limitation, rank hypotheses, and name the evidence that would distinguish them.

## Verification

- Repeat the focused reproduction or check when feasible, and distinguish observed cause from correlation.
