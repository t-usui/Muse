#ifndef __SIFT_H__
#define __SIFT_H__

#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <dirent.h>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

#include <mysql55/mysql/mysql.h>

// #include <boost/algorithm/string.hpp>

#include "version.h"

namespace sift{
	class Classifier{
		private:

		public:
			cv::Mat *descriptors;
			cv::SiftFeatureDetector detector_;
			cv::SiftDescriptorExtractor extractor_;
			cv::FlannBasedMatcher matcher;
			std::vector<cv::DMatch> matches;

			int result;
			double reliability;

			Classifier();
			~Classifier();

			void StartupIntaractiveCommandLine();
			void ExecuteMatching(char *image_name);
			void ExecuteMatchingOnMemory(char *image_name);
	};
}

#endif /* __SIFT_H__ */
