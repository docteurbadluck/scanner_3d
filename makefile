server:
	cd computer/webserver/www && python3 -m http.server 8080

server-background:
	cd computer/webserver/www && python3 -m http.server 8080 &

server-kill:
	# Trouve le PID du serveur Python et le tue
	kill $(shell pgrep -f 'python3 -m http.server 8080')

.PHONY: server server-background server-test server-kill