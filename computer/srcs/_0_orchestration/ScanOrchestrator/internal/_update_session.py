from __future__ import annotations

from srcs._1_domain.PiMessages import PiResponse, PiResponseKind
from srcs._1_domain.System import State
from srcs._0_orchestration.Session import Session


async def _update_session(session: Session, parsed: PiResponse) -> None:
    if parsed.kind == PiResponseKind.STATE:
        try:
            session.set_state(State(parsed.payload))
        except ValueError:
            pass
    if parsed.kind == PiResponseKind.PICO_STATUS:
        session.set_pico_state(parsed.payload)
