#pragma once
#include <array>
#include <vector>
#include <string>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include "Client.hpp"
#include <sys/epoll.h>
#include <unordered_map>

constexpr static const std::array<uint32_t, 6> eventTypes{EPOLLIN, EPOLLOUT, EPOLLRDHUP, EPOLLPRI, EPOLLERR, EPOLLRDHUP};

class Server {
	private:
		std::unordered_map<int, class Client> _clients{};
		std::vector<epoll_event> _events{};
		const int _fd;
		const int _max_events = 10;
		std::string _password;
	public:
		Server(std::string passwd = "");
		virtual ~Server();
		void addClient(const int fd);
		void removeClient(const int fd);
		void poll(int tout = -1);
		const std::unordered_map<int, class Client>& getClients() const;
		int getServerFd() const;

};
