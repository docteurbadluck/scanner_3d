from __future__ import annotations

from srcs._0_orchestration.Session import Session


async def _notify_browsers(session: Session, msg: str) -> None:
    for browser in session.browsers():
        await browser.send(msg)
