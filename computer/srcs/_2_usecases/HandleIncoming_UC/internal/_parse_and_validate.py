from __future__ import annotations

import json
from typing import Any

from srcs._1_domain.Messages import ErrorMessage
from srcs._3_interface.IConnection import IConnection


async def _parse_and_validate(connection: IConnection, raw: str) -> dict[str, Any] | None:
    try:
        decoded: Any = json.loads(raw)
    except json.JSONDecodeError:
        await connection.send(ErrorMessage("Invalid JSON message").to_json())
        return None
    if not isinstance(decoded, dict):
        await connection.send(ErrorMessage("Invalid message shape").to_json())
        return None
    return decoded
