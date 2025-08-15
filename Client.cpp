#include "Client.hpp"
#include <stdexcept>
#include <sys/epoll.h>

Client::Client(int fd) : _fd(fd) {

}

void Client::setHandler(uint32_t eventType, std::function<void(int)> handler) {
	switch (eventType) {
		case EPOLLIN:
			IN = std::move(handler);
			break;
		case EPOLLOUT:
			OUT = std::move(handler);
			break;
		case EPOLLRDHUP:
			RDHUP = std::move(handler);
			break;
		case EPOLLPRI:
			PRI = std::move(handler);
			break;
		case EPOLLERR:
			ERR = std::move(handler);
			break;
		case EPOLLHUP:
			HUP = std::move(handler);
			break;
		default:
			break;
	}
}

std::function<void(int)>& Client::getHandler(uint32_t eventType) {
	switch (eventType) {
		case EPOLLIN:
			return IN;
		case EPOLLOUT:
			return OUT;
		case EPOLLRDHUP:
			return RDHUP;
		case EPOLLPRI:
			return PRI;
		case EPOLLERR:
			return ERR;
		case EPOLLHUP:
			return HUP;
		default:
			throw std::runtime_error("Client::getHandler: Error; invalid eventType");
	}
}
