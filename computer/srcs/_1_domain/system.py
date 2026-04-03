from __future__ import annotations

from dataclasses import dataclass, field
from enum import Enum


class State(Enum):
    INITIALIZATION = "INITIALIZATION"
    READY          = "READY"
    PROCESSING     = "PROCESSING"
    SENDING        = "SENDING"


@dataclass(frozen=True)
class Commands:
    valid: tuple[str, ...] = field(default=(
        "PING",
        "PONG",
        "GET_STATUS",
        "GET_PICO_STATUS",
        "START_CAPTURE",
    ))


class System:
    def __init__(self) -> None:
        self._current_state:      State    = State.INITIALIZATION
        self._commands:           Commands = Commands()
        self._command_to_execute: str      = ""

    def get_state(self) -> State:
        return self._current_state

    def get_state_string(self) -> str:
        return self._current_state.value

    def get_commands(self) -> Commands:
        return self._commands

    def set_command_to_execute(self, command: str) -> None:
        self._command_to_execute = command

    def get_command_to_execute(self) -> str:
        return self._command_to_execute

    def ready(self) -> None:
        self._current_state = State.READY

    def processing(self) -> None:
        self._current_state = State.PROCESSING

    def sending(self) -> None:
        self._current_state = State.SENDING

    def reset(self) -> None:
        self._current_state      = State.INITIALIZATION
        self._command_to_execute = ""
