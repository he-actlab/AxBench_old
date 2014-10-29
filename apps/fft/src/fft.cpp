#include <cstdio>
#include <iostream>
#include "fourier.hpp"
#include <fstream>
#include "neuralnetwork.hpp"
#include "xml_parser.hpp"
#include <time.h>


#ifdef NPU_OBSERVATION
std::ofstream fft_dataFile;
#endif


#ifdef NPU_OBSERVATION
#define MAX_K ((1 << 11))
#endif
#ifdef NPU_SW
#define MAX_K ((1 << 20))
#endif
#ifdef NPU_FANN
#define MAX_K ((1 << 15))
#endif

static int indices[MAX_K];
static Complex x_orig[MAX_K] ;
static Complex f_orig[MAX_K] ;
static Complex x[MAX_K] ;
static Complex f[MAX_K] ;



#ifdef NPU_FANN
#include "floatfann.h"
struct fann *ann ;
#endif //NPU_FANN

#ifdef NPU_SW
boost::shared_ptr<anpu::NeuralNetwork> currNeuralNetworkPtr ;
ApproxType at ;
int iBits ;
int wBits ;

#endif


int main(int argc, char* argv[])
{


	int i ;

	int K = MAX_K;

	#ifdef NPU_OBSERVATION
		fft_dataFile.open("./train/fft.data");
		fft_dataFile.precision(8) ;
		fft_dataFile << K << std::endl ;
		fft_dataFile << "1" << "\n" ;
		fft_dataFile << "2" << "\n" ;
	#endif

	#ifdef NPU_FANN
		std::string nn = argv[1] ;
		ann = fann_create_from_file(nn.c_str()) ;
		std::cout << "# Creating the NN from the FANN configuration file...\n" ; 
	#endif // NPU_FANN


	//#ifdef NPU_OBSERVATION
	for(i = 0 ;i < K ; i++)
	{
		x[i].real = i;
		x[i].imag = 0 ;

		x_orig[i].real =  i;
		x_orig[i].imag = 0;
	}
	//#endif

	// #ifdef NPU_FANN
	// 	std::ifstream data("./train/fft.data");
	// 	int number;
	// 	data >> number;
	// 	std::cout << "Total number of data: " << number << std::endl;
	// 	int in, out;
	// 	data >> in;
	// 	data >> out;
	// 	std::cout << "Number of input: 	" << in << std::endl;
	// 	std::cout << "Number of output: " << out << std::endl;
	// 	float real;
	// 	int K= number;
	// 	for(int j = 0; j < number; j++)
	// 	{
	// 		data >> real;
	// 		x[j].real = real;
	// 		x[j].imag = 0;
	// 		x_orig[j].real = real;
	// 		x_orig[j].imag = 0;
	// 	}

	// #endif 

	#ifdef NPU_SW
		std::string nn = argv[1] ;
		iBits	   = atoi(argv[2]) ;
		wBits      = atoi(argv[3]) ;
		int isFp   = atoi(argv[4]) ;



		if (isFp == 0)
		{
			at = LP;
		}
		else if (isFp == 1)
		{
			at = FP ;
		}
		else
		{
			std::cout << "Error:!!:!:\n" ;
			exit(1) ;
		}
		anpu::XMLParser currXMLParser(nn, currNeuralNetworkPtr, false) ;
	#endif

	radix2DitCooleyTykeyFft(K, indices, x, f, x_orig, f_orig) ;

	// Compute error
	#ifndef NPU_OBSERVATION
	    double e = 0;
	    int count = 0 ;
	    for(i = 0; i < K - 1; ++i) {


	    	float diff_real = (f_orig[i].real - f[i].real);
	    	float diff_imag = (f_orig[i].imag - f[i].imag);
	    	float norm_nominator 	= sqrt((diff_real*diff_real) + (diff_imag*diff_imag));
	    	float norm_denominator 	= sqrt((f_orig[i].real*f_orig[i].real) + (f_orig[i].imag*f_orig[i].imag));



	    	//float norm_orig = sqrt((f_orig[i].real * f_orig[i].real) + (f_orig[i].imag * f_orig[i].imag));
	    	//float norm_appx = sqrt((f[i].real * f[i].real) + (f[i].imag * f[i].imag));

	    	//e += abs((norm_orig - norm_appx) / (norm_orig));

	    	//std::cout << norm_orig << std::endl;

	    	//e = (f[i].real - f_orig[i].real) * (f[i].real - f_orig[i].real);
	    	//e += (f[i].imag - f_orig[i].imag) * (f[i].imag - f_orig[i].imag);

	    	if(norm_denominator == 0)
	    	{
	    		e += 1.0;
	    	}
	    	else
	    	{
	    		if((norm_nominator / norm_denominator) > 1.0)
	    			e += 1.0;
	    		else
	    			e += (norm_nominator / norm_denominator);
	    		
	    	}
	    	count++;
	    }
	    printf("Count:					\t\t%d\n", count);
	    printf("Total Error:			\t\t%f\n", e);
	    e = e / (float)count;
	    printf("Average Relative Error:	\t\t%f\n", e);
    #endif

	return 0 ;
}