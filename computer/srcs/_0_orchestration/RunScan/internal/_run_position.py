from __future__ import annotations

from srcs._0_orchestration.RunScan.internal._broadcast_scan_state import _broadcast_scan_state
from srcs._0_orchestration.RunScan.internal._run_shot import _run_shot
from srcs._0_orchestration.ScanOrchestrator.internal._send_and_await import send_and_await
from srcs._0_orchestration.Session import Session
from srcs._1_domain.PiMessages.PiMessages import PiResponseKind
from srcs._1_domain.Scan.Scan import POSITIONS, SHOTS_PER_POSITION, ScanState
from srcs._3_interface.IPiTransport import IPiTransport
from srcs._3_interface.IScanStore import IScanStore


async def _run_position(
    state: ScanState,
    pi: IPiTransport,
    session: Session,
    store: IScanStore,
) -> None:
    position = POSITIONS[state.position_index]
    start_shot = state.shot_index if state.shot_index > 0 else 1
    resp = await send_and_await(session, pi, f"POSITION_{position}", 10.0)
    if resp.kind == PiResponseKind.FAIL:
        raise RuntimeError(f"POSITION_{position} failed")
    for shot in range(start_shot, SHOTS_PER_POSITION + 1):
        await _run_shot(state, shot, pi, session, store)
        await _broadcast_scan_state(session, state)
