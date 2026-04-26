from __future__ import annotations

from typing import TYPE_CHECKING, Any

from srcs._1_domain.PiMessages.internal._normalize_state import _normalize_state

if TYPE_CHECKING:
    from srcs._1_domain.PiMessages.PiMessages import PiResponse, PiResponseKind


def _parse_state_message(data: dict[str, Any]) -> PiResponse:
    from srcs._1_domain.PiMessages.PiMessages import PiResponse, PiResponseKind
    state = _normalize_state(str(data.get("state", "")))
    return PiResponse(kind=PiResponseKind.STATE, payload=state)
