#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include "Server.h"

#define MAX_OFFICIAL_IRC_PORT 6669
#define MIN_OFFICIAL_IRC_PORT 6665
#define MIN_DYNAMIC_PORT 49152
#define MAX_DYNAMIC_PORT 65535

int validate_port(const std::string& p) {
	int port = std::stoi(p);
	if (!((port >= MIN_OFFICIAL_IRC_PORT && port <= MAX_OFFICIAL_IRC_PORT)
		  || (port >= MIN_DYNAMIC_PORT && port <= MAX_DYNAMIC_PORT)))
		throw std::out_of_range("Choose port in range: 6665-6669 (official IRC ports), 49152-65535 (dynamic client-server ports)");
	return port;
}

int main(int argc, char** argv) {

	if (argc != 3) {
		error("Usage: ./ircserver port password"); return 1; }

	try {

		Server* server = &Server::getInstance(validate_port(argv[1]), argv[2]);
		server->start();

	}
	catch (std::invalid_argument) {
		error("You must specify a port as an integer"); return 1; }
	catch (std::out_of_range& e) {
		error(e.what()); return 1; }
	catch (Server::ServerException& e) {
		error(e.what()); return 1; }




/*
	// Bind the ip address and port to a socket


	// Tell Winsock the socket is for listening


	// Wait for a connection
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);

	int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];      // Client's remote name
	char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on

	memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " connected on port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
	}

	// Close listening socket
	close(listening);

	// While loop: accept and echo message back to client
	char buf[4096];

	while (true)
	{
		memset(buf, 0, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == -1)
		{
			std::cerr << "Error in recv(). Quitting" << std::endl;
			break;
		}

		if (bytesReceived == 0)
		{
			std::cout << "Client disconnected " << std::endl;
			break;
		}

		std::cout << std::string(buf, 0, bytesReceived) << std::endl;

		// Echo message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);
	}

	// Close the socket
	close(clientSocket);

	return 0;*/
}
