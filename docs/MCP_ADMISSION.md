# MCP Admission Record

Admission snapshot: 2026-07-13. Owner: repository maintainers.

This record is the repository-owned evidence and decision point for Model Context Protocol (MCP) integrations. It does not install a server, add host or repository configuration, alter credentials, grant permissions, or authorize a tool invocation. Repository instructions and task-specific approval gates continue to apply.

## Status Rules

An MCP integration is `Available` only when its admitted configuration is present in the supported workflow. It is `Approved Optional` only for the documented opt-in boundary and only after the agent verifies its admitted configuration and task prerequisites. `Evaluate`, `Deferred`, and `Rejected` integrations are not installed capabilities and must not be used as prerequisites.

Every admission record must contain the required fields below. Missing, stale, contradictory, or unverified evidence fails closed: do not configure, connect to, or invoke the integration; use its documented fallback instead. A material change to provenance, license, version or integrity evidence, transport, permission surface, credentials, data boundary, owner, or rollback path requires a new review before use.

## Admission Criteria

For each MCP candidate, the following evidence is required. Missing, stale, contradictory, or unverified evidence fails closed.

### 1. Provenance & Maintenance
- Canonical upstream source (GitHub repo / official site)
- Active maintainers and release ownership
- Review process (PR requirements, code of conduct)

### 2. License Compatibility
- Exact license type (MIT/Apache-2.0/etc.)
- License for server binary vs. upstream source
- Attribution requirements

### 3. Version & Integrity
- Exact commit hash / release tag for pinned versions
- Remote endpoints must state if unpinnable
- Integrity verification method (SHA checksum, GPG signature)

### 4. Transport & Security
- Local stdio vs. loopback vs. remote HTTP endpoint
- Authentication mechanism (none / token / OAuth)
- Certificate validation requirements

### 5. Permissions Matrix
- List all exposed tools with granular scope
- Read/write/process/debugger capabilities
- Default privilege level (least-privilege required)

### 6. Data Exposure Boundary
- What data may leave the host (prompts, source paths, artifacts)
- Sensitive material that must never be transmitted
- Token budget for each request

### 7. Smoke Test Evidence
- Bounded success case (without credentials)
- Bounded failure case (denied permission / unavailable tool)
- No credential values in logs or outputs

### 8. Rollback Procedure
- Disable command / config file removal step
- Credential revocation path when applicable
- Native/manual fallback mechanism

### 9. Approval Boundary
- Explicit statement that admission ≠ auto-approval for installation/config
- Required approvals at implementation time
- Cautious-mode gates that still apply

| Required field | Minimum evidence |
| --- | --- |
| Provenance & Maintenance | Canonical source, maintainers, release ownership, and review process. |
| License Compatibility | Exact license type, server vs upstream licensing, and attribution requirements. |
| Version & Integrity | Exact commit hash or release tag for pinned versions; state if remote endpoint is unpinnable; integrity verification method (SHA checksum, GPG signature). |
| Transport & Security | Local stdio vs loopback vs remote HTTP endpoint; authentication mechanism; certificate validation requirements. |
| Permissions Matrix | List of all exposed tools with granular scope; read/write/process/debugger capabilities; default privilege level (least-privilege required). |
| Data Exposure Boundary | What data may leave the host (prompts, source paths, artifacts); sensitive material that must never be transmitted; token budget for each request. |
| Smoke Test Evidence | Bounded success case (without credentials); bounded failure case (denied permission / unavailable tool); no credential values in logs or outputs. |
| Rollback Procedure | Disable command / config file removal step; credential revocation path when applicable; native/manual fallback mechanism. |
| Approval Boundary | Explicit statement that admission ≠ auto-approval for installation/config; required approvals at implementation time; cautious-mode gates that still apply. |

Generic filesystem MCP, generic memory MCP, and sequential-thinking MCP remain `Rejected`. Their decisions in [AI Tooling](AI_TOOLING.md#MCP-candidate-matrix) are unchanged: they duplicate scoped local tools or agent reasoning while adding permission, retention, or trust risk.

| Field | Evidence to Record |
| --- | --- |
| Status and owner | `Available`; repository maintainers own the existing authenticated GitHub workflow. |
| Provenance and license | GitHub-hosted integration via CLI; GitHub service terms apply. Canonical upstream source is [microsoft/gh-cli](https://github.com/microsoft/gh-cli) under the MIT License. Active maintainers from Microsoft own releases and PR review. |
| Version and integrity | Host-installed, no separate pinning needed; verified 2026-07-13. The installed version is determined at runtime by inspecting `gh --version`. No additional checksum or commit hash is required because the CLI is part of the host environment. |
| Transport and credentials | Authenticated HTTPS to `github.com`; keyring-stored credentials. GitHub REST API v3 endpoint. No certificate validation requirements beyond system trust store. |
| Permissions | Read repository/issue/PR/CI metadata within authenticated account permissions; writes require explicit user approval. Exposed tools: read issues, pull requests, commits, branches, workflow runs, and repository metadata. Default privilege level is least-privilege for read-only operations. |
| Data exposure | Limited to authorized repository metadata only; no secrets transmitted. Prompt material is limited to task-relevant issue/PR context. No source code, artifacts, dumps, or environment data are transmitted unless explicitly included in an issue/PR body by the user. |
| Smoke evidence | Account `Sphag` with `repo` and `workflow` scopes confirmed 2026-07-13; read-only queries successfully returned repository metadata, issue [#97](https://github.com/Sphag/moldy/issues/97) metadata, and empty PR list. No token values recorded in logs or outputs. |
| Rollback and fallback | Remove CLI credential via `gh auth logout`; use local Git commands and GitHub web UI as fallback. |

| Field | Evidence to Record |
| --- | --- |
| Status and owner | `Approved Optional`; repository maintainers own periodic review and every opt-in decision. Review cadence: quarterly or when upstream commit/endpoint changes. Current status: not installed or configured by this record. |
| Provenance and license | Canonical upstream source: [MicrosoftDocs/mcp](https://github.com/MicrosoftDocs/mcp), maintained by MicrosoftDocs. The repository declared `CC-BY-4.0` when reviewed on 2026-07-13; this is source-repository license evidence, not a substitute for reviewing any separately distributed client or server component. Attribution requirements follow the CC-BY-4.0 terms. |
| Version and integrity | Reviewed upstream commit: [`caa3d670bf2814171dba4f7346ece5080964021e`](https://github.com/microsoftdocs/mcp/commit/caa3d670bf2814171dba4f7346ece5080964021e). This pins the reviewed source evidence only; the dynamic hosted service endpoint cannot be pinned. No checksum or signature verification is available for the remote endpoint. |
| Transport and credentials | Remote Streamable HTTP only at `https://learn.microsoft.com/api/mcp`. No authentication required. No alternate endpoint, client/host configuration, or automatic installation is admitted by this record. Certificate validation follows system trust store. |
| Permissions | Only public official-documentation search, fetch, and sample retrieval for technical queries. Tool discovery must be dynamic and limited to the task; no broad or assumed tool set is authorized. Default privilege level is least-privilege read-only access. |
| Data exposure | Send no secrets, credentials, proprietary source, repository paths, artifacts, captures, dumps, symbols, environment data, or other sensitive prompt material. Each request must be bounded with a task-specific token budget; never exceed 4096 tokens per request. Returned content is limited to public `learn.microsoft.com` documentation. |
| Smoke evidence | Not yet verified — requires separate explicit opt-in approval before configuration and testing. Expected success case: configure exact endpoint, discover tools dynamically, request a public documentation query within bounded budget, verify returned official material. Expected failure cases: misconfigured endpoint, unavailable service, unknown tool, oversized output must fail closed without transmitting sensitive material. |
| Rollback and fallback | After an approved opt-in, remove the exact host/client configuration and any task-scoped local artifacts; no credential revocation is needed because authentication is not admitted. The fallback is direct official Microsoft documentation on `learn.microsoft.com`. |

Before any Microsoft Learn configuration or smoke connection, obtain separate explicit approval. Recheck the endpoint, source commit, license declaration, tool surface, data boundary, and rollback instructions at that time.

## Admission Workflow

1. **Candidate nomination** — Issue or PR proposes new MCP integration
2. **Evidence gathering** — Assignee collects provenance, license, version, transport, permissions, data exposure, smoke test plan, rollback steps against all 9 criteria
3. **Review** — `dependency-review` skill or equivalent assessment against admission criteria; decision is approve/defer/reject with documented justification
4. **Record update** — Update `MCP_ADMISSION.md` with full evidence matrix and exact boundary statement
5. **Implementation** — Only after explicit approval for installation/config; cautious-mode gates still apply

Close condition: All baseline candidates have complete admission record.

## Rejected Candidates

| Candidate | Reason |
|-----------|--------|
| Generic filesystem MCP | Duplicates scoped workspace file tools; adds permission boundary risk |
| Generic memory MCP | Cross-task persistence not required; increases retention/privacy risk |
| Sequential-thinking MCP | Duplicates agent reasoning without external system of record |

See `docs/AI_TOOLING.md` for detailed rejection rationale per candidate.

---

