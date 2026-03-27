server:
	cd computer/www && python3 -m http.server 8080

server-background:
	cd computer/www && python3 -m http.server 8080 &

server-kill:
	# Trouve le PID du serveur Python et le tue
	kill $(shell pgrep -f 'python3 -m http.server 8080')

server-test:
	python3 computer/ping_pages.py 

.PHONY: server server-background server-test server-kill