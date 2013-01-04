#include "client.h"

using namespace sift;

int main(int argc, char **argv){
	int len;
	char buf[32];

	if(argc != 4){
		std::cerr << "Usage: client <hostname> <port> <image_name>" << std::endl;
		exit(EXIT_FAILURE);
	}

	Client c;

	c.ConnectServer(argv[1], argv[2]);

	len = write(c.sock, argv[3], sizeof(buf));
	len = read(c.sock, buf, sizeof(buf));
	write(1, buf, len);

	c.DisconnectServer();

	return 0;
}
