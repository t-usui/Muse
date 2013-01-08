#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace muse{
	class Network{
		private:

		public:
			int sock;
			int accept_sock;

			Network();
			~Network();

			void CreateSocket(char *mode, char *hostname, char *port);
			void CloseSocket(int sock);
	};


	class Server : public Network{
		private:
			void BindSocket(char *port);
			void ListenPort();

		public:
			Server();
			~Server();

			void SetupServer(char *hostname, char *port);
			void AcceptClient();
			void CloseServer();
	};


	class Client : public Network{
		private:

		public:
			Client();
			~Client();

			void ConnectServer(char *hostname, char *port);
			void DisconnectServer();
	};
}

#endif /* __NETWORK_H__ */
