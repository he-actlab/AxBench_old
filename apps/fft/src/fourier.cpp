/*
 * fourier.cpp
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */


#include "fourier.hpp"
#include <cmath>
#include <fstream>
#include <map>
#include "neuralnetwork.hpp"
#include "xml_parser.hpp"



#ifdef NPU_OBSERVATION
	extern std::ofstream fft_dataFile ;
#endif

#ifdef NPU_FANN
	#include "floatfann.h"
	extern struct fann *ann ;
	fann_type* parrotOut ;
#endif

#ifdef NPU_ANALOG
	extern boost::shared_ptr<anpu::NeuralNetwork> currNeuralNetworkPtr ;
	extern ApproxType at ;
	extern int iBits ;
	extern int wBits ;
#endif

void calcFftIndices(int K, int* indices)
{
	int i, j ;
	int N ;

	N = (int)log2f(K) ;

	indices[0] = 0 ;
	indices[1 << 0] = 1 << (N - (0 + 1)) ;
	for (i = 1; i < N; ++i)
	{
		for(j = (1 << i) ; j < (1 << (i + 1)); ++j)
		{
			indices[j] = indices[j - (1 << i)] + (1 << (N - (i + 1))) ;
		}
	}
}

void radix2DitCooleyTykeyFft(int K, int* indices, Complex* x, Complex* f, Complex* x_orig, Complex* f_orig)
{

	calcFftIndices(K, indices) ;

	int step ;
	float arg ;
	int eI ;
	int oI ;

	#ifndef NPU_OBSERVATION
		float fftSin ;
		float fftCos ;
		Complex t ;
	#endif

	float fftSin_orig ;
	float fftCos_orig ;
	

	Complex t_orig ;
	int i ;
	int N ;
	int j ;
	int k ;

	std::vector<double> inputData ;
	std::vector<double> outputData ;


	#ifdef NPU_ANALOG
		std::ofstream differenceFile ;
		differenceFile.open("./data/sw_difference.data") ;
		differenceFile << "IN\tFFT_SIN_NN\tFFT_SIN\tFFT_COS_NN\tFFT_COS\n" ;			
	#endif

	#ifdef NPU_FANN
		std::ofstream differenceFile ;
		differenceFile.open("./data/fann_difference.data") ;
		differenceFile << "IN\tFFT_SIN_NN\tFFT_SIN\tFFT_COS_NN\tFFT_COS\n" ;
	#endif

	for(i = 0, N = 1 << (i + 1); N <= K ; i++, N = 1 << (i + 1))
	{
		for(j = 0 ; j < K ; j += N)
		{
			step = N >> 1 ;
			for(k = 0; k < step ; k++)
			{
				arg = (float)k / N ;
				eI = j + k ; 
				oI = j + step + k ;

				#ifdef NPU_ANALOG
					// Initialize input / output neurons
					inputData.clear() ;
					inputData.push_back(arg) ;
					
					currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;
					fftSin = outputData[0] ;
					fftCos = outputData[1] ;					

					fftSinCos(arg, &fftSin_orig, &fftCos_orig) ;

					differenceFile << arg << "\t" ;
					differenceFile << fftSin << "\t" << fftSin_orig << "\t" ;
					differenceFile << fftCos << "\t" << fftCos_orig << "\n" ;
				#endif

				#ifdef NPU_FANN
						parrotOut = fann_run(ann, &arg) ;

						fftSin = parrotOut[0] ;
						fftCos = parrotOut[1] ;

						fftSinCos(arg, &fftSin_orig, &fftCos_orig) ;
						differenceFile << arg << "\t" ;
						differenceFile << fftSin << "\t" << fftSin_orig << "\t" ;
						differenceFile << fftCos << "\t" << fftCos_orig << "\n" ;
				#endif

				#ifdef NPU_OBSERVATION
					fftSinCos(arg, &fftSin_orig, &fftCos_orig) ;
					fft_dataFile << arg << std::endl ;
					fft_dataFile << fftSin_orig << " " << fftCos_orig << std::endl ;
					std::cout << "Arg: " << arg << ", Sin: " << fftSin_orig << ", Cos: " << fftCos_orig << std::endl ;
				#endif


				// Non-approximate
				t_orig =  x_orig[indices[eI]] ;
				x_orig[indices[eI]].real = t_orig.real + (x_orig[indices[oI]].real * fftCos_orig - x_orig[indices[oI]].imag * fftSin_orig);
                x_orig[indices[eI]].imag = t_orig.imag + (x_orig[indices[oI]].imag * fftCos_orig + x_orig[indices[oI]].real * fftSin_orig);

                x_orig[indices[oI]].real = t_orig.real - (x_orig[indices[oI]].real * fftCos_orig - x_orig[indices[oI]].imag * fftSin_orig);
                x_orig[indices[oI]].imag = t_orig.imag - (x_orig[indices[oI]].imag * fftCos_orig + x_orig[indices[oI]].real * fftSin_orig);


                #ifndef NPU_OBSERVATION
                	t =  x[indices[eI]] ;
					x[indices[eI]].real = t.real + (x[indices[oI]].real * fftCos - x[indices[oI]].imag * fftSin);
                	x[indices[eI]].imag = t.imag + (x[indices[oI]].imag * fftCos + x[indices[oI]].real * fftSin);
	
                	x[indices[oI]].real = t.real - (x[indices[oI]].real * fftCos - x[indices[oI]].imag * fftSin);
                	x[indices[oI]].imag = t.imag - (x[indices[oI]].imag * fftCos + x[indices[oI]].real * fftSin);
                #endif
			}
		}
	}


	#ifdef NPU_ANALOG
		differenceFile.close() ;
	#endif

	#ifdef NPU_FANN
		differenceFile.close() ;
	#endif

	#ifdef NPU_OBSERVATION
		fft_dataFile.close() ;
	#endif


	for (int i = 0 ; i < K ; i++)
	{
		f_orig[i] = x_orig[indices[i]] ;
		f[i] = x[indices[i]] ;
	}
}