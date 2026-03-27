#include "EpollServerRunner.hpp"

EpollServerRunner::EpollServerRunner(const t_server_rules &server_rules, SessionHandlerUC &session_handler, RequestHandlerUC &request_handler) : server_rules(server_rules), session_handler(session_handler), request_handler(request_handler), mode_switcher(NULL), epoll_fd(-1)
{
	stream_in.set_max_body_size(server_rules.max_size_request_body);
}

EpollServerRunner::~EpollServerRunner()
{
	for (std::map<int, IClient*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		delete it->second;
	}
	this->clients.erase(this->clients.begin(), this->clients.end());
	if (this->mode_switcher != NULL)
		delete this->mode_switcher;
	if (this->epoll_fd != -1)
		close(this->epoll_fd);
}
