---
name: health
description: Assess repository health from supported toolchain, quality-gate, and debt evidence without making automatic fixes or backlog changes.
---

## Trigger

Use for a requested or periodic repository health assessment. Do not use to silently repair failures or create debt issues.

## Inputs

- Supported toolchain state, full-check results, known debt, and recent comparable changes.

## Outputs

- Evidence-based quality summary, comparable trends, confidence limits, and prioritized follow-ups.

## Mutations and approvals

- May run supported read-only checks and create their normal generated build artifacts; do not apply fixes or create external issues automatically.
- If approval is denied, stop.

## Prerequisites and failure behavior

- Use the documented supported toolchain and comparable configurations for trend claims.
- **Failure:** Missing tools, unavailable evidence, and sandbox failures remain visible and lower confidence; they are not passes.

## Verification

- Prefer `scripts/check.ps1`; compare trends only across compatible configurations, toolchains, and evidence windows.
