from __future__ import annotations

from http.server import SimpleHTTPRequestHandler
from pathlib import Path

from srcs._4_framework.UploadHandler.internal._save_photo import _save_photo

PHOTOS_DIR = "photos"


def handle_upload(handler: SimpleHTTPRequestHandler, www_dir: Path) -> None:
    length: int = int(handler.headers.get("Content-Length", 0))
    data: bytes = handler.rfile.read(length)
    _save_photo(www_dir / PHOTOS_DIR, data)
    handler.send_response(200)
    handler.end_headers()
