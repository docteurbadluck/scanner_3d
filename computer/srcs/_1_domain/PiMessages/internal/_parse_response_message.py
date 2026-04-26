from __future__ import annotations

from typing import Any


def _parse_response_message(data: dict[str, Any]) -> PiResponse:
    from srcs._1_domain.PiMessages.PiMessages import PiResponse, PiResponseKind
    kind_raw = str(data.get("kind", ""))
    command  = str(data.get("command", ""))
    ms: int | None = int(data["ms"]) if "ms" in data else None
    return PiResponse(
        kind=PiResponseKind.from_raw(kind_raw),
        payload=kind_raw,
        command=command,
        ms=ms,
    )
