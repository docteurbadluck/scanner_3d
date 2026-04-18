from __future__ import annotations

import pytest
import requests

HTTP_URL = "http://localhost:8080"

PAGES: list[str] = ["/", "/index.html", "/metrics/", "/status/", "/productivity/"]


@pytest.mark.parametrize("path", PAGES)
def test_page_returns_200(path: str) -> None:
    response = requests.get(HTTP_URL + path, timeout=5)
    assert response.status_code == 200


def test_missing_page_returns_404() -> None:
    response = requests.get(HTTP_URL + "/not_found.html", timeout=5)
    assert response.status_code == 404


def test_data_json_returns_200() -> None:
    response = requests.get(HTTP_URL + "/data/productivity.json", timeout=5)
    assert response.status_code == 200
