---
name: review
description: Perform an evidence-based pre-landing review of a meaningful diff or patch against its acceptance criteria and affected contracts.
---

## Trigger

Use when a scoped patch needs review before landing. Do not use to amend code, resolve comments, or publish a review unless explicitly asked.

## Inputs

- Base and head, complete diff, acceptance criteria, test evidence, and affected public contracts.

## Outputs

- Prioritized findings with file evidence, test gaps, residual risk, or a clear no-findings result.

## Mutations and approvals

- Read-only. Do not modify the patch, resolve review threads, or create a remote review without approval.
- If approval is denied, stop.

## Prerequisites and failure behavior

- Establish an unambiguous comparison base and inspect the complete scoped diff.
- **Failure:** If the base, diff, or relevant evidence is incomplete, report the missing evidence and stop rather than infer coverage.

## Verification

- Inspect relevant tests and documentation; cite precise file locations for each finding and separate facts from risk assessment.
