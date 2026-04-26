from __future__ import annotations

from typing import Any

from srcs._1_domain.PiMessages.internal._parse_error_message import _parse_error_message
from srcs._1_domain.PiMessages.internal._parse_pico_status_message import _parse_pico_status_message
from srcs._1_domain.PiMessages.internal._parse_response_message import _parse_response_message
from srcs._1_domain.PiMessages.internal._parse_state_message import _parse_state_message


def _parse_json_dict(data: dict[str, Any]) -> PiResponse | None:
    from srcs._1_domain.PiMessages.PiMessages import PiResponse
    msg_type = str(data.get("type", ""))
    if msg_type == "state":
        return _parse_state_message(data)
    if msg_type == "pico_status":
        return _parse_pico_status_message(data)
    if msg_type == "response":
        return _parse_response_message(data)
    if msg_type == "error":
        return _parse_error_message(data)
    return None
