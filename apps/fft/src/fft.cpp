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
#define MAX_K ((1 << 15))
#endif
#ifdef NPU_SW
#define MAX_K ((1 << 11))
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

	#ifdef NPU_OBSERVATION
		fft_dataFile.open("./train/fft.data");
		fft_dataFile.precision(8) ;
		fft_dataFile << MAX_K << std::endl ;
		fft_dataFile << "1" << "\n" ;
		fft_dataFile << "2" << "\n" ;
	#endif

	#ifdef NPU_FANN
		std::string nn = argv[1] ;
		ann = fann_create_from_file(nn.c_str()) ;
		std::cout << "# Creating the NN from the FANN configuration file...\n" ; 
	#endif // NPU_FANN

	int i ;
	int K = MAX_K ;

	srand(time(NULL));
	for(i = 0 ;i < K ; i++)
	{
		x[i].real = rand() % 1024 ;
		x[i].imag = 0 ;

		x_orig[i].real = x[i].real ;
		x_orig[i].imag = 0;
	}

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
		double mse = 0.;
	    double absErr = 0.;
	    double e;
	    int count = 0 ;
	    for(i = 0; i < K - 1; ++i) {
	    	//printf("########################################\n");
	    	//printf("%d:\n", indices[i]);
	    	//printf("%.4f --------> %.4f + %.4fj\n", abs(f_orig + i), f_orig[i].real, f_orig[i].imag);
	    	//printf("%.4f --------> %.4f + %.4fj\n", abs(f + i), f[i].real, f[i].imag);
	    	mse += (f[i].real - f_orig[i].real) * (f[i].real - f_orig[i].real);
	    	mse += (f[i].imag - f_orig[i].imag) * (f[i].imag - f_orig[i].imag);


	    	e = (f[i].real - f_orig[i].real) * (f[i].real - f_orig[i].real);
	    	e += (f[i].imag - f_orig[i].imag) * (f[i].imag - f_orig[i].imag);

	    	if((f_orig[i].real * f_orig[i].real + f_orig[i].imag * f_orig[i].imag) == 0)
	    	{
	    		e = 0 ;
	    	}
	    	else
	    	{
	    		e = e / (f_orig[i].real * f_orig[i].real + f_orig[i].imag * f_orig[i].imag);
	    		count++;
	    	}

	    	//printf("%.4f\n", e);
	    	absErr += e;
	    }
	    mse = mse / K;
	    absErr = absErr / count;
	    printf("Count: %d", count);
	    printf("MSE: %f, RMSE: %f\n", mse, sqrt(mse));
	    printf("absErr: %f\n", absErr);
    #endif

	return 0 ;
}