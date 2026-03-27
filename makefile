server:
		make -C ./computer/webserver/webserv && ./computer/webserver/webserv/run.sh

server-background:
		make -C ./computer/webserver/webserv && ./computer/webserver/webserv/run.sh &

server-test:
		make -C ./computer/webserver/testeur_webserver && ./computer/webserver/testeur_webserver/testeur_webserver

server-kill:
	@pkill -f "webserv" || true

.PHONY: server server-background server-test server-kill