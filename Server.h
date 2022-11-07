#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <map>

#define ALL_IPV4_LOCAL_ADRESSES "0.0.0.0"
# define BUF_SIZE	4096

void warning(const std::string& mes) {
	std::cerr << "\033[1;37mWarning:\033[0m " << mes << std::endl;
}

void error(const std::string& mes) {
	std::cerr << "\033[1;31mError:\033[0m " << mes << std::endl;
}

class Server;

class ServerDestroyer {
private:
	Server* instance_;
public:
	ServerDestroyer();
	~ServerDestroyer();
	void initialize(Server* p);
};

class Server {
private:

	struct Client {

		char	buf_read_[BUF_SIZE + 1];
		char	buf_write_[BUF_SIZE + 1];

		Client();
		void read();
		void write();

		struct ClientException: public std::runtime_error {
			ClientException(const std::string& mes); };

	};

	static Server*			instance_;
	static ServerDestroyer	destroyer_;

	int	 					port_;
	std::string				password_;
	int	 					serv_sfd_;
	std::map<int, Client>	clients_;

	Server();
	Server(const Server& other);
	Server& operator=(const Server& other);
	Server(int port, const std::string& password);

	void emergency_shutdown();

public:

	struct ServerException : public std::runtime_error {
		ServerException(const std::string& mes); };

	~Server();
	static Server& getInstance(const int& port, const std::string& password);

    void start();
	void accept();
    void stop();
};


#endif //SERVER_H