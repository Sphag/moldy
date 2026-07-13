---
name: document-release
description: Synchronize documentation for an implemented subsystem or workflow before landing without inventing unimplemented behavior or publishing a release.
---

## Trigger

Use after a completed implementation needs documentation synchronized before landing. Do not use to document speculative behavior or create a release.

## Inputs

- Implemented diff, nearest documentation, public contracts, issue context, and verification evidence.

## Outputs

- Documentation gap list and scoped updates that describe only implemented behavior.

## Mutations and approvals

- May edit in-scope documentation; do not create releases, commits, pushes, or pull requests without their existing approvals.
- If approval is denied, stop.

## Prerequisites and failure behavior

- Inspect implementation evidence and the closest documentation before writing.
- **Failure:** If evidence is missing, report the documentation gap and stop; do not publish speculative claims or status.

## Verification

- Check local links, source and API claims, issue references, and the full quality gate when feasible.
