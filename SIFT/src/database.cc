#include "database.h"

namespace sift{
	Database::Database(){

	}


	Database::~Database(){

	}


	void Database::ConnectDatabase(){
		MYSQL *ret;

		const char *kHostName = "localhost";
		const char *kUserName = "fx_interns";
		const char *kPassWord = "fx_P455w0rd";
		const char *kDatabaseName = "fx_ip";
		const int kPort = 3306;

		this->con = mysql_init(NULL);
		if(this->con == NULL){
			fprintf(stderr, "Could not init the mysql\n");
			exit(EXIT_FAILURE);
		}

		ret = mysql_real_connect(
			this->con,
			kHostName,
			kUserName,
			kPassWord,
			kDatabaseName,
			kPort,
			NULL,
			0
		);

		if(ret == NULL){
			fprintf(stderr, "Error: %s\n", mysql_error(this->con));
			exit(EXIT_FAILURE);
		}

		return;
	}


	void Database::CloseDatabase(){
		if(this->con != NULL){
			mysql_close(this->con);
		}

		return;
	}


	void Database::UpdateDatabase(){
		int ret;

		DIR *dir;
		struct dirent *dp;

		char image_path[128];

		cv::Mat image;
		cv::Mat descriptor;

		std::vector<cv::KeyPoint> key_point;

		const char *kImageDirectory = "../img/";
		const char *kDeleteQuery = "DELETE FROM sift_descriptor;";


		ret = mysql_query(con, kDeleteQuery);

		dir = opendir(kImageDirectory);
		if(dir == NULL){
			fprintf(stderr, "Error: %s\n", mysql_error(con));
			exit(EXIT_FAILURE);
		}

		for(int i=0;(dp=readdir(dir))!=NULL;i++){
			if(strncmp(dp->d_name, ".", strlen(dp->d_name)) == 0 || strncmp(dp->d_name, "..", strlen(dp->d_name)) == 0){
				i--;
				continue;
			}

			printf("%s\n", dp->d_name);
			snprintf(image_path, sizeof(image_path), "%s%s", kImageDirectory, dp->d_name);
			image = cv::imread(image_path);

/*
			if(image.empty() != 1){
				cv::imshow(image_path, image);
			}
*/

			this->detector_.detect(image, key_point);
			this->extractor_.compute(image, key_point, descriptor);

			this->ExecuteInsertQuery(i, descriptor);
		}

/*
		std::cout << key_point;
		std::cout << descriptor.rows << std::endl;
		std::cout << descriptor.cols << std::endl;
		std::cout << descriptor.dims << std::endl;
		std::cout << descriptor;
*/

//		cv::waitKey(0);

		closedir(dir);

		return;
	}


	void Database::ExecuteInsertQuery(int id, cv::Mat descriptor){
		int ret;

		int feature_id;
		float feature[128];

		const char *query =	"INSERT INTO sift_descriptor VALUES ("
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
					"?, ?, ?, ?, ?, ?, ?, ?, ?, ? );";

		MYSQL_STMT *statement;
		MYSQL_BIND bind[130];

		statement = mysql_stmt_init(this->con);

		if(statement == NULL){
			fprintf(stderr, "Error: %s\n", mysql_stmt_error(statement));
			exit(EXIT_FAILURE);
		}

		ret = mysql_stmt_prepare(statement, query, strlen(query));
		if(ret != 0){
			fprintf(stderr, "Error: %s\n", mysql_stmt_error(statement));
			exit(EXIT_FAILURE);
		}

		for(int i=0;i<130;i++){
			if(i == 0){
				bind[i].buffer_type = MYSQL_TYPE_LONG;
				bind[i].buffer = &id;
			}else if(i == 1){
				bind[i].buffer_type = MYSQL_TYPE_LONG;
				bind[i].buffer = &feature_id;
			}else{
				bind[i].buffer_type = MYSQL_TYPE_FLOAT;
				bind[i].buffer = &feature[i-2];
			}
	
			bind[i].is_null = 0;
		}

		ret = mysql_stmt_bind_param(statement, bind);
		if(ret != 0){
			fprintf(stderr, "Error: %s", mysql_stmt_error(statement));
			exit(EXIT_FAILURE);
		}

		for(int i=0;i<descriptor.rows;i++){
			feature_id = i;

			for(int j=0;j<descriptor.cols;j++){
				feature[j] = descriptor.at<float>(i, j);
			}

			ret = mysql_stmt_execute(statement);

			if(ret != 0){
				fprintf(stderr, "Error: %s\n", mysql_stmt_error(statement));
				exit(EXIT_FAILURE);
			}
		}

		return;
	}


	int Database::ExecuteSelectQuery(int id, float **feature){
		int i;
		int ret;

		int num_field;
		int num_row;

		float *test;

		const char *query =	"SELECT "
					"val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, "
					"val11, val12, val13, val14, val15, val16, val17, val18, val19, val20, "
					"val21, val22, val23, val24, val25, val26, val27, val28, val29, val30, "
					"val31, val32, val33, val34, val35, val36, val37, val38, val39, val40, "
					"val41, val42, val43, val44, val45, val46, val47, val48, val49, val50, "
					"val51, val52, val53, val54, val55, val56, val57, val58, val59, val60, "
					"val61, val62, val63, val64, val65, val66, val67, val68, val69, val70, "
					"val71, val72, val73, val74, val75, val76, val77, val78, val79, val80, "
					"val81, val82, val83, val84, val85, val86, val87, val88, val89, val90, "
					"val91, val92, val93, val94, val95, val96, val97, val98, val99, val100, "
					"val101, val102, val103, val104, val105, val106, val107, val108, val109, val110, "
					"val111, val112, val113, val114, val115, val116, val117, val118, val119, val120, "
					"val121, val122, val123, val124, val125, val126, val127, val128 "
					"FROM sift_descriptor WHERE id = ?;";

		MYSQL_RES *res;

		MYSQL_STMT *statement;
		MYSQL_BIND bind[1];

		statement = mysql_stmt_init(this->con);
		if(statement == NULL){
			fprintf(stderr, "Error: %s\n", mysql_stmt_error(statement));
			exit(EXIT_FAILURE);
		}

		ret = mysql_stmt_prepare(statement, query, strlen(query));
		if(ret != 0){
			fprintf(stderr, "Error: %s\n", mysql_stmt_error(statement));
			exit(EXIT_FAILURE);
		}

		res = mysql_stmt_result_metadata(statement);
		assert(res);
		num_field = mysql_num_fields(res);
		mysql_free_result(res);

		float output[num_field];
		MYSQL_BIND result[num_field];

		memset(bind, 0x00, sizeof(bind));
		bind[0].buffer_type = MYSQL_TYPE_LONG;
		bind[0].buffer = &id;
		bind[0].buffer_length = sizeof(id);
		bind[0].is_null = 0;

		ret = mysql_stmt_bind_param(statement, bind);
		if(ret != 0){
			fprintf(stderr, "Error: %s", mysql_stmt_error(statement));
			exit(EXIT_FAILURE);
		}

		ret = mysql_stmt_execute(statement);
		if(ret != 0){
			fprintf(stderr, "Error: %s", mysql_stmt_error(statement));
			exit(EXIT_FAILURE);
		}

		ret = mysql_stmt_store_result(statement);
		if(ret != 0){
			fprintf(stderr, "Error: %s", mysql_stmt_error(statement));
			exit(EXIT_FAILURE);
		}

		for(int i=0;i<num_field;i++){
			memset(&result[i], 0x00, sizeof(result[i]));
			result[i].buffer_type = MYSQL_TYPE_FLOAT;
			result[i].buffer = &output[i];
			result[i].buffer_length = sizeof(output[i]);
			result[i].is_null = 0;
		}

		ret = mysql_stmt_bind_result(statement, result);
		if(ret != 0){
			fprintf(stderr, "Error: %s", mysql_stmt_error(statement));
			exit(EXIT_FAILURE);
		}

		num_row = mysql_stmt_num_rows(statement);

		*feature = new float[num_field * num_row];

		i = 0;
		while((ret = mysql_stmt_fetch(statement)) == 0){
			for(int j=0;j<num_field;j++){
				(*feature)[i*num_field+j] = output[j];
			}
			i++;
		}

		if(ret != MYSQL_NO_DATA){
			fprintf(stderr, "Error: %s\n", mysql_stmt_error(statement));
			exit(EXIT_FAILURE);
		}

		mysql_stmt_close(statement);

		return num_row;
	}


	cv::Mat *Database::LoadOnMemory(){
		const int kImageNumber = 800;

		int vector_num;
		float *feature;

		this->descriptors = new cv::Mat[kImageNumber];

		for(int i=0;i<kImageNumber;i++){
			vector_num = this->ExecuteSelectQuery(i, &feature);
			this->descriptors[i] = cv::Mat::zeros(vector_num, 128, CV_32FC1);
			for(int j=0;j<vector_num;j++){
				for(int k=0;k<128;k++){
					this->descriptors[i].at<float>(j, k) = feature[j*128+k];
				}
			}
		}

		puts("Feature information is loaded on memory successfully.");

		return this->descriptors;
	}
}

