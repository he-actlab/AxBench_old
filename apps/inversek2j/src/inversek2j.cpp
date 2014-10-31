/*
 * inversek2j.cpp
 * 
 *  Created on: Sep. 10 2013
 *			Author: Amir Yazdanbakhsh <yazdanbakhsh@wisc.edu>
 */

#include <iostream>
#include <cstdlib>
#include "kinematics.hpp"

#include "neuralnetwork.hpp"
#include "xml_parser.hpp"
#include <time.h>


#ifdef NPU_FANN
	#include "floatfann.h"
#endif //NPU_FANN

#define PI 3.141592653589

#include <boost/math/special_functions/fpclassify.hpp>

int main(int argc, const char* argv[])
{
	int n, x;

	#ifdef NPU_OBSERVATION
		n = atoi(argv[1]) ;
	#endif

	#ifdef NPU_ANALOG
		std::string nn_name = argv[2] ;
		std::ofstream diffFile ;
		diffFile.open("./data/sw_difference.data");

		diffFile << "In1\tIn2\tOut1\tOut1_Orig\tOut2\tOut2_Orig\n" ;

		int iBits = atoi(argv[3]) ;
		int wBits = atoi(argv[4]) ;
		int isFP  = atoi(argv[5]) ;
		ApproxType at ;

		if (isFP == 0)
		{
			at = LP ;
		}
		else
		{
			at = FP ;
		}
	#endif


	#ifdef NPU_FANN
		std::string nn_name = argv[1] ;
		fann_type* parrotOut ;
		struct fann *ann ;

		std::ofstream diffFile ;
		diffFile.open("./data/fann_difference.data");
		diffFile.precision(8);

		diffFile << "In1\tIn2\tOut1\tOut1_Orig\tOut2\tOut2_Orig\n" ;

		ann = fann_create_from_file(nn_name.c_str()) ;
		std::cout << "# Creating the NN from the FANN configuration file...\n" ;
	#endif

	// Create the NN
	#ifdef NPU_ANALOG
		std::vector<double> inputData ;
		std::vector<double> outputData ;

		// Create the Neural Network
		boost::shared_ptr<anpu::NeuralNetwork> currNeuralNetworkPtr ;
		anpu::XMLParser currXMLParser(nn_name, currNeuralNetworkPtr, false) ;
	#endif



	#ifdef NPU_FANN
		std::ifstream data("./data/inversek2j.data");
		int number;
		data >> number;
		//std::cout << "Total number of inputs: " << number << std::endl;
		n = number;

		int in, out;
		data >> in;
		data >> out;
		//std::cout << "Number of inputs: 	" << in 	<< std::endl;
		//std::cout << "Number of outputs:	" << out 	<< std::endl;

		float in_data1, in_data2;
		float out_data1, out_data2;
	#endif


	// Number of iteration
	//std::cout << "# Number of iterations: " << n << std::endl ;

	float* t1t2xy = (float*)malloc(n * 2 * 2 * sizeof(float)) ;

	if(t1t2xy == NULL)
	{
		std::cerr << "# Cannot allocate memory for the coordinates an angles!" << std::endl ;
		return -1 ;
	}

	srand (time(NULL));


	#ifdef NPU_OBSERVATION
		std::ofstream dataFile ;
		dataFile.open("./data/inversek2j_observation.data");
		dataFile.precision(8);
		dataFile << n << std::endl ;
		dataFile << 2 << std::endl ;
		dataFile << 2 << std::endl ;
	#endif




	for(int i = 0 ; i < n * 2 * 2 ; i += 2 * 2)
	{

		#ifdef NPU_OBSERVATION
			x = rand() ;
			t1t2xy[i] = fabs((((float)x)/RAND_MAX)) * PI / 2 ;

			x = rand() ;
			t1t2xy[i + 1] = fabs((((float)x)/RAND_MAX)) * PI / 2 ;
		#endif


		#ifdef NPU_FANN
			data >> in_data1 	>> 	in_data2;
			data >> out_data1 	>>	out_data2;

			t1t2xy[i] = out_data1;
			t1t2xy[i+1] = out_data2;
		#endif

		#ifdef NPU_OBSERVATION
			float theta1 = t1t2xy[i] ;
			float theta2 = t1t2xy[i + 1] ;
		#endif


		#ifdef NPU_VALIDATION
			t1t2xy[i + 2] = 0;
			t1t2xy[i + 3] = 0;
			std::cout << "##################################################" << std::endl ;
			std::cout << "(t1 = " << t1t2xy[i] * 180 / PI ;
			std::cout << ", t1 = " << t1t2xy[i + 1] * 180 / PI  << ") =============> ";
			std::cout << "(x = " << t1t2xy[i + 2] << ", y = " << t1t2xy[i + 3] << ")" <<std::endl ;
		#endif

		forwardk2j(t1t2xy[i + 0], t1t2xy[i + 1], t1t2xy + (i + 2), t1t2xy + (i + 3)) ;

		#ifdef NPU_VALIDATION
			std::cout << "(t1 = " << t1t2xy[i] * 180 / PI ;
			std::cout << ", t1 = " << t1t2xy[i + 1] * 180 / PI  << ") =============> ";
			std::cout << "(x = " << t1t2xy[i + 2] << ", y = " << t1t2xy[i + 3] << ")" <<std::endl ;
		#endif


		#ifdef NPU_OBSERVATION
			float x = t1t2xy[i+2] ;
			float y = t1t2xy[i+3] ;

			dataFile << x << " " << y  << std::endl ;
			dataFile << theta1 << " " << theta2;
			

			if(i != (n-1) * 2 * 2)
			{
				dataFile << std::endl ;
			}
		#endif
	}

	#ifdef NPU_VALIDATION
		std::cout << "##################################################" << std::endl ;
	#endif



	#ifndef NPU_OBSERVATION
	double e = 0 ;
	int count = 0;
	double absError = 0.0;
	#endif

	for(int i = 0 ; i < n * 2 * 2 ; i += 2 * 2)
	{
		#ifdef NPU_VALIDATION
			std::cout << "##################################################" << std::endl ;
			std::cout << "(t1 = " << t1t2xy[i] * 180 / PI ;
			std::cout << ", t1 = " << t1t2xy[i + 1] * 180 / PI  << ") =============> ";
			std::cout << "(x = " << t1t2xy[i + 2] << ", y = " << t1t2xy[i + 3] << ")" <<std::endl ;
		#endif

		#ifdef NPU_VALIDATION
			inversek2j(t1t2xy[i + 2], t1t2xy[i + 3], t1t2xy + (i + 0), t1t2xy + (i + 1)) ;
		#endif

		#ifdef NPU_ANALOG
			// Initialize input / output neurons
			inputData.clear() ;
			inputData.push_back(t1t2xy[i + 2]) ;
			inputData.push_back(t1t2xy[i + 3]) ;
			currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;
			t1t2xy[i + 0] = outputData[0] ;
			t1t2xy[i + 1] = outputData[1] ;

			float in1  = t1t2xy[i + 2] ;
			float in2  = t1t2xy[i + 3] ;
			float out1 = t1t2xy[i + 0];
			float out2 = t1t2xy[i + 1];
			inversek2j(in1, in2, &out1, &out2) ;

			diffFile << t1t2xy[ i + 2 ] << "\t" << t1t2xy[ i + 3 ]  << "\t" ;
			diffFile << t1t2xy[ i + 0 ] << "\t" << out1 << "\t" ;
			diffFile << t1t2xy[ i + 1 ] << "\t" << out2 << std::endl ;

		#endif

		#ifdef NPU_FANN
			parrotOut = fann_run(ann, (fann_type*)(t1t2xy + i + 2)) ;
			t1t2xy[i + 0] = parrotOut[0] ; 
			t1t2xy[i + 1] = parrotOut[1] ;

			float in1  = t1t2xy[i + 2] ;
			float in2  = t1t2xy[i + 3] ;
			float out1 = t1t2xy[i + 0] ;
			float out2 = t1t2xy[i + 1] ;
			inversek2j(in1, in2, &out1, &out2) ;

			diffFile << t1t2xy[ i + 2 ] << "\t" << t1t2xy[ i + 3 ]  << "\t" ;
			diffFile << t1t2xy[ i + 0 ] << "\t" << out1 << "\t" ;
			diffFile << t1t2xy[ i + 1 ] << "\t" << out2 << std::endl ;


			float diff1 = out1 - t1t2xy[i+0];
			float diff2 = out2 - t1t2xy[i+1];

			float nominator 	= sqrt(diff1*diff1+diff2*diff2);
			float denominator 	= sqrt(out1*out1+out2*out2);


			if (denominator==0)
				e = 1;
			else if (isnan(nominator) or isnan(denominator))
				e = 1;
			else if (nominator / denominator > 1)
				e = 1;
			else
				e = nominator / denominator;
			count++;
			absError += e;
		#endif

		#ifdef NPU_VALIDATION
			forwardk2j(t1t2xy[i + 0], t1t2xy[i + 1], t1t2xy + (i + 2), t1t2xy + (i + 3));
			std::cout << "(t1 = " << t1t2xy[i] * 180 / PI ;
			std::cout << ", t1 = " << t1t2xy[i + 1] * 180 / PI  << ") =============> ";
			std::cout << "(x = " << t1t2xy[i + 2] << ", y = " << t1t2xy[i + 3] << ")" <<std::endl ;
		#endif


		#ifdef NPU_ANALOG
			e = fabs(out1 - t1t2xy[i+0]) + fabs(out2 - t1t2xy[i + 1]) ;
			printf("e: %f\n", e);
			double denominator = fabs(out1 + out2) ;
			if(isnan(e))
				continue;
			if(denominator == 0)
			{
				e = 0 ;
			}
			else
			{
				e /= denominator ;
				count++;
			}

			absError += e;
		#endif
	}

	#ifdef NPU_FANN
		absError = (absError / (double) count) * 100.0 ;
		printf("\033[31;1m--------------------\033[0m\n");
		printf("\033[31;1mError:	%0.2f%% \033[0m\n", absError);
		printf("\033[31;1m--------------------\033[0m\n\n");
	#endif
		printf("\033[37;1m\nThank you for using ** AxBench **...\033[0m\n\n");

	#ifdef NPU_VALIDATION
		std::cout << "##################################################" << std::endl ;
	#endif

	free(t1t2xy) ; 
	t1t2xy = NULL ;

	#ifdef NPU_ANALOG
		diffFile.close() ;
	#endif
	#ifdef NPU_FANN
		diffFile.close() ;
	#endif

	#ifdef NPU_OBSERVATION
		dataFile.close() ;
	#endif
	return 0 ;
}