#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <dirent.h>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

#include <mysql55/mysql/mysql.h>

namespace muse{
	class Database{
		private:
			MYSQL *con;
			static const char *kImageDirectory;

		public:
			cv::Mat *descriptors;
			cv::SiftFeatureDetector detector_;
			cv::SiftDescriptorExtractor extractor_;

			Database();
			~Database();

			void ConnectDatabase();
			void CloseDatabase();
			void UpdateDatabase();
			void ExecuteInsertQuery(int id, cv::Mat descriptor);
			int ExecuteSelectQuery(int id, float **feature);
			cv::Mat *LoadOnMemory();
	};
}

#endif /* __DATABASE_H__ */
