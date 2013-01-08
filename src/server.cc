#include "server.h"

int main(int argc, char **argv){
	int ret;
	int len;

	char image_name[32];
	char buf[64];

	muse::Database *db;
	muse::Server *server;
	muse::Classifier *classifier;

	if(argc != 3){
		std::cerr << "Usage: server <Hostname> <Port>" << std::endl;
		exit(EXIT_FAILURE);
	}

	db = new muse::Database;
	classifier = new muse::Classifier;
	server = new muse::Server;

	std::cout << "Loading data on memory... ";
	db->ConnectDatabase();
	classifier->descriptors = db->LoadOnMemory();
	std::cout << "Done." << std::endl;
	delete db;

	std::cout << "Setting up server... ";
	server->SetupServer(argv[1], argv[2]);
	std::cout << "Done." << std::endl;

	std::cout << "Now, waiting for client connection... " << std::endl;

	while(1){ 
		server->AcceptClient();

		len = read(server->accept_sock, image_name, sizeof(image_name));
		ret = classifier->ExecuteMatchingOnMemory(image_name);
		if(ret == 0){
			memset(buf, 0x00, sizeof(buf));
			snprintf(buf, sizeof(buf), "%d %f", classifier->get_result_(), classifier->get_reliability_());
			len = write(server->accept_sock, buf, strlen(buf));
			if(len < 1){
				perror("write");
				break;
			}
		}else{
			memset(buf, 0x00, sizeof(buf));
			strncpy(buf, "Error: could not read the image file(s)\n", sizeof(buf));
			len = write(server->accept_sock, buf, strlen(buf));
			if(len < 1){
				perror("write");
				break;
			}
		}

		server->CloseSocket(server->accept_sock);
	}

	server->CloseServer();

	delete classifier;
	delete server;

	return 0;
}
	
