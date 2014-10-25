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
	int i, j;
	int x ;
	int n ;

	int x_orig ;
	std::cout.precision(8);

	n = atoi(argv[1]) ;

	std::cout << "# Number of iterations: " << n << std::endl;

	float* xyz = (float*)malloc(n * 6 * 3 * sizeof (float)) ;

	if(xyz == NULL)
	{
		std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
		return -1 ;
	}
	srand (time(NULL));
	for(i = 0 ; i < n * 6 * 3 ; ++i)
	{
		x = rand() % 10 ;
		xyz[i] = ((float)x)/10 ;
	}


	#ifdef NPU_SW
		std::ofstream diffFile ;
		diffFile.open("./data/difference_nn.data") ;
		diffFile << "Out1\tOut2\tX\tX_orig" << std::endl ;
	#endif

	#ifdef NPU_FANN
		fann_type* parrotOut ;
		struct fann *ann ;
		std::string nn = argv[2];
		ann = fann_create_from_file(nn.c_str()) ;

		std::ofstream diffFile ;
		diffFile.open("./data/difference_fann.data") ;
		


		std::ofstream classifier_data ;
		classifier_data.open("./train/jmeint_class.data");

	#endif

// Create the NN

#ifdef NPU_SW
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
	jmeintDataFile.open("./train/jmeint_train.data") ;
	jmeintDataFile.precision(4) ;

	jmeintDataFile << n << std::endl ;
	jmeintDataFile << 18 << "\n" << 1 << std::endl ; 

#endif

	int missRate = 0 ;
	int totalCount = 0 ;

	double output = 0.0;


	for(i = 0 ; i < (n * 6 * 3); i += 6 * 3)
	{
		totalCount++ ;
		#ifdef NPU_OBSERVATION
			for(j = 0 ; j < 6; ++j)
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
			//float nn_value = parrotOut[0] + 4.0 * parrotOut[1] ;

			double threshold = 0.00 ;
			//std::cout << "data: " << parrotOut[0] << std::endl;
			if(parrotOut[0] > 0.5 )
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

			// if(outValue == 0)
			// {
			// 	x_orig = 1 ;
			// }
			// else
			// {
			// 	x_orig = 0 ;
			// }

			diffFile << xyz + i + 0 * 3 << 

			diffFile << parrotOut[0] << "\t" << x_orig << "\t" << x << std::endl ;
			// if(x!= x_orig)
			// 	diffFile << "x" ;
			// else
			// 	diffFile << "-" ;
			// diffFile << std::endl ;

			classifier_data << parrotOut[0] << "\n";
			classifier_data << x_orig << "\n";


			if(x != x_orig)
			{
				missRate++ ;
			}

		#endif
		#ifdef NPU_SW

			inputData.clear() ;

			for(int j = 0 ; j < 18 ; j++)
			{
				inputData.push_back(xyz[i + j]) ;
			}

			currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at, 
												isNoiseBefore, meanBefore, sigmaBefore,
												isNoiseAfter, meanAfter, sigmaAfter) ;

			diffFile << outputData[0] << "\t" ;
			//diffFile << outputData[1] << "\t" ;

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
				jmeintDataFile << "0.9";
			}
			else
			{
				jmeintDataFile << "0.1";
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


	std::cout << "\033[1;31m Miss Rate: " << (double)missRate/(double)totalCount << "\033[0m\n";
	#ifdef NPU_SW
		diffFile.close() ;
	#endif
	#ifdef NPU_FANN
		diffFile.close() ;
		classifier_data.close();
	#endif
	return 0 ;
}