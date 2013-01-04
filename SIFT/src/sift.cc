#include "sift.h"

namespace sift{
	Classifier::Classifier(){

	}


	Classifier::~Classifier(){

	}


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
*/
		return;
	}


	void Classifier::ExecuteMatching(char *image_name){
/*		const int kImageNumber = 800;

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
*/
/*
			for(int j=0;j<vector_num;j++){
				for(int k=0;k<128;k++){
					printf("[%d][%d]: %f %f\n", j, k, descriptor.at<float>(j, k), fetched_descriptor.at<float>(j, k));
					// printf("[%d][%d]: %f %f\n", j, k, descriptor.at<float>(j, k), feature[j*128+k]);
				}
			}
*/
/*
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
*/
		return;
	}


	void Classifier::ExecuteMatchingOnMemory(char *image_name){
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
			return;
		}

		std::cout << "OK" << std::endl;

		this->detector_.detect(image, key_point);
		this->extractor_.compute(image, key_point, descriptor);
	
		for(int i=0;i<kImageNumber;i++){
			this->matcher.match(descriptor, this->descriptors[i], this->matches);

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

		puts("(On memory mode)\n");

		return;
	}
}

