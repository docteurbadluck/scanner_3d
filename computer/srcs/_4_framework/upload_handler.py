from __future__ import annotations

from datetime import datetime, timezone
from http.server import SimpleHTTPRequestHandler
from pathlib import Path

PHOTOS_DIR = "photos"


def _save_photo(photos_dir: Path, data: bytes) -> None:
    photos_dir.mkdir(exist_ok=True)
    timestamp: str = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%S%f")
    (photos_dir / f"{timestamp}.jpg").write_bytes(data)


def handle_upload(handler: SimpleHTTPRequestHandler, www_dir: Path) -> None:
    length: int = int(handler.headers.get("Content-Length", 0))
    data: bytes = handler.rfile.read(length)
    _save_photo(www_dir / PHOTOS_DIR, data)
    handler.send_response(200)
    handler.end_headers()
