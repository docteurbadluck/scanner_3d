from __future__ import annotations

from typing import Callable

from srcs._3_interface.IPiTransport import IPiTransport


def _require_active_pi(
    pi: IPiTransport | None,
    pi_provider: Callable[[], IPiTransport | None] | None,
) -> IPiTransport:
    active_pi = pi_provider() if pi_provider is not None else pi
    if active_pi is None:
        raise ValueError("Pi unavailable")
    return active_pi
