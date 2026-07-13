---
name: dependency-review
description: Assess a proposed third-party dependency or tool and provide evidence for an explicit approve, defer, or reject decision.
---

## Trigger

Use when a task proposes adding or updating a third-party dependency or tool. Do not use the assessment as approval to install, pin, vendor, or change a lockfile.

## Inputs

- Source, license, exact version, lock or pin, transitive surface, permissions, and alternatives.

## Outputs

- Provenance and license assessment, risk record, pin/update/rollback plan, and approve/defer/reject recommendation.

## Mutations and approvals

- Read-only assessment. Installing, pinning, vendoring, or changing a lockfile requires separate dependency approval.
- If approval is denied, stop.

## Prerequisites and failure behavior

- Gather primary-source provenance, license, integrity, permission, and removal-path evidence.
- **Failure:** Missing provenance, license, integrity, or rollback evidence produces `Deferred` or `Rejected`; never grant implicit approval.

## Verification

- Verify source, version, license, transitive surface, permissions, smoke-test plan, and removal path against primary evidence.
