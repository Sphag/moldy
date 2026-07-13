---
name: plan-eng-review
description: Review an architecture-sensitive or multi-step implementation plan before execution and identify missing decisions or unsafe ordering.
---

## Trigger

Use when a proposed implementation plan changes public contracts, spans several steps, or has architectural risk. Do not use as a substitute for code review after a diff exists.

## Inputs

- Proposed plan, repository constraints, dependency graph, affected APIs, and relevant documentation.

## Outputs

- Findings, missing decisions, ordering corrections, risk controls, and an approved-or-blocked recommendation.

## Mutations and approvals

- Read-only; write a revised plan only when requested. Do not implement changes or adopt architecture while reviewing.
- If approval is denied, stop.

## Prerequisites and failure behavior

- Read `AGENTS.md`, the plan, and the affected contracts before evaluating it.
- **Failure:** If the plan or affected contract is unavailable, report the bounded context gap and stop the recommendation.

## Verification

- Trace each recommendation to an observable risk, dependency, repository constraint, or public contract.
