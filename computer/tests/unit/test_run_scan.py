from __future__ import annotations

import json
from pathlib import Path

import pytest

from srcs._0_orchestration.RunScan import RunScan
from srcs._0_orchestration.Session import Session
from srcs._1_domain.PiMessages.PiMessages import PiResponse, PiResponseKind
from srcs._1_domain.Scan.Scan import (
    ScanState, ScanStatus,
    scan_state_from_dict, scan_state_to_dict,
)
from srcs._4_framework.ScanStore import ScanStore


class ScriptedPi:
    """Auto-resolves pending futures; fails on the Nth TAKE_PHOTO if requested."""

    def __init__(self, session: Session, fail_at_take_photo: int | None = None) -> None:
        self._session = session
        self._fail_at = fail_at_take_photo
        self._take_photo_count = 0
        self.sent: list[str] = []

    async def send(self, message: str) -> None:
        data = json.loads(message)
        command: str = data.get("command", "")
        self.sent.append(command)
        if command == "TAKE_PHOTO":
            self._take_photo_count += 1
        should_fail = (
            command == "TAKE_PHOTO"
            and self._fail_at is not None
            and self._take_photo_count == self._fail_at
        )
        kind = PiResponseKind.FAIL if should_fail else PiResponseKind.DONE
        self._session.pending().resolve_if_match(
            PiResponse(kind=kind, payload="", command=command)
        )


async def test_happy_path_command_counts(tmp_path: Path) -> None:
    session = Session()
    pi = ScriptedPi(session)
    await RunScan(session, ScanStore(tmp_path)).start("scan", pi)
    assert pi.sent.count("TAKE_PHOTO") == 40
    assert pi.sent.count("PLATE_NEXT") == 18
    assert sum(1 for c in pi.sent if c.startswith("POSITION_")) == 22


async def test_happy_path_final_state_is_done(tmp_path: Path) -> None:
    session = Session()
    store = ScanStore(tmp_path)
    await RunScan(session, store).start("scan", ScriptedPi(session))
    assert scan_state_from_dict(store.load("scan")).status == ScanStatus.DONE


async def test_fail_on_take_photo_stores_failed_state(tmp_path: Path) -> None:
    session = Session()
    store = ScanStore(tmp_path)
    # Pair AB: step 7 (odd) → first=A (photo 13), second=B (photo 14) fails
    pi = ScriptedPi(session, fail_at_take_photo=14)
    await RunScan(session, store).start("scan", pi)
    state = scan_state_from_dict(store.load("scan"))
    assert state.status == ScanStatus.FAILED
    assert state.position_index == 0
    assert state.shot_index == 7


async def test_resume_from_failed_state(tmp_path: Path) -> None:
    session = Session()
    store = ScanStore(tmp_path)
    failed = ScanState(name="scan", status=ScanStatus.FAILED, position_index=0, shot_index=7)
    store.ensure_folders("scan")
    store.save("scan", scan_state_to_dict(failed))

    pi = ScriptedPi(session)
    await RunScan(session, store).resume("scan", pi)

    # Pair AB steps 7-10 = 8 photos, pair CD steps 1-10 = 20 photos
    assert pi.sent.count("TAKE_PHOTO") == 28
    assert "POSITION_A" in pi.sent
    assert "POSITION_C" in pi.sent
    assert scan_state_from_dict(store.load("scan")).status == ScanStatus.DONE


async def test_start_rejects_invalid_name(tmp_path: Path) -> None:
    session = Session()
    with pytest.raises(ValueError, match="Invalid scan name"):
        await RunScan(session, ScanStore(tmp_path)).start("bad name!", ScriptedPi(session))


async def test_start_rejects_existing_scan(tmp_path: Path) -> None:
    session = Session()
    store = ScanStore(tmp_path)
    store.save("dup", {"name": "dup", "status": "done"})
    with pytest.raises(ValueError, match="already exists"):
        await RunScan(session, store).start("dup", ScriptedPi(session))


async def test_resume_rejects_missing_scan(tmp_path: Path) -> None:
    session = Session()
    with pytest.raises(ValueError, match="not found"):
        await RunScan(session, ScanStore(tmp_path)).resume("ghost", ScriptedPi(session))
