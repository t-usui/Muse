#include "network.h"

namespace muse{
	Network::Network(){

	}


	Network::~Network(){

	}


	void Network::CreateSocket(char *mode, char *hostname, char *port){
		int ret;
		int mode_number;

		struct addrinfo hints;
		struct addrinfo *ai;
		struct addrinfo *res;

		if(strncmp(mode, "server", 6) == 0){
			mode_number = 0;
		}else if(strncmp(mode, "client", 6) == 0){
			mode_number = 1;
		}else{
			std::cerr << "Error: no mode is specified" << std::endl;
		}

		memset(&hints, 0x00, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = AF_UNSPEC;

		ret = getaddrinfo(hostname, port, &hints, &res);
		if(ret != 0){
			std::cerr << "Error: " << ret << std::endl;
			exit(EXIT_FAILURE);
		}

		for(ai=res;ai!=NULL;ai=ai->ai_next){
			this->sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
			if(this->sock < 0){
				continue;
			}

			if(mode_number == 1){
				ret = connect(this->sock, ai->ai_addr, ai->ai_addrlen);
				if(ret != 0){
					CloseSocket(this->sock);
					continue;
				}
			}

			break;
		}

		freeaddrinfo(res);

		if(this->sock < 0){
			perror("socket");
			exit(EXIT_FAILURE);
		}

		return;
	}


	void Network::CloseSocket(int sock){
		close(sock);

		return;
	}
	

	Server::Server(){

	}


	Server::~Server(){

	}


	void Server::BindSocket(char *port){
		int ret;
		int value;

		struct sockaddr_in server;

		server.sin_family	= AF_UNSPEC;
		server.sin_port		= htons(atoi(port));
		server.sin_addr.s_addr	= INADDR_ANY;

		value = 1;
		setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&value, sizeof(value));

		ret = bind(this->sock, (struct sockaddr *)&server, sizeof(server));
		if(ret != 0){
			perror("bind");
			exit(EXIT_FAILURE);
		}

		return;
	}


	void Server::ListenPort(){
		const int kMaxQueue = 10;

		int ret;

		ret = listen(this->sock, kMaxQueue);
		if(ret != 0){
			perror("listen");
			exit(EXIT_FAILURE);
		}

		return;
	}


	void Server::SetupServer(char *hostname, char *port){
		int len;

		char mode[] = "server";

		CreateSocket(mode, hostname, port);
		BindSocket(port);
		ListenPort();

		return;
	}


	void Server::AcceptClient(){
		int ret;
		int len;

		struct sockaddr_in client;

		len = sizeof(client);

		this->accept_sock = accept(this->sock, (struct sockaddr *)&client, (socklen_t *)&len);
		if(this->accept_sock < 0){
			perror("accept");
			exit(EXIT_FAILURE);
		}

		std::cout << "Accepted connection: Source IP address: " << inet_ntoa(client.sin_addr) << "Port: " << ntohs(client.sin_port) << std::endl;

		return;
	}


	void Server::CloseServer(){
		CloseSocket(this->sock);

		return;
	}


	Client::Client(){

	}


	Client::~Client(){

	}


	void Client::ConnectServer(char *hostname, char *port){
		char mode[] = "client";

		CreateSocket(mode, hostname, port);

		return;
	}


	void Client::DisconnectServer(){
		CloseSocket(this->sock);

		return;
	}
}

