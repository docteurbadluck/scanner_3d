PI_HOST    ?= pi
PI_REPO    ?= ~/3D_scanner
RSYNC      := rsync -avz --exclude-from=.rsync-exclude
SSH        := ssh $(PI_HOST)
PICO_BUILD := $(PI_REPO)/pico/motor/build
PICO_UF2   := $(PICO_BUILD)/motor.uf2

# ── Phase 1 — Prerequisites ─────────────────────────────────────────────────

check-ssh:
	@printf "==> Checking SSH to $(PI_HOST)...\n"
	@$(SSH) "echo ok" || (printf "❌ SSH failed — see docs/remote-workflow.md\n" && exit 1)

check-prereqs: check-ssh
	@printf "==> Checking toolchain on $(PI_HOST)...\n"
	@$(SSH) "bash -s" < scripts/check-prereqs.sh

# ── Phase 2 — Sync ──────────────────────────────────────────────────────────

sync:
	@printf "==> Syncing to $(PI_HOST):$(PI_REPO)...\n"
	@$(RSYNC) --delete . $(PI_HOST):$(PI_REPO)

sync-dry:
	@printf "==> Dry-run sync to $(PI_HOST):$(PI_REPO)...\n"
	@$(RSYNC) --delete --dry-run . $(PI_HOST):$(PI_REPO)

# ── Phase 3 — Remote Pi build/test ──────────────────────────────────────────

build-pi:
	@printf "==> Building Pi code on $(PI_HOST)...\n"
	@$(SSH) "make -C $(PI_REPO)/pi"

test-pi:
	@printf "==> Running Pi tests on $(PI_HOST)...\n"
	@$(SSH) "make test -C $(PI_REPO)/pi"

# ── Phase 4 — Remote Pico build ─────────────────────────────────────────────

build-pico:
	@printf "==> Configuring Pico build on $(PI_HOST)...\n"
	@$(SSH) "PICO_SDK_PATH=~/pico-sdk cmake -B $(PICO_BUILD) -S $(PI_REPO)/pico/motor -G Ninja 2>&1"
	@printf "==> Building Pico firmware on $(PI_HOST)...\n"
	@$(SSH) "cmake --build $(PICO_BUILD)"

# ── Phase 5/6 — Flash Pico ──────────────────────────────────────────────────

flash-pico:
	@printf "==> Rebooting Pico into BOOTSEL mode...\n"
	@$(SSH) "picotool reboot -f -u" || true
	@sleep 2
	@printf "==> Flashing Pico firmware...\n"
	@$(SSH) "picotool load $(PICO_UF2) && picotool reboot" \
		|| (printf "❌ Flash failed — is the Pico connected via USB?\n" && exit 1)

# ── Phase 7 — Full loop ─────────────────────────────────────────────────────

run-pi:
	@printf "==> Starting Pi binary on $(PI_HOST)...\n"
	@$(SSH) "nohup $(PI_REPO)/pi/build/pi > /tmp/pi.log 2>&1 & echo \$$!"

log-pi:
	@$(SSH) "tail -f /tmp/pi.log"

stop-pi:
	@printf "==> Stopping Pi binary on $(PI_HOST)...\n"
	@$(SSH) "pkill -f pi/build/pi || true"

all-remote:
	@printf "==> Starting full remote loop [%s]\n" "$$(date '+%H:%M:%S')"
	@$(MAKE) check-ssh
	@$(MAKE) sync
	@$(MAKE) build-pi
	@$(MAKE) build-pico
	@$(MAKE) flash-pico
	@printf "==> Done [%s]\n" "$$(date '+%H:%M:%S')"
	@$(MAKE) run-pi

.PHONY: check-ssh check-prereqs sync sync-dry \
        build-pi test-pi build-pico flash-pico run-pi log-pi stop-pi all-remote
