#include "Server.hpp"


Server::Server(std::string passwd) :  _fd(epoll_create1(0)), _password(passwd) {
	if(_fd == -1)
		throw std::runtime_error("Server::Server: ERROR - Failed to create epoll file");

	_events.reserve(_max_events * sizeof(epoll_event));
}

Server::~Server() { close(_fd); }

void Server::addClient(const int fd) {
	_clients.try_emplace(fd, fd);
}

void Server::removeClient(const int fd) {
	if(!_clients.empty())
		_clients.erase(fd);
}

void Server::poll(int tout) {
	int nbrEvents = epoll_wait(_fd, &_events[0], _max_events, tout);

	for (int idx = 0; idx < _max_events; idx++) {
		uint32_t events = _events[idx].events;
	 	int fd = _events[idx].data.fd;
		for (uint32_t eventType : eventTypes) {
			if (_clients.count(fd) == 0)
				return ;
			_clients.at(fd).getHandler(eventType);
		}

	if (events & (EPOLLRDHUP & EPOLLHUP))
		removeClient(fd);
	}
}
