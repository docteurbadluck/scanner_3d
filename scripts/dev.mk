PYTHON ?= python3

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

.PHONY: server-test server-typecheck pico-test pico-coverage pi-test pi-coverage complexity fitness doxygen doxygen-clean
