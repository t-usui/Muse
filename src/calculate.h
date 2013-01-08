#ifndef __CALCULATE_H__
#define __CALCULATE_H__

#include <iostream>
#include <new>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include <boost/utility.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

namespace muse{
	class Calculator{
		private:
			boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::mean, boost::accumulators::tag::variance> > accumulator;

			void InitAccumulator();
			void Accumulate(double num);
			void SetAccumulator(double *data, int num);
			double CalculateVariance(double *data, int num);
			double CalculateStandardDeviation(double variance);

		public:
			Calculator();
			~Calculator();

			double CalculateEuclideanDistance(cv::DMatch matrix);
			double CalculateCosineSimilarity(cv::Mat vector1, cv::Mat vector2);
			double CalculateJaccardSimilarityCoefficient(int num1, int num2, int intersection);
			double CalculateMean(double *data, int num);
			double *CalculateDeviationValue(double *data, int num, const char *mode);
			double *CalculateSimilarity(double *deviation_value1, double *deviation_value2, double *deviation_value3, int num);
	};
}

#endif /* __CALCULATE_H__ */
