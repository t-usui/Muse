#include "sift.h"

namespace muse{
	Classifier::Classifier(){

	}


	Classifier::~Classifier(){

	}


	int Classifier::get_result_(){
		return this->result_;
	}


	double Classifier::get_reliability_(){
		return this->reliability_;
	}


	// Not currently in use
	void Classifier::StartupIntaractiveCommandLine(){
/*		int i;
		int load_flag = 0;
		char image_name[256];
		char *p;

		while(1){
			puts("Input the image.");
			printf("> ");
			memset(image_name, 0x00, sizeof(image_name));
			fgets(image_name, sizeof(image_name), stdin);
			if(strncmp(image_name, "quit\n", 5) == 0){
				exit(EXIT_SUCCESS);
			}else if(strncmp(image_name, "load\n", 5) == 0){
				this->LoadOnMemory();
				load_flag = 1;
			}else{
				p = image_name;
				while(*p != '\0'){
					if(*p == '\n'){
						*p = '\0';
					}
					p++;
				}

				if(load_flag == 0){
					this->ExecuteMatching(image_name);
				}else{
					this->ExecuteMatchingOnMemory(image_name);
				}
			}
		}

		return;
	}


	// Not currently in use
	void Classifier::ExecuteMatching(char *image_name){
		const int kImageNumber = 800;

		int vector_num;

		double distance;
		double min_distance;
		double max_distance;

		cv::Mat image;
		cv::Mat descriptor;
		cv::Mat raw_result;

		float *feature;

		std::vector<cv::KeyPoint> key_point;

		image = cv::imread(image_name);
		if(image.empty() != 0){
			fprintf(stderr, "Error: could not read the image file(s)\n");
			exit(EXIT_FAILURE);
		}

		this->detector_.detect(image, key_point);
		this->extractor_.compute(image, key_point, descriptor);
	
		for(int i=0;i<kImageNumber;i++){
			vector_num = this->ExecuteSelectQuery(i, &feature);
			cv::Mat fetched_descriptor(vector_num, 128, CV_32FC1, feature);


			for(int j=0;j<vector_num;j++){
				for(int k=0;k<128;k++){
					printf("[%d][%d]: %f %f\n", j, k, descriptor.at<float>(j, k), fetched_descriptor.at<float>(j, k));
					// printf("[%d][%d]: %f %f\n", j, k, descriptor.at<float>(j, k), feature[j*128+k]);
				}
			}

			this->matcher.match(descriptor, fetched_descriptor, this->matches);

			min_distance = 100;
			max_distance = 0;
			for(int j=0;j<descriptor.rows;j++){
				distance = this->matches[j].distance;
				if(distance < min_distance){
					min_distance = distance;
				}
				if(distance > max_distance){
					max_distance = distance;
				}
			}

			std::vector<cv::DMatch> good_matches;

			for(int j=0;j<descriptor.rows;j++){
				if(this->matches[j].distance <= 2 * min_distance){
					good_matches.push_back(this->matches[j]);
				}
			}

			for(int j=0;j<good_matches.size();j++){
				printf("[%d]Keypoint 1: %d, Keypoint 2: %d\n", i, good_matches[j].queryIdx, good_matches[j].trainIdx);
			}
		}

		// cv::drawMatches(img[0], key_point[0], img[1], key_point[1], matches, raw_result);
		// cv::imshow("SIFT raw match result", raw_result);



		// cv::drawMatches(img[0], key_point[0], img[1], key_point[1], good_matches, result, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
		// cv::imshow("SIFT good match result", result);
		// cv::imshow("SIFT raw match result", raw_result);


		//	cv::waitKey(0);
		//	cv::imwrite("result.jpg", result);

		puts("(Off memory mode)\n");

		delete []feature;

		return;
*/	}


	int Classifier::ExecuteMatchingOnMemory(char *image_name){
		const int kImageNumber = 800;

		int vector_num;
		int good_matched_image[kImageNumber];
		int good_matched_image_number;

		double distance;
		double min_distance;
		double max_distance;
		double *euclidean_distance;
		double *cosine_similarity;
		double euclidean_distance_mean[kImageNumber];
		double *euclidean_distance_mean_deviation_value;
		double cosine_similarity_mean[kImageNumber];
		double *cosine_similarity_mean_deviation_value;
		double jaccard_similarity_coefficient[kImageNumber];
		double *jaccard_similarity_coefficient_deviation_value;
		double *similarity;

		cv::Mat image;
		cv::Mat descriptor;
		cv::Mat matched_feature_vector[2];

		#ifdef IMAGE
			cv::Mat raw_result;
		#endif

		std::vector<cv::KeyPoint> key_point;

		Calculator *calculator;

		image = cv::imread(image_name);
		if(image.empty() != 0){
			std::cerr << "Error: could not read the image file(s)" << std::endl;
			return EXIT_FAILURE;
		}

		calculator = new Calculator;

		this->detector_.detect(image, key_point);
		this->extractor_.compute(image, key_point, descriptor);


		for(int i=0, j=0;i<kImageNumber;i++, j++){
			this->matcher.match(descriptor, this->descriptors[i], this->matches);

			min_distance = 100;
			max_distance = 0;
			for(int k=0;k<descriptor.rows;k++){
				distance = this->matches[k].distance;
				if(distance < min_distance){
					min_distance = distance;
				}
				if(distance > max_distance){
					max_distance = distance;
				}
			}

			std::vector<cv::DMatch> good_matches;

			for(int k=0;k<descriptor.rows;k++){
				if(this->matches[k].distance <= 2 * min_distance){
					good_matches.push_back(this->matches[k]);
				}
			}

			if(good_matches.size() != 0){
				good_matched_image[j] = i + 1;

				#ifdef DEBUG
					std::cout << "Image number [" << i + 1 << "]" << std::endl;
				#endif

				euclidean_distance	= new double[good_matches.size()];
				cosine_similarity	= new double[good_matches.size()];

				for(int k=0;k<good_matches.size();k++){
					euclidean_distance[k] = calculator->CalculateEuclideanDistance(good_matches[k]);

					matched_feature_vector[0] = descriptor.row(good_matches[k].queryIdx);
					matched_feature_vector[1] = this->descriptors[i].row(good_matches[k].trainIdx);
					cosine_similarity[k] = calculator->CalculateCosineSimilarity(matched_feature_vector[0], matched_feature_vector[1]);

					#ifdef DEBUG
						std::cout << "Keypoint 1: " << good_matches[k].queryIdx << ", " << "Keypoint 2: " << good_matches[k].trainIdx << ", ";
						std::cout << "Euclidean distance: " << euclidean_distance[k] << ", " << "Cosine similarity: " << cosine_similarity[k] << std::endl;
					#endif
				}

				euclidean_distance_mean[j] = calculator->CalculateMean(euclidean_distance, good_matches.size());
				cosine_similarity_mean[j] = calculator->CalculateMean(cosine_similarity, good_matches.size());
				jaccard_similarity_coefficient[j] = calculator->CalculateJaccardSimilarityCoefficient(descriptor.rows, this->descriptors[j].rows, good_matches.size());

				delete []euclidean_distance;
				delete []cosine_similarity;

				#ifdef DEBUG
					std::cout << "Euclidean distance mean: " << euclidean_distance_mean[j] << std::endl;
					std::cout << "Cosine similarity mean: " << cosine_similarity_mean[j] << std::endl;
					std::cout << "Jaccard similarity coefficient: " << jaccard_similarity_coefficient[j] << std::endl;
				#endif
			}else{
				j--;
			}
			good_matched_image_number = j + 1;
		}

		// Calculate each deviation value
		euclidean_distance_mean_deviation_value = calculator->CalculateDeviationValue(euclidean_distance_mean, good_matched_image_number, "reverse");
		cosine_similarity_mean_deviation_value = calculator->CalculateDeviationValue(cosine_similarity_mean, good_matched_image_number, "normal");
		jaccard_similarity_coefficient_deviation_value = calculator->CalculateDeviationValue(jaccard_similarity_coefficient, good_matched_image_number, "normal");

		// Calculate similarity
		similarity = calculator->CalculateSimilarity(euclidean_distance_mean_deviation_value, cosine_similarity_mean_deviation_value, jaccard_similarity_coefficient_deviation_value, good_matched_image_number);

		#ifdef DEBUG
			for(int i=0;i<good_matched_image_number;i++){
				std::cout << "euclidean: [" << i << "]: " << euclidean_distance_mean_deviation_value[i] << std::endl;
				std::cout << "cosine: [" << i << "]: " << cosine_similarity_mean_deviation_value[i] << std::endl;
				std::cout << "jaccard: [" << i << "]: " << jaccard_similarity_coefficient_deviation_value[i] << std::endl;
				std::cout << "[" << i << "]: " << similarity[i] << std::endl;
			}
		#endif

		this->SetResultAndReliability(similarity, good_matched_image, good_matched_image_number);

		std::cout << "Result: " << this->result_ << std::endl;
		std::cout << "Reliability: " << this->reliability_ << std::endl;

		#ifdef IMAGE
			cv::drawMatches(img[0], key_point[0], img[1], key_point[1], matches, raw_result);
			cv::imshow("SIFT raw match result", raw_result);
		#endif

		std::cout << "(On memory mode)" << std::endl;

		delete calculator;
		delete []euclidean_distance_mean_deviation_value;
		delete []cosine_similarity_mean_deviation_value;
		delete []jaccard_similarity_coefficient_deviation_value;
		delete []similarity;

		return EXIT_SUCCESS;
	}


	void Classifier::SetResultAndReliability(double *data, int *table, int num){
		int best_number = 0;
		double reliability = 0;

		for(int i=0;i<num;i++){
			if(data[i] > reliability){
				reliability = data[i];
				best_number = i;
			}
		}

		this->reliability_ = reliability;
		this->result_ = table[best_number];

		return;
	}
}

