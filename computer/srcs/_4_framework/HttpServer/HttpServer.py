from __future__ import annotations

import threading
from http.server import HTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
from typing import Any, Callable

from srcs._4_framework.UploadHandler import handle_upload


def build_http_handler(
    www_dir: Path,
    pending_upload_provider: Callable[[], tuple[str, str, str] | None] | None = None,
) -> type[SimpleHTTPRequestHandler]:
    class Handler(SimpleHTTPRequestHandler):
        def __init__(self, *args: Any, **kwargs: Any) -> None:
            super().__init__(*args, directory=str(www_dir), **kwargs)

        def log_message(self, *_args: Any) -> None:
            pass

        def end_headers(self) -> None:
            if self.path.endswith(".js") or self.path.endswith(".css"):
                self.send_header("Cache-Control", "no-store")
            super().end_headers()

        def do_POST(self) -> None:
            if self.path != "/upload":
                self.send_error(404)
                return
            handle_upload(self, www_dir, pending_upload_provider)

    return Handler


def start_http_server(
    port: int,
    www_dir: Path,
    pending_upload_provider: Callable[[], tuple[str, str, str] | None] | None = None,
) -> HTTPServer:
    handler = build_http_handler(www_dir, pending_upload_provider)
    server = HTTPServer(("", port), handler)
    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()
    return server
