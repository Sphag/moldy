# Backlog

- Continue Phase 3 core APIs:
  - Add logging.
  - Add time utilities.
  - Define application lifecycle primitives.
- Migrate local task tracking into GitHub Issues:
  - Create issues for active items from `tasks/current.md`.
  - Create issues for pending items from `tasks/backlog.md`.
  - Keep local task files as lightweight session notes once GitHub Issues is active.
- Verify the first GitHub Actions CI run on a pushed branch and adjust only if runner tooling fails.
- Expand CI to macOS and Linux after runner toolchain feasibility is verified.
- Decide whether to keep plain executable tests or approve a test framework dependency.
- Evaluate `import std` later as a Ninja-only experiment until required generators support it cleanly.
- Decide any future reflection approach separately from the C++23 scaffold.
- Add more core APIs only when driven by concrete needs.
