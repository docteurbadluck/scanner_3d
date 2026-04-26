from __future__ import annotations

from typing import Any


def _parse_pico_status_message(data: dict[str, Any]) -> PiResponse:
    from srcs._1_domain.PiMessages.PiMessages import PiResponse, PiResponseKind
    state = str(data.get("state", ""))
    return PiResponse(kind=PiResponseKind.PICO_STATUS, payload=state)
