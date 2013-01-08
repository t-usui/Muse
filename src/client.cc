#include "client.h"

int main(int argc, char **argv){
	int len;
	char buf[64];

	if(argc != 4){
		std::cerr << "Usage: client <Hostname> <Port> <Image name>" << std::endl;
		exit(EXIT_FAILURE);
	}

	muse::Client *client;
	
	client = new muse::Client();

	client->ConnectServer(argv[1], argv[2]);

	len = write(client->sock, argv[3], sizeof(buf));
	len = read(client->sock, buf, sizeof(buf));

	std::cout << buf << std::endl;

	client->DisconnectServer();
	delete client;

	return 0;
}
