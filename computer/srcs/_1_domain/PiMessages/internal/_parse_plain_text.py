from __future__ import annotations

from srcs._1_domain.PiMessages.internal._normalize_state import _KNOWN_STATES, _normalize_state


def _parse_plain_text(stripped: str) -> PiResponse:
    from srcs._1_domain.PiMessages.PiMessages import PiResponse, PiResponseKind
    normalized = _normalize_state(stripped)
    if stripped in _KNOWN_STATES or normalized != stripped:
        return PiResponse(kind=PiResponseKind.STATE, payload=normalized)
    kind = PiResponseKind.from_raw(stripped)
    return PiResponse(kind=kind, payload=stripped)
