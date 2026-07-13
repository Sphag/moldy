---
name: spec
description: Produce a decision-ready specification when a new contract, workflow, or behavior is materially ambiguous.
---

## Trigger

Use for a new contract or an ambiguous behavior that needs explicit scope and acceptance criteria. Do not use to implement an already approved, unambiguous change.

## Inputs

- Goal, constraints, affected contracts, platform scope, and available issue or documentation evidence.

## Outputs

- Scope, invariants, alternatives, acceptance criteria, risks, dependencies, and a verification plan.

## Mutations and approvals

- Read-only by default. Edit a requested design document or issue only within the approved task scope; architecture adoption remains separately approved.
- If approval is denied, stop.

## Prerequisites and failure behavior

- Read `AGENTS.md`, the closest applicable documentation, and current task context first.
- **Failure:** If a product or architecture decision is missing, stop and identify the exact decision instead of inventing it.

## Verification

- Trace every acceptance criterion to the stated goal or constraint; cover success, failure, edge cases, desktop platforms, and documentation impact.
