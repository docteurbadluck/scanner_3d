from __future__ import annotations

from srcs._0_orchestration.ScanOrchestrator.internal._send_and_await import send_and_await
from srcs._0_orchestration.Session import Session
from srcs._1_domain.PiMessages.PiMessages import PiResponseKind
from srcs._1_domain.Scan.Scan import (
    POSITIONS, SHOTS_PER_POSITION, ScanState, photo_filename, scan_state_to_dict,
)
from srcs._3_interface.IPiTransport import IPiTransport
from srcs._3_interface.IScanStore import IScanStore


async def _rotate_if_needed(
    session: Session,
    pi: IPiTransport,
    shot: int,
    position: str,
    filename: str,
) -> None:
    if shot >= SHOTS_PER_POSITION:
        return
    resp = await send_and_await(session, pi, "PLATE_NEXT", 5.0)
    if resp.kind == PiResponseKind.FAIL:
        raise RuntimeError(f"PLATE_NEXT failed after {position}/{filename}")


async def _run_shot(
    state: ScanState,
    shot: int,
    pi: IPiTransport,
    session: Session,
    store: IScanStore,
) -> None:
    position = POSITIONS[state.position_index]
    filename = photo_filename(position, shot)
    state.shot_index = shot
    session.set_pending_upload_target(state.name, position, filename)
    if (await send_and_await(session, pi, "TAKE_PHOTO", 15.0)).kind == PiResponseKind.FAIL:
        raise RuntimeError(f"TAKE_PHOTO failed at {position}/{filename}")
    store.save(state.name, scan_state_to_dict(state))
    await _rotate_if_needed(session, pi, shot, position, filename)
