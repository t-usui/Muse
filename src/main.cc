#include "main.h"

int main(int argc, char **argv){
	if(argc > 3){
		fprintf(stderr, "Usage: sift <<option (image1, ...)> or image_to_match>\n");
		exit(EXIT_FAILURE);
	}

	cv::initModule_nonfree();

	sift::Classifier c;

	c.ConnectDatabase();

	if(argc == 1){
		c.StartupIntaractiveCommandLine();
	}else{
		if(strncmp(argv[1], "--update", strlen(argv[1])) == 0){
			c.UpdateDatabase();
		}else if(strncmp(argv[1], "--add", strlen(argv[1])) == 0){

		}else if(strncmp(argv[1], "--cvversion", strlen(argv[1])) == 0){
			PrintOpenCVVersion();
		}
	}

	c.CloseDatabase();

	return 0;
}

