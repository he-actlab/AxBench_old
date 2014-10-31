/*
 * fft.cpp
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */
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
	#ifdef NPU_ANALOG
	#define MAX_K ((1 << 15))
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
#endif

#ifdef NPU_ANALOG
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
		fft_dataFile.open("./data/fft_observation.data");
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

	for(i = 0 ;i < K ; i++)
	{
		x[i].real = i;
		x[i].imag = 0 ;

		x_orig[i].real =  i;
		x_orig[i].imag = 0;
	}

	#ifdef NPU_ANALOG
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
	    //printf("Count:					\t\t%d\n", count);
	    //printf("Total Error:			\t\t%f\n", e);
	    e = 100.0 * (e / (float)count);
	    printf("\033[31;1m--------------------\033[0m\n");
	    printf("\033[31;1mFFT Error:	%0.2f%% \033[0m\n", e);
	    printf("\033[31;1m--------------------\033[0m\n\n");
	    #endif
	    printf("\033[37;1m\nThank you for using ** AxBench **...\033[0m\n\n");
    

	return 0 ;
}