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
		--cov=server --cov=srcs \
		--cov-report=json:www/data/coverage.json \
		--cov-report=json:www/data/computer_srcs_coverage.json \
		--cov-report=html:www/coverage/computer

server-typecheck:
	cd computer && $(PYTHON) -m mypy

pico-test:
	make test -C ./pico/motor

pico-coverage:
	make coverage -C ./pico/motor
	python3 metrics/pico_coverage.py

pi-test:
	make test -C ./pi

pi-coverage:
	make coverage -C ./pi
	python3 metrics/pi_coverage.py

complexity:
	$(PYTHON) metrics/collect_complexity.py

fitness:
	$(PYTHON) metrics/fitness.py

doxygen:
	doxygen docs/Doxyfile
	@echo "Doc generated in computer/www/docs/html/index.html"

doxygen-clean:
	rm -rf computer/www/docs

.PHONY: install server-background server-stop server-test server-typecheck pico-test pico-coverage pi-test pi-coverage complexity fitness doxygen doxygen-clean

include scripts/remote.mk
