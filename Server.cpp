#include "Server.h"
#include <iostream>


/* -------------------------------------------------------------------------------------------------- ServerDestroyer */

ServerDestroyer::ServerDestroyer() : instance_(NULL) {}

ServerDestroyer::~ServerDestroyer() {
	delete instance_; }

void ServerDestroyer::initialize(Server* p) {
	instance_ = p; }


/* ----------------------------------------------------------------------------------------------------------- Client */

Server::Client::Client() {}

/* ----------------------------------------------------------------------------------------------------------- Server */

Server* Server::instance_ = NULL;
ServerDestroyer Server::destroyer_;

Server::Server(int port, const std::string& password) : port_(port), password_(password) {
	struct protoent	*pe = getprotobyname("tcp");
	if (pe == NULL)
		throw ServerException("Couldn't get a protocol");

//	serv_sfd_ = socket(AF_INET, SOCK_STREAM, 6);
	serv_sfd_ = socket(AF_INET, SOCK_STREAM, pe->p_proto);
	if (serv_sfd_ == -1)
		throw ServerException("Couldn't create a server socket");

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port_);
//	hint.sin_addr.s_addr = inet_addr(ALL_IPV4_LOCAL_ADRESSES);
	hint.sin_addr.s_addr = INADDR_ANY;

	if (bind(serv_sfd_, (sockaddr*)&hint, sizeof(hint)) == -1)
		throw ServerException("Couldn't bind a server socket");
}

Server& Server::getInstance(const int& port, const std::string& password) {
	if (!instance_) {
		instance_ = new Server(port, password);
		destroyer_.initialize( instance_);
	}
	else {
		if (port == instance_->port_ && password == instance_->password_)
			warning("Server has been already set up with the same parameters");
		else
			warning("Server has been already set up with the other parameters");
	}
	return *instance_;
}

void Server::start() {
	if (listen(serv_sfd_, SOMAXCONN) == -1)
		throw ServerException("Couldn't listen on a server socket");

}

void Server::accept() {
	int			cs;
	sockaddr_in	csin;

	socklen_t csin_len = sizeof(sockaddr_in);
	cs = ::accept(serv_sfd_, (sockaddr*)&csin, &csin_len);
	if (cs == -1)
		throw Client::ClientException("Couldn't accept a connection");
	std::cout << "New client #" << cs
			  << " from " << inet_ntoa(csin.sin_addr)
			  << ":" << ntohs(csin.sin_port) << std::endl;
	clients_[cs] = Client();
}

Server::~Server() {}

Server::ServerException::ServerException(const std::string& mes) : std::runtime_error(mes) {}