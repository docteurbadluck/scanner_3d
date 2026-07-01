from __future__ import annotations

from srcs._0_orchestration.ScanOrchestrator.internal._send_and_await import send_and_await
from srcs._0_orchestration.Session import Session
from srcs._1_domain.PiMessages.PiMessages import PiResponseKind
from srcs._1_domain.Scan.Scan import ScanState, photo_filename, scan_state_to_dict
from srcs._3_interface.IPiTransport import IPiTransport
from srcs._3_interface.IScanStore import IScanStore


async def _shoot_at(
    state: ScanState,
    step: int,
    position: str,
    pi: IPiTransport,
    session: Session,
    store: IScanStore,
) -> None:
    filename = photo_filename(position, step)
    state.shot_index = step
    session.set_pending_upload_target(state.name, position, filename)
    if (await send_and_await(session, pi, "TAKE_PHOTO", 15.0)).kind == PiResponseKind.FAIL:
        raise RuntimeError(f"TAKE_PHOTO failed at {position}/{filename}")
    store.save(state.name, scan_state_to_dict(state))
