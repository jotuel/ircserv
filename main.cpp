#include "Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <csignal>

int main(int argc, char *argv[]) {
	Server *irc;
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
	irc->poll();
	close(sock);
	delete irc;
}
