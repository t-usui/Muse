#include "server.h"

using namespace sift;

int main(int argc, char **argv){
	int len;

	char image_name[32];
	char buf[64];

	Database *db;
	Server *srv;
	Classifier *cls;

	db = new Database;
	cls = new Classifier;

	std::cout << "Loading data on memory... ";
	db->ConnectDatabase();
	cls->descriptors = db->LoadOnMemory();
	std::cout << "Done." << std::endl;

	srv = new Server;

	std::cout << "Setting up server... ";
	srv->SetupServer(argv[1], argv[2]);
	std::cout << "Done." << std::endl;

	std::cout << "Now, waiting for client connection... " << std::endl;


	while(1){ 
		srv->AcceptClient();

		len = read(srv->accept_sock, image_name, sizeof(image_name));
		cls->ExecuteMatchingOnMemory(image_name);

		memset(buf, 0x00, sizeof(buf));
		len = read(srv->accept_sock, buf, sizeof(buf));
		cls->result = atoi(buf);

		memset(buf, 0x00, sizeof(buf));
		len = read(srv->accept_sock, buf, sizeof(buf));
		cls->reliability = atof(buf);

		memset(buf, 0x00, sizeof(buf));
		snprintf(buf, "%d %f", cls->result, cls->reliability, sizeof(buf));
		write(srv->accept_sock, buf, strlen(buf));
		if(len < 1){
			perror("write"); 
			break; 
		}
		srv->CloseSocket(srv->accept_sock);
	}

	srv->CloseServer();

	delete db;
	delete cls;
	delete srv;

	return 0;
}
	
