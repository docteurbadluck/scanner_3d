from __future__ import annotations

from srcs._0_orchestration.ScanOrchestrator.internal._notify_browsers import _notify_browsers
from srcs._0_orchestration.Session import Session
from srcs._1_domain.Messages.Messages import ScanStateMessage
from srcs._1_domain.Scan.Scan import ScanState


async def _broadcast_scan_state(session: Session, state: ScanState) -> None:
    await _notify_browsers(session, ScanStateMessage(state).to_json())
