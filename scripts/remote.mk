PI_HOST    ?= raspi1@<ip_addr>
PI_REPO    ?= ~/3d-scanner/
FLASH_MODE ?= usb

RSYNC      := rsync -avz --exclude-from=.rsync-exclude
SSH        := ssh $(PI_HOST)
PICO_BUILD := $(PI_REPO)/pico/motor/build
PICO_ELF   := $(PICO_BUILD)/motor.elf
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
	@$(RSYNC) . $(PI_HOST):$(PI_REPO)

sync-dry:
	@printf "==> Dry-run sync to $(PI_HOST):$(PI_REPO)...\n"
	@$(RSYNC) --dry-run . $(PI_HOST):$(PI_REPO)

sync-delete:
	@printf "==> Syncing (with --delete) to $(PI_HOST):$(PI_REPO)...\n"
	@$(RSYNC) --delete . $(PI_HOST):$(PI_REPO)

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
	@$(SSH) "cmake -B $(PICO_BUILD) -S $(PI_REPO)/pico/motor -G Ninja 2>&1"
	@printf "==> Building Pico firmware on $(PI_HOST)...\n"
	@$(SSH) "cmake --build $(PICO_BUILD)"

# ── Phase 5/6 — Flash Pico ──────────────────────────────────────────────────

flash-pico:
ifeq ($(FLASH_MODE),swd)
	@printf "==> Flashing Pico via SWD (openocd) on $(PI_HOST)...\n"
	@$(SSH) "openocd -f interface/raspberrypi-native.cfg \
		-f target/rp2040.cfg \
		-c 'program $(PICO_ELF) verify reset exit'"
else
	@printf "==> Flashing Pico via USB (picotool) on $(PI_HOST)...\n"
	@$(SSH) "picotool load $(PICO_UF2) --force" \
		|| (printf "❌ Flash failed — is the Pico connected and visible to picotool?\n" && exit 1)
endif

# ── Phase 7 — Full loop ─────────────────────────────────────────────────────

all-remote:
	@printf "==> Starting full remote loop [%s]\n" "$$(date '+%H:%M:%S')"
	@$(MAKE) check-ssh
	@$(MAKE) sync
	@$(MAKE) build-pi
	@$(MAKE) test-pi
	@$(MAKE) build-pico
	@$(MAKE) flash-pico
	@printf "==> Done [%s]\n" "$$(date '+%H:%M:%S')"

.PHONY: check-ssh check-prereqs sync sync-dry sync-delete \
        build-pi test-pi build-pico flash-pico all-remote
