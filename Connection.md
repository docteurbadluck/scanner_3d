prompt : Read Connection.md.
Then execute Phase X only.
Before making any changes, tell me what you plan to do and what files you will touch.
Only proceed when I confirm.
After completing, run the acceptance criteria checks described in the phase and report results.

# Remote Development Workflow: Computer → Pi → Pico

## Agent Brief

Goal: set up a reliable local-first workflow where I code on my computer, then control everything remotely on Raspberry Pi via SSH:
1) sync repository to Pi with `rsync`,
2) build/test Pi code remotely,
3) build Pico firmware remotely on Pi,
4) flash Pico from Pi (USB first, optional SWD later).

Constraints:
- Do not modify product logic unless required for automation.
- Prefer small, reversible infra changes.
- Keep commands fast and deterministic.
- Use SSH key auth (no password prompts in normal loop).
- Add clear docs and simple one-command entry points.

Deliverables:
1) Remote execution scripts/tasks for:
   - `sync`
   - `build-pi`
   - `test-pi`
   - `build-pico`
   - `flash-pico`
   - `all` (sync + build/test + build/flash)
2) Safe `rsync` configuration:
   - excludes build/cache artifacts,
   - optional `--delete` mode (explicitly controlled),
   - dry-run mode.
3) SSH config guidance:
   - host alias,
   - connection reuse/multiplexing,
   - key-based auth checks.
4) Pico flashing modes:
   - USB UF2 default workflow,
   - optional SWD/OpenOCD workflow.
5) Validation:
   - a verification checklist and pass/fail output per step.
6) Documentation:
   - quick start,
   - troubleshooting for common failures (SSH, permissions, USB detection, flash errors).

Definition of done:
- I can run the full loop from my computer without manual file copying.
- Build/test logs are visible on my computer.
- Pico firmware can be flashed from Pi from my computer-triggered command.
- Each step has an explicit test proving it works.

---

## Implementation Plan

### Phase 1 — Baseline & prerequisites
Task:
- Detect OS/tools on computer and Pi.
- Verify SSH connectivity and key auth.
- Verify required toolchains on Pi (C/C++, CMake/Make, Pico SDK/tooling, flash tools).

Tests:
- `ssh <pi-alias> "echo ok"` returns `ok`.
- No password prompt for SSH.
- Tool version checks succeed on Pi (`cmake`, compiler, pico flashing tool).

Acceptance criteria:
- Agent outputs a prerequisite report with ✅/❌ and exact missing items.

---

### Phase 2 — Reliable sync pipeline (computer → Pi)
Task:
- Create sync command(s) using `rsync` over SSH.
- Add exclude list for artifacts and caches.
- Add dry-run and real-run variants.
- Optional safe-delete mode.

Tests:
- Dry-run shows expected changed files only.
- Real sync transfers edited file.
- Deleted local test file is removed remotely only in delete-enabled mode.

Acceptance criteria:
- Sync is repeatable and idempotent (second run after no changes transfers nothing).

---

### Phase 3 — Remote Pi build/test commands
Task:
- Add remote commands/tasks to build and test Pi-side code from computer.
- Ensure logs stream back to local terminal.

Tests:
- `build-pi` exits 0 on clean state.
- `test-pi` exits 0 and reports tests executed.
- Introduce a small intentional compile failure and verify non-zero exit + readable error.

Acceptance criteria:
- Exit codes propagate correctly to local machine.

---

### Phase 4 — Remote Pico build on Pi
Task:
- Add command/task to compile Pico firmware on Pi from synced sources.
- Ensure build artifacts path is clear and stable.

Tests:
- `build-pico` exits 0.
- Expected firmware artifact exists after build (UF2/ELF as relevant).

Acceptance criteria:
- Rebuild after no code changes is fast and succeeds.

---

### Phase 5 — Pico flash from Pi (USB default)
Task:
- Implement default flash workflow via USB UF2 from Pi.
- Add detection checks (device connected/mounted or tool-visible).

Tests:
- With Pico connected: `flash-pico` exits 0 and device updates.
- Without Pico connected: command fails clearly with actionable message.

Acceptance criteria:
- Flash step is deterministic and error messages are human-readable.

---

### Phase 6 — Optional SWD/OpenOCD mode
Task:
- Add optional debug-probe flash path (SWD/OpenOCD), selectable via flag/task.
- Keep USB path as default.

Tests:
- SWD mode detects probe and target.
- Flash over SWD exits 0.

Acceptance criteria:
- User can choose USB or SWD without changing code manually.

---

### Phase 7 — Single-command developer loop
Task:
- Add orchestrated `all` command chaining:
  `sync -> build-pi -> test-pi -> build-pico -> flash-pico`
- Add per-step timing and fail-fast behavior.

Tests:
- Full command succeeds on happy path.
- If one step fails, pipeline stops and returns non-zero.
- Output clearly indicates which step failed.

Acceptance criteria:
- One command from computer controls complete loop.

---

### Phase 8 — Documentation & handoff
Task:
- Write concise "daily workflow" and "troubleshooting" guide.
- Include common failure signatures and fixes.

Tests:
- Fresh run by following docs only (no hidden knowledge).
- Another person can execute full loop end-to-end.

Acceptance criteria:
- Documentation is sufficient for independent use.

---

## QA Checklist (final validation)
- SSH key auth works without prompt.
- Sync works (dry-run + real-run + delete mode behavior validated).
- Pi build/test callable remotely with correct exit codes.
- Pico build callable remotely.
- Pico flash callable remotely (USB default).
- Optional SWD flash works if probe connected.
- One-command full loop works and is fail-fast.
- Docs complete and tested.
