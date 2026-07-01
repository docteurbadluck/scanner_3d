from __future__ import annotations

from srcs._0_orchestration.RunScan.internal._broadcast_scan_state import _broadcast_scan_state
from srcs._0_orchestration.RunScan.internal._run_step import _run_step
from srcs._0_orchestration.ScanOrchestrator.internal._send_and_await import send_and_await
from srcs._0_orchestration.Session import Session
from srcs._1_domain.PiMessages.PiMessages import PiResponseKind
from srcs._1_domain.Scan.Scan import POSITION_PAIRS, STEPS_PER_PAIR, ScanState
from srcs._3_interface.IPiTransport import IPiTransport
from srcs._3_interface.IScanStore import IScanStore


async def _run_pair(
    state: ScanState,
    pi: IPiTransport,
    session: Session,
    store: IScanStore,
) -> None:
    pair = POSITION_PAIRS[state.position_index]
    start_step = state.shot_index if state.shot_index > 0 else 1
    first = pair[0] if start_step % 2 == 1 else pair[1]
    resp = await send_and_await(session, pi, f"POSITION_{first}", 10.0)
    if resp.kind == PiResponseKind.FAIL:
        raise RuntimeError(f"POSITION_{first} failed")
    for step in range(start_step, STEPS_PER_PAIR + 1):
        await _run_step(state, step, pair, pi, session, store)
        await _broadcast_scan_state(session, state)
        if step < STEPS_PER_PAIR:
            resp = await send_and_await(session, pi, "PLATE_NEXT", 5.0)
            if resp.kind == PiResponseKind.FAIL:
                raise RuntimeError(f"PLATE_NEXT failed at step {step}")
