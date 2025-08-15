#include "Server.hpp"
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <csignal>

Server *irc;

void clientWrite(int fd) {
	(void)fd;
}

void clientDisconnect(int fd) {
	std::cout << "Client with socket" << fd << " disconnected" << std::endl;
}

void acceptClient(int socket) {
	int fd;
	struct sockaddr_in remote{};
	socklen_t remoteLen{};

	fd = accept(socket, (struct sockaddr*) &remote, &remoteLen);

	if (fd > 0) {
		std::cout << "A client connected with" << fd << "number connected" << std::endl;

		irc->addClient(fd);
		irc->registerHandler(fd, EPOLLIN, clientWrite);
		irc->registerHandler(fd, EPOLLRDHUP | EPOLLHUP, clientDisconnect);
	} else {
		throw std::runtime_error("Failed creating a new TCP connection to client");
	}
}

int main(int argc, char *argv[]) {
	struct sockaddr_in sa_bindy{};
	sa_bindy.sin_family = AF_INET;

	if (argc != 2 && argc != 3) {
		std::cerr << "Usage ./ircserv [port] <password>" << std::endl;
		return 1;
	} else if (argc == 3) {
		irc = new Server(argv[2]);
	} else {
		irc = new Server;
	}

	int port;

	try {
		port = std::stoi(argv[1]);
	} catch (...) {
		std::cerr << "Faulty port" << std::endl;
		return 1;
	}

	sa_bindy.sin_port = htons(port);
	sa_bindy.sin_addr.s_addr = inet_addr("127.0.0.1");

	auto sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		delete irc;
		std::cerr << "Socket creation failed" << std::endl;
		return 1;
	}

	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));


	if (bind(sock, (struct sockaddr *)&sa_bindy, sizeof(sa_bindy))) {
		delete irc;
		close(sock);
		std::cerr << "Binding failed" << std::endl;
		return 1;
	}

	if (listen(sock, 5)) {
		delete irc;
		close(sock);
		std::cerr << "Listening on the port failed" << std::endl;
		return 1;
	}

	irc->addClient(sock);
	irc->registerHandler(sock, EPOLLIN | EPOLLOUT, [](int socket) { acceptClient(socket); });
	while (true)
		irc->poll();
	close(sock);
	delete irc;
}
