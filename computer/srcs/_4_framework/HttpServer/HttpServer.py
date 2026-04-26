from __future__ import annotations

import threading
from http.server import HTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
from typing import Any

from srcs._4_framework.UploadHandler import handle_upload


def build_http_handler(www_dir: Path) -> type[SimpleHTTPRequestHandler]:
    class Handler(SimpleHTTPRequestHandler):
        def __init__(self, *args: Any, **kwargs: Any) -> None:
            super().__init__(*args, directory=str(www_dir), **kwargs)

        def log_message(self, *_args: Any) -> None:
            pass

        def do_POST(self) -> None:
            if self.path != "/upload":
                self.send_error(404)
                return
            handle_upload(self, www_dir)

    return Handler


def start_http_server(port: int, www_dir: Path) -> HTTPServer:
    handler: type[SimpleHTTPRequestHandler] = build_http_handler(www_dir)
    server = HTTPServer(("", port), handler)
    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()
    return server
