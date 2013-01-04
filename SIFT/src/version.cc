#include "version.h"

void PrintOpenCVVersion(){
	std::cout << "version: " << CV_VERSION << std::endl;
	std::cout << "  major: " << CV_MAJOR_VERSION << std::endl;
	std::cout << "  minor: " << CV_MINOR_VERSION << std::endl;
	std::cout << "  subminor: " << CV_SUBMINOR_VERSION << std::endl;
	std::cout << "OpenCV >= 2.0.0: " << (OPENCV_VERSION_CODE>=OPENCV_VERSION(2,0,0)?"true":"false") << std::endl;

	return;
}
