#ifndef __SIFT_H__
#define __SIFT_H__

#define NOIMAGEMODE

#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <dirent.h>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

#include <mysql55/mysql/mysql.h>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include "calculate.h"
#include "version.h"

namespace muse{
	class Classifier{
		private:
			int result_;
			double reliability_;

		public:
			cv::Mat *descriptors;
			cv::SiftFeatureDetector detector_;
			cv::SiftDescriptorExtractor extractor_;
			cv::FlannBasedMatcher matcher;
			std::vector<cv::DMatch> matches;

			Classifier();
			~Classifier();

			// Accessor, Mutator
			int get_result_();
			double get_reliability_();

			void StartupIntaractiveCommandLine();
			void ExecuteMatching(char *image_name);
			int ExecuteMatchingOnMemory(char *image_name);
			double CalculateEuclideanDistance(cv::DMatch matrix);
			double CalculateCosineSimilarity(cv::Mat vector_x, cv::Mat vector_y);
			double CalculateJaccardSimilarityCoefficient(int num1, int num2, int intersection);
			void SetResultAndReliability(double *data, int *table, int num);
	};
}

#endif /* __SIFT_H__ */
