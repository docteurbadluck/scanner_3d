from __future__ import annotations

from srcs._0_orchestration.RunScan.internal._run_shot import _shoot_at
from srcs._0_orchestration.ScanOrchestrator.internal._send_and_await import send_and_await
from srcs._0_orchestration.Session import Session
from srcs._1_domain.PiMessages.PiMessages import PiResponseKind
from srcs._1_domain.Scan.Scan import ScanState
from srcs._3_interface.IPiTransport import IPiTransport
from srcs._3_interface.IScanStore import IScanStore


async def _run_step(
    state: ScanState,
    step: int,
    pair: tuple[str, str],
    pi: IPiTransport,
    session: Session,
    store: IScanStore,
) -> None:
    first, second = (pair[0], pair[1]) if step % 2 == 1 else (pair[1], pair[0])
    await _shoot_at(state, step, first, pi, session, store)
    resp = await send_and_await(session, pi, f"POSITION_{second}", 10.0)
    if resp.kind == PiResponseKind.FAIL:
        raise RuntimeError(f"POSITION_{second} failed")
    await _shoot_at(state, step, second, pi, session, store)
