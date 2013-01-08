#ifndef __VERSION_H__
#define __VERSION_H__

#include <iostream>
#include <opencv2/core/core.hpp>

#define OPENCV_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define OPENCV_VERSION_CODE OPENCV_VERSION(CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION)

void PrintOpenCVVersion();

#endif /* __VERSION_H__ */
