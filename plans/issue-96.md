# Plan: Issue #96 - Add Deterministic AI Workflow Support Scripts

## Goal
Add deterministic local support scripts for AI-assisted engineering workflow as defined in issue #96.

## Constraints
- PowerShell scripts (cross-platform: Windows, macOS, Linux)
- Must not install tools, perform network writes, or mutate git history
- Preserve cautious-mode approval gates
- Follow existing `scripts/` conventions

## Acceptance Criteria
- Each script has bounded output and scoped paths
- All scripts are documented in `scripts/README.md`
- Scripts follow deterministic patterns (no implicit state)
- Verification via `scripts/check.ps1` passes

## Deterministic AI Workflow Support Scripts

### 1. `scripts/doctor.ps1`
**Role:** Report local repository and supported-toolchain readiness without installing tools.

**Scope:**
- Check tool availability (CMake, compiler, etc.)
- Verify directory structure
- Report status without side effects
- Exit with clear code (0 = ready, 1 = issues found)

### 2. `scripts/context.ps1`
**Role:** Emit bounded, scoped, redacted tracked-file context with stable output.

**Scope:**
- List tracked files in scope
- Apply explicit path filters
- Redact sensitive data (paths, credentials)
- Output stable text or JSON

### 3. `scripts/check-changed.ps1`
**Role:** Run only proven focused checks for documentation or Agent Skills changes; otherwise defer to `check.ps1`.

**Scope:**
- Detect changed files via git
- Apply conservative selector rules
- Run minimal checks for scoped changes
- Fall back to full gate if scope unclear

### 4. `scripts/collect-diagnostics.ps1`
**Role:** Collect deterministic local failure evidence into an explicit ignored artifact directory.

**Scope:**
- Gather error logs and state
- Write only to explicit output paths (ignored)
- No network writes
- Preserve originals; never mutate source files

## Implementation Steps

1. Create `scripts/doctor.ps1`
   - Implement tool checks
   - Add exit codes
   - Document parameters

2. Create `scripts/context.ps1`
   - Implement file listing
   - Add redaction logic
   - Support text/JSON output

3. Create `scripts/check-changed.ps1`
   - Implement git diff detection
   - Add scoped check routing
   - Document fallback behavior

4. Create `scripts/collect-diagnostics.ps1`
   - Implement evidence collection
   - Add path validation
   - Document artifact directory requirement

5. Update `scripts/README.md`
   - Add entries for all new scripts
   - Document parameters and usage

6. Run `scripts/check.ps1` to verify

## Known Risks
- Platform-specific PowerShell version differences (v7+ features)
- Git path separators across platforms
- File permission issues in restricted sandbox environments

## Dependencies
- Issue #93: Establish repository AI-assisted engineering workflow (prerequisite)

## PR Delivery Steps

1. Create branch `feature/issue-96-deterministic-scripts`
2. Commit changes with message following repo style
3. Push branch to remote
4. Open pull request using `.github/pull_request_template.md`
5. Reference issue #96 in description
6. Request review from repository maintainers

## PR Template Checklist

- [ ] Script implementations complete
- [ ] `scripts/README.md` updated
- [ ] `scripts/check.ps1` passes
- [ ] Cross-platform tested (Windows, macOS, Linux)
- [ ] Documentation clear and concise
- [ ] No dependency additions
