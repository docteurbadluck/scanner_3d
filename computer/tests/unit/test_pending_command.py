from __future__ import annotations

import asyncio

import pytest

from srcs._0_orchestration.Session.internal._pending_command import PendingCommand
from srcs._1_domain.PiMessages.PiMessages import PiResponse, PiResponseKind


def _done_response(command: str) -> PiResponse:
    return PiResponse(kind=PiResponseKind.DONE, payload="", command=command)


def _fail_response(command: str) -> PiResponse:
    return PiResponse(kind=PiResponseKind.FAIL, payload="", command=command)


async def test_arm_and_resolve_matching_command() -> None:
    pc = PendingCommand()
    future = pc.arm("TAKE_PHOTO")
    resolved = pc.resolve_if_match(_done_response("TAKE_PHOTO"))
    assert resolved is True
    result = await asyncio.wait_for(future, timeout=1.0)
    assert result.kind == PiResponseKind.DONE


async def test_resolve_wrong_command_returns_false() -> None:
    pc = PendingCommand()
    pc.arm("TAKE_PHOTO")
    assert pc.resolve_if_match(_done_response("PLATE_NEXT")) is False


async def test_double_resolve_is_noop() -> None:
    pc = PendingCommand()
    future = pc.arm("TAKE_PHOTO")
    pc.resolve_if_match(_done_response("TAKE_PHOTO"))
    second = pc.resolve_if_match(_done_response("TAKE_PHOTO"))
    assert second is False
    await asyncio.wait_for(future, timeout=1.0)


async def test_fail_kind_resolves_future() -> None:
    pc = PendingCommand()
    future = pc.arm("TAKE_PHOTO")
    pc.resolve_if_match(_fail_response("TAKE_PHOTO"))
    result = await asyncio.wait_for(future, timeout=1.0)
    assert result.kind == PiResponseKind.FAIL


async def test_no_arm_resolve_is_noop() -> None:
    pc = PendingCommand()
    assert pc.resolve_if_match(_done_response("TAKE_PHOTO")) is False
