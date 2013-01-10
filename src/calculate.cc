#include "calculate.h"

namespace muse{
	Calculator::Calculator(){
		this->InitAccumulator();
	}


	Calculator::~Calculator(){

	}


	double Calculator::CalculateEuclideanDistance(cv::DMatch matrix){
		return matrix.distance;
	}


	double Calculator::CalculateCosineSimilarity(cv::Mat vector1, cv::Mat vector2){
		double cosine_similarity;
		double inner_product;
		double norm1;
		double norm2;

		// (Cosine similarity) = x*y/|x||y|
		inner_product = vector1.dot(vector2);
		norm1 = cv::norm(vector1);
		norm2 = cv::norm(vector2);
		cosine_similarity = inner_product / (norm1 * norm2);

		return cosine_similarity;
	}


	double Calculator::CalculateJaccardSimilarityCoefficient(int num1, int num2, int intersection){
		double jaccard_similarity_coefficient;

		// (Jaccard similarity coefficient) = |X∩Y|/|X∪Y|
		jaccard_similarity_coefficient = (double)intersection / ((double)num1 + (double)num2 - (double)intersection);

		return jaccard_similarity_coefficient;
	}


	// Clear accumulator
	void Calculator::InitAccumulator(){
		this->accumulator = boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::mean, boost::accumulators::tag::variance> >();

		return;
	}


	// Accumulate from double variance
	void Calculator::Accumulate(double num){
		this->accumulator(num);

		return;
	}


	// Accumulate from array
	void Calculator::SetAccumulator(double *data, int num){
		std::for_each(&data[0], &data[num], boost::bind<void>(boost::ref(this->accumulator), _1));

		return;
	}


	double Calculator::CalculateMean(double *data, int num){
		double mean;

		this->InitAccumulator();
		this->SetAccumulator(data, num);
		mean = boost::accumulators::mean(this->accumulator);

		return mean;
	}


	double Calculator::CalculateVariance(double *data, int num){
		double variance;

		this->InitAccumulator();
		this->SetAccumulator(data, num);
		variance = boost::accumulators::variance(this->accumulator);

		return variance;
	}


	double Calculator::CalculateStandardDeviation(double variance){
		double standard_deviation;

		// (Standard deviation) = √variance
		standard_deviation = sqrt(variance);

		return standard_deviation;
	}


	double *Calculator::CalculateDeviationValue(double *data, int num, const char *mode){
		double mean;
		double variance;
		double standard_deviation;
		double *deviation_value;

		mean = this->CalculateMean(data, num);
		variance = this->CalculateVariance(data, num);
		standard_deviation = this->CalculateStandardDeviation(variance);

		#ifdef DEBUG
			std::cout << "mean: " << mean << std::endl;
			std::cout << "variance: " << variance << std::endl;
			std::cout << "standard deviation: " << standard_deviation << std::endl;
		#endif

		deviation_value = new double[num];

		for(int i=0;i<num;i++){
			if(strncmp(mode, "normal", 6) == 0){
				deviation_value[i] = (data[i] - mean) * 10 / standard_deviation + 50;
			}else if(strncmp(mode, "reverse", 7) == 0){
				deviation_value[i] = ((data[i] - mean) * 10) * -1 / standard_deviation + 50;
			}
		}

		return deviation_value;
	}


	double *Calculator::CalculateSimilarity(double *deviation_value1, double *deviation_value2, double *deviation_value3, int num){
		double *similarity;

		similarity = new double[num];

		for(int i=0;i<num;i++){
			#ifdef JACCARD
				similarity[i] = floor((deviation_value1[i] + deviation_value2[i] + deviation_value3[i]) / 3) / 100;
			#else
				similarity[i] = floor((deviation_value1[i] + deviation_value2[i]) / 2) / 100;
			#endif
		}

		return similarity;
	}
}

