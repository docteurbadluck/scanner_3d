from __future__ import annotations

import time

from srcs._0_orchestration.RunScan.internal._broadcast_scan_state import _broadcast_scan_state
from srcs._0_orchestration.RunScan.internal._run_position import _run_position
from srcs._0_orchestration.ScanOrchestrator.internal._send_and_await import send_and_await
from srcs._0_orchestration.Session import Session
from srcs._1_domain.Scan.Scan import (
    POSITIONS, ScanState, ScanStatus,
    is_valid_scan_name, scan_state_from_dict, scan_state_to_dict,
)
from srcs._3_interface.IPiTransport import IPiTransport
from srcs._3_interface.IScanStore import IScanStore


class RunScan:
    def __init__(self, session: Session, store: IScanStore) -> None:
        self._session = session
        self._store = store

    async def start(self, name: str, pi: IPiTransport) -> None:
        if not is_valid_scan_name(name):
            raise ValueError(f"Invalid scan name: {name!r}")
        if self._store.load(name) is not None:
            raise ValueError(f"Scan already exists: {name!r}")
        self._store.ensure_folders(name)
        await self._run(ScanState(name=name), pi)

    async def resume(self, name: str, pi: IPiTransport) -> None:
        data = self._store.load(name)
        if data is None:
            raise ValueError(f"Scan not found: {name!r}")
        state = scan_state_from_dict(data)
        state.status = ScanStatus.IN_PROGRESS
        await self._run(state, pi)

    async def _run(self, state: ScanState, pi: IPiTransport) -> None:
        started = time.monotonic()
        try:
            await send_and_await(self._session, pi, "INITIAL_POS", 10.0)
            start_pos = state.position_index
            for i in range(start_pos, len(POSITIONS)):
                state.position_index = i
                if i > start_pos:
                    state.shot_index = 0
                await _broadcast_scan_state(self._session, state)
                await _run_position(state, pi, self._session, self._store)
            await self._finish_ok(state, pi, round(time.monotonic() - started, 1))
        except Exception as exc:
            await self._handle_error(state, exc)

    async def _finish_ok(self, state: ScanState, pi: IPiTransport, duration_s: float) -> None:
        state.status = ScanStatus.DONE
        self._store.save(state.name, scan_state_to_dict(state))
        self._store.update_hardware_metrics(state.name, duration_s)
        await _broadcast_scan_state(self._session, state)
        try:
            await send_and_await(self._session, pi, "INITIAL_POS", 10.0)
        except Exception:
            pass

    async def _handle_error(self, state: ScanState, exc: Exception) -> None:
        state.status = ScanStatus.FAILED
        state.error = str(exc)
        self._store.save(state.name, scan_state_to_dict(state))
        await _broadcast_scan_state(self._session, state)
