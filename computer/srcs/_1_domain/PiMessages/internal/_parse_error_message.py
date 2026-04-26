from __future__ import annotations

from typing import Any


def _parse_error_message(data: dict[str, Any]) -> PiResponse:
    from srcs._1_domain.PiMessages.PiMessages import PiResponse, PiResponseKind
    reason = str(data.get("reason", ""))
    return PiResponse(kind=PiResponseKind.INVALID_CMD, payload=reason)
