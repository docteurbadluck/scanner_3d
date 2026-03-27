PYTHON   = python3
PID_FILE = .server.pid

install:
	curl -s https://bootstrap.pypa.io/get-pip.py | $(PYTHON) - --user --break-system-packages
	$(HOME)/.local/bin/pip install -r computer/requirements.txt --break-system-packages

server-background:
	$(PYTHON) computer/server.py & echo $$! > $(PID_FILE)

server-stop:
	fuser -k 8080/tcp 8081/tcp 2>/dev/null; rm -f $(PID_FILE)

server-test:
	cd computer && $(PYTHON) -m pytest tests/ -v \
		--cov=server --cov-report=json:www/data/coverage.json

.PHONY: install server-background server-stop server-test
