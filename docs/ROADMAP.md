# Roadmap

GitHub Issues are the canonical roadmap and backlog. This document describes the track outcomes, dependency gates, and execution order without duplicating task issue descriptions.

## Track 0: Core Math, Memory, and Debuggability Foundations

- Track issue: [#23](https://github.com/Sphag/moldy/issues/23)
- Milestone: `Core Foundations`
- Label: `track:core-foundations`

Outcome: establish the math, allocation, tracing, crash, sanitizer, and GPU diagnostic contracts needed by later tracks.

Existing active work precedes new track tasks:

1. [#17 Pin and enforce the supported development toolchain](https://github.com/Sphag/moldy/issues/17) remains a nested umbrella. Its ordered #18-#21 checklist is maintained only in #17.
2. [#15 Plan async logging queue](https://github.com/Sphag/moldy/issues/15) remains planning-only. Any implementation requires a separate issue.

Track 1 may begin after the math sequence #27-#31 in [Track 0](https://github.com/Sphag/moldy/issues/23), allocator contracts [#32](https://github.com/Sphag/moldy/issues/32), and base trace contracts [#36](https://github.com/Sphag/moldy/issues/36) are complete. Memory tracking, richer tracing, crash tooling, sanitizers, GPU diagnostics, and editor diagnostic panels may then proceed in parallel according to their issue dependencies.

Closed issues #1-#6 and #10-#14 are retained in the milestone and listed as completed groundwork in #23. They provide provenance but do not determine the order of new tasks.

## Track 1: First Rendered Editor Viewport

- Track issue: [#24](https://github.com/Sphag/moldy/issues/24)
- Milestone: `Rendered Editor Viewport`
- Label: `track:rendered-editor-viewport`

Outcome: deliver a Win32 editor host that renders deterministic static geometry and Dear ImGui through explicit RHI and D3D12 adapters. The track includes host and input handling, RHI contracts, device and queue setup, resource and descriptor management, shader compilation, graphics pipelines, editor adapters, and WARP coverage.

This is a Windows-first implementation track, not a change to the Windows, macOS, and Linux desktop target. Backend-neutral contracts must remain separate from Win32 and D3D12 adapters.

## Track 2: Editable and Persistent Scene

- Track issue: [#25](https://github.com/Sphag/moldy/issues/25)
- Milestone: `Editable Persistent Scene`
- Label: `track:editable-persistent-scene`

Outcome: deliver an editable scene that can be saved as versioned JSON source, compiled into a deterministic binary cache, reconstructed transactionally, migrated, and round-tripped. EnTT, nlohmann/json, and ImGuizmo remain isolated behind project-owned adapters and require normal dependency approval before implementation.

Track 2 builds on the editor viewport, math types, allocation contracts, and trace contracts. Individual task dependencies in #25 control safe overlap with unfinished Track 1 work.

## Track 3: Advanced Graphics Foundations

- Track issue: [#26](https://github.com/Sphag/moldy/issues/26)
- Milestone: `Advanced Graphics Foundations`
- Label: `track:advanced-graphics-foundations`

Outcome: deliver validated resource-state, compute, multi-queue, descriptor, shader, render-graph, transient-resource, parallel-recording, and GPU-profiling contracts. WARP contract coverage and evidence-based future-backend criteria close the track.

Track 3 extends the RHI and viewport contracts from Track 1. It does not authorize a second rendering backend; the final task defines the evidence and portability criteria for a separate future decision.

## Roadmap Rules

- The ordered checklist in each track issue is the progress source of truth.
- Every task issue has exactly one `track:*` label, one corresponding milestone, and relevant independent `area:*` labels.
- Closed groundwork remains closed and stays in the milestone so historical progress is visible.
- Nested umbrellas own their child checklists. Parent tracks link the umbrella once and do not duplicate its children.
- Task descriptions, acceptance criteria, and dependency links live in their GitHub issues, not in local roadmap or task files.
