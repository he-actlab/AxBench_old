/*
 * fft.cpp
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include "tritri.hpp"

#include <fstream>
#include <map>

#include "neuralnetwork.hpp"
#include "xml_parser.hpp"
#include <time.h>       /* time */

#include <boost/algorithm/string/regex.hpp>

#include "floatfann.h"

int main(int argc, char* argv[])
{
	int i;
	int x ;
	int n ;

	#ifndef NPU_OBSERVATION
	int x_orig ;
	#endif
	std::cout.precision(8);

	n = atoi(argv[1]) ;

	//std::cout << "# Number of iterations: " << n << std::endl;

	float* xyz = (float*)malloc(n * 6 * 3 * sizeof (float)) ;

	if(xyz == NULL)
	{
		std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
		return -1 ;
	}

	#ifdef NPU_OBSERVATION
		srand (time(NULL));
		for(i = 0 ; i < n * 6 * 3 ; ++i)
		{
			x = rand();
			xyz[i] = ((float)x)/(float)RAND_MAX;
		}
	#endif

	#ifdef NPU_FANN
		std::ifstream dataIn("./data/jmeint.data");
		// pass two first line
		int number;
		dataIn >> number;
		//std::cout << "Total Number: " << number << std::endl;

		int in, out;
		dataIn >> in;
		dataIn >> out;
		//std::cout << "Number of input: 	" 	<< in 	<< 	std::endl;
		//std::cout << "Number of output: "	<<	out << 	std::endl;

		i = 0;
		while(i < n)
		{
			float a[18];
			dataIn >> 	a[0] 	>> 	a[1] 	>> 	a[2] 	>> a[3] 	>> a[4] 		>> a[5] >>
						a[6]	>>	a[7]	>> 	a[8]	>> a[9]		>> a[10]		>> a[11] >>
						a[12]	>> 	a[13] 	>> 	a[14] 	>> a[15] 	>> a[16]	 	>> a[17];

			for(int j = 0 ; j < 18; j++)
			{
				xyz[i * 18 + j] = a[j];
			}

			float out1, out2;
			dataIn >> out1 >> out2 ;

			i++;
		}


	#endif


	#ifdef NPU_ANALOG
		std::ofstream diffFile ;
		diffFile.open("./data/difference_nn.data") ;
		diffFile << "Out1\tOut2\tX\tX_orig" << std::endl ;
	#endif

	#ifdef NPU_FANN
		fann_type* parrotOut ;
		struct fann *ann ;
		std::string nn = argv[2];
		ann = fann_create_from_file(nn.c_str()) ;
		std::cout << "# Creating the NN from the FANN configuration file...\n" ; 

		std::ofstream diffFile ;
		diffFile.open("./data/difference_fann.data") ;

		std::ofstream classifier_data ;
		classifier_data.open("./train/jmeint_class.data");

	#endif

#ifdef NPU_ANALOG
	// create the input/output map to get the data
	std::vector<double> inputData ;
	std::vector<double> outputData ;

	std::string nn = argv[2] ;

	int iBits = atoi(argv[3]) ;
	int wBits = atoi(argv[4]) ;
	int isFp = atoi(argv[5]) ;

	ApproxType at ;

	if(isFp == 0)
	{
		at = LP ;
	}
	else
	{
		at = FP ;
	}

	// Create the Neural Network
	boost::shared_ptr<anpu::NeuralNetwork> currNeuralNetworkPtr ;
	anpu::XMLParser currXMLParser(nn, currNeuralNetworkPtr, false) ;
#endif

#ifdef NPU_OBSERVATION
	std::ofstream jmeintDataFile ;
	jmeintDataFile.open("./data/jmeint_observation.data") ;
	jmeintDataFile.precision(4) ;

	jmeintDataFile << n << std::endl ;
	jmeintDataFile << 18 << "\n" << 2 << std::endl ; 

#endif

	#ifndef NPU_OBSERVATION
	int missRate = 0 ;
	#endif
	int totalCount = 0 ;

	//double output = 0.0;


	for(i = 0 ; i < (n * 6 * 3); i += 6 * 3)
	{
		totalCount++ ;
		#ifdef NPU_OBSERVATION
			for(int j = 0 ; j < 6; ++j)
			{
					jmeintDataFile << xyz[i + 3 * j + 0] << " " << xyz[i + 3 * j + 1] << " " << xyz[i + 3 * j + 2] ;
					if(j < 5)
					{
						jmeintDataFile << " " ;
					}
			}
			
		#endif

		#ifdef NPU_OBSERVATION
			x = tri_tri_intersect(
				xyz + i + 0 * 3, xyz + i + 1 * 3, xyz + i + 2 * 3,
				xyz + i + 3 * 3, xyz + i + 4 * 3, xyz + i + 5 * 3) ;

			//jmeintDataFile << " " << plane1 << " " << plane2 ;

			jmeintDataFile << std::endl ;

		#endif

		#ifdef NPU_FANN
			parrotOut = fann_run(ann, (fann_type*)(xyz + i)) ;

			//double threshold = 0.00 ;
			if(parrotOut[0] > parrotOut[1] )
			{
				x = 0 ;
			}
			else
			{
				x = 1 ;
			}

			x_orig = tri_tri_intersect(
				xyz + i + 0 * 3, xyz + i + 1 * 3, xyz + i + 2 * 3,
				xyz + i + 3 * 3, xyz + i + 4 * 3, xyz + i + 5 * 3) ;


			diffFile << xyz + i + 0 * 3 << 

			diffFile << parrotOut[0] << "\t" << parrotOut[1] << "\t" << x_orig << "\t" << x << std::endl ;

			if(x != x_orig)
			{
				missRate++ ;
			}

		#endif
		#ifdef NPU_ANALOG

			inputData.clear() ;

			for(int j = 0 ; j < 18 ; j++)
			{
				inputData.push_back(xyz[i + j]) ;
			}

			currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at, 
												isNoiseBefore, meanBefore, sigmaBefore,
												isNoiseAfter, meanAfter, sigmaAfter) ;

			diffFile << outputData[0] << "\t" ;

			if(outputData[0] > 0)
				x = 1 ;
			else
				x = 0 ;

			x_orig = tri_tri_intersect(
				xyz + i + 0 * 3, xyz + i + 1 * 3, xyz + i + 2 * 3,
				xyz + i + 3 * 3, xyz + i + 4 * 3, xyz + i + 5 * 3
			) ;

			diffFile << x << "\t" << x_orig << std::endl ;

			if(x != x_orig)
			{
				missRate++ ;
			}

		#endif

		#ifdef NPU_OBSERVATION

			if(x == 0)
			{
				jmeintDataFile << "0.8 0.2";
			}
			else
			{
				jmeintDataFile << "0.2 0.8";
			}
			if(i != ((n-1) * 6 * 3))
			{
				jmeintDataFile <<"\n" ;
			}

		#endif
	}

	#ifdef NPU_OBSERVATION
		jmeintDataFile.close() ;
	#endif

	free(xyz) ;
	xyz = NULL ;


	#ifndef NPU_OBSERVATION
	printf("\033[31;1m--------------------\033[0m\n");
	printf("\033[31;1mError:	%0.2f%% \033[0m\n", ((double)missRate/(double)totalCount * 100));
	printf("\033[31;1m--------------------\033[0m\n\n");
	#endif
	printf("\033[37;1m\nThank you for using ** AxBench **...\033[0m\n\n");

	#ifdef NPU_ANALOG
		diffFile.close() ;
	#endif
	#ifdef NPU_FANN
		diffFile.close() ;
		classifier_data.close();
	#endif
	return 0 ;
}