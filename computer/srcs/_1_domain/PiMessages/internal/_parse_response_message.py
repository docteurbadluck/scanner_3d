from __future__ import annotations

from typing import Any


def _parse_response_message(data: dict[str, Any]) -> PiResponse:
    from srcs._1_domain.PiMessages.PiMessages import PiResponse, PiResponseKind
    known    = {"type", "kind", "command", "ms"}
    kind_raw = str(data.get("kind", ""))
    command  = str(data.get("command", ""))
    ms: int | None = int(data["ms"]) if "ms" in data else None
    extras   = {k: v for k, v in data.items() if k not in known}
    return PiResponse(
        kind=PiResponseKind.from_raw(kind_raw),
        payload=kind_raw,
        command=command,
        ms=ms,
        extras=extras,
    )
