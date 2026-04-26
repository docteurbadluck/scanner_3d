from __future__ import annotations

from pathlib import Path
from unittest.mock import MagicMock

from srcs._4_framework.UploadHandler import PHOTOS_DIR, handle_upload
from srcs._4_framework.UploadHandler.internal._save_photo import _save_photo


def test_save_photo_creates_directory(tmp_path: Path) -> None:
    _save_photo(tmp_path / "photos", b"data")
    assert (tmp_path / "photos").exists()


def test_save_photo_writes_jpg(tmp_path: Path) -> None:
    photos_dir = tmp_path / "photos"
    _save_photo(photos_dir, b"\xff\xd8\xff")
    files = list(photos_dir.glob("*.jpg"))
    assert len(files) == 1
    assert files[0].read_bytes() == b"\xff\xd8\xff"


def test_handle_upload_saves_body(tmp_path: Path) -> None:
    handler = MagicMock()
    handler.headers.get.return_value = "3"
    handler.rfile.read.return_value = b"abc"
    handle_upload(handler, tmp_path)
    assert len(list((tmp_path / PHOTOS_DIR).glob("*.jpg"))) == 1


def test_handle_upload_sends_200(tmp_path: Path) -> None:
    handler = MagicMock()
    handler.headers.get.return_value = "0"
    handler.rfile.read.return_value = b""
    handle_upload(handler, tmp_path)
    handler.send_response.assert_called_once_with(200)
    handler.end_headers.assert_called_once()
