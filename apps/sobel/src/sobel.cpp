/*
 * sobel.cpp
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include "neuralnetwork.hpp"
#include "xml_parser.hpp"
#include "rgb_image.hpp"
#include "convolution.hpp"
#include <iostream>
#include <cmath>

#ifdef NPU_FANN
 #include "floatfann.h"
#endif

int main ( int argc, const char* argv[])
{
	int x, y;
	float s;

	#ifndef NPU_OBSERVATION
		float s_orig;
	#endif

	// Source and destination image	
	boost::shared_ptr<Image> srcImagePtr(new Image());
	boost::shared_ptr<Image> dstImagePtr(new Image());

	float w[][3] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	} ;


	srcImagePtr->loadRgbImage( argv[1] ); // source image
	dstImagePtr->loadRgbImage( argv[1] ); // destination image

	// either NPU_ANALOG or NPU_FANN
	#ifndef NPU_OBSERVATION
		std::string nn_name = argv[2]; // neural network name
		boost::shared_ptr<Image> origImagePtr(new Image());
		origImagePtr->loadRgbImage( argv[1] );
	#endif

	#ifdef NPU_OBSERVATION
		std::string dataFileName = argv[2];
		std::ofstream dataFile;
		dataFile.open(dataFileName);
		dataFile.precision(8);
	#endif

	#ifdef NPU_FANN
		fann_type* parrotOut;
		struct fann *ann;
		ann = fann_create_from_file(nn_name.c_str());
		std::cout << "# Creating the NN from the FANN configuration file...";
	#endif

	#ifdef NPU_ANALOG
		// Create the Neural Network
		boost::shared_ptr<anpu::NeuralNetwork> currNeuralNetworkPtr;
		anpu::XMLParser currXMLParser(nn_name, currNeuralNetworkPtr, false);
		std::vector<double> inputData;
		std::vector<double> outputData;
	
		int iBits = atoi(argv[2]);
		int wBits = atoi(argv[3]);
		int isFp  = atoi(argv[4]);
		ApproxType at;
		if(isFp == 0)
		{	
			at = LP;
		}
		else
		{
			at = FP;
		}
	#endif


	srcImagePtr->makeGrayscale( ) ;

	#ifndef NPU_OBSERVATION
		origImagePtr->makeGrayscale( ) ;
	#endif

	std::cerr << "# The image is converted to GrayScale..." << std::endl ;
	y = 0 ;
	
	// Start performing Sobel operation
	for( x = 0 ; x < srcImagePtr->width ; x++ ) {
		HALF_WINDOW(srcImagePtr, x, y, w) ;

	#ifdef NPU_OBSERVATION
		s = sobel(w) ;
		dataFile << w[0][0] << " " ;
		dataFile << w[0][0] << " " ;
		dataFile << w[0][1] << " " ;
		dataFile << w[0][2] << " " ;
		dataFile << w[1][0] << " " ;
		dataFile << w[1][1] << " " ;
		dataFile << w[1][2] << " " ;
		dataFile << w[2][0] << " " ;
		dataFile << w[2][1] << " " ;
		dataFile << w[2][2] << std::endl ;
		dataFile << s << std::endl ;
	#endif

	#ifdef NPU_ANALOG
	
		inputData.clear() ;
	
		// Initialize the data for inputs
		inputData.push_back(w[0][0]) ;
		inputData.push_back(w[0][1]) ;
		inputData.push_back(w[0][2]) ;
		inputData.push_back(w[1][0]) ;
		inputData.push_back(w[1][1]) ;
		inputData.push_back(w[1][2]) ;
		inputData.push_back(w[2][0]) ;
		inputData.push_back(w[2][1]) ;
		inputData.push_back(w[2][2]) ;

		currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;

		s = outputData[0] ;
		s_orig = sobel(w) ;
	#endif

	#ifdef NPU_FANN
		parrotOut = fann_run(ann, (fann_type*)w) ;
		s = parrotOut[0] ;
		s_orig = sobel(w) ;
	#endif

		dstImagePtr->pixels[y][x]->r = s ;
		dstImagePtr->pixels[y][x]->g = s ;
		dstImagePtr->pixels[y][x]->b = s ;

	#ifndef NPU_OBSERVATION
		origImagePtr->pixels[y][x]->r = s_orig ;
		origImagePtr->pixels[y][x]->g = s_orig ;
		origImagePtr->pixels[y][x]->b = s_orig ;
	#endif

	}

	for (y = 1 ; y < (srcImagePtr->height - 1) ; y++) {
		x = 0 ;
		HALF_WINDOW(srcImagePtr, x, y, w) ;

		#ifdef NPU_OBSERVATION
			s = sobel(w) ;
			if(DEBUG)
				std::cerr << "Sobel1: " << s << std::endl ;
	
			dataFile << w[0][0] << " " ;
			dataFile << w[0][0] << " " ;
			dataFile << w[0][1] << " " ;
			dataFile << w[0][2] << " " ;
			dataFile << w[1][0] << " " ;
			dataFile << w[1][1] << " " ;
			dataFile << w[1][2] << " " ;
			dataFile << w[2][0] << " " ;
			dataFile << w[2][1] << " " ;
			dataFile << w[2][2] << std::endl ;
			dataFile << s << std::endl ;
		#endif

		#ifdef NPU_ANALOG
			inputData.clear() ;
	
			// Initialize the data for inputs
			inputData.push_back(w[0][0]) ;
			inputData.push_back(w[0][1]) ;
			inputData.push_back(w[0][2]) ;
			inputData.push_back(w[1][0]) ;
			inputData.push_back(w[1][1]) ;
			inputData.push_back(w[1][2]) ;
			inputData.push_back(w[2][0]) ;
			inputData.push_back(w[2][1]) ;
			inputData.push_back(w[2][2]) ;
			currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;
			s = outputData[0] ;
			s_orig = sobel(w) ;
		#endif

		#ifdef NPU_FANN
			parrotOut = fann_run(ann, (fann_type*)w) ;
			s = parrotOut[0] ;
			s_orig = sobel(w) ;
		#endif

		dstImagePtr->pixels[y][x]->r = s ;
		dstImagePtr->pixels[y][x]->g = s ;
		dstImagePtr->pixels[y][x]->b = s ;

		#ifndef NPU_OBSERVATION
			origImagePtr->pixels[y][x]->r = s_orig ;
			origImagePtr->pixels[y][x]->g = s_orig ;
			origImagePtr->pixels[y][x]->b = s_orig ;
		#endif

		for( x = 1 ; x < srcImagePtr->width - 1 ; x++ ) {
			WINDOW(srcImagePtr, x, y, w) ;

			#ifdef NPU_OBSERVATION
				s = sobel(w) ;
				dataFile << w[0][0] << " " ;
				dataFile << w[0][0] << " " ;
				dataFile << w[0][1] << " " ;
				dataFile << w[0][2] << " " ;
				dataFile << w[1][0] << " " ;
				dataFile << w[1][1] << " " ;
				dataFile << w[1][2] << " " ;
				dataFile << w[2][0] << " " ;
				dataFile << w[2][1] << " " ;
				dataFile << w[2][2] << std::endl ;
				dataFile << s << std::endl ;
			#endif

			#ifdef NPU_ANALOG
				inputData.clear() ;

				// Initialize the data for inputs
				inputData.push_back(w[0][0]) ;
				inputData.push_back(w[0][1]) ;
				inputData.push_back(w[0][2]) ;
				inputData.push_back(w[1][0]) ;
				inputData.push_back(w[1][1]) ;
				inputData.push_back(w[1][2]) ;
				inputData.push_back(w[2][0]) ;
				inputData.push_back(w[2][1]) ;
				inputData.push_back(w[2][2]) ;
		
				currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;
		
				s = outputData[0] ;
				s_orig = sobel(w) ;
			#endif

			#ifdef NPU_FANN
				parrotOut = fann_run(ann, (fann_type*)w) ;
				s = parrotOut[0] ;
				s_orig = sobel(w) ;
			#endif

			dstImagePtr->pixels[y][x]->r = s ;
			dstImagePtr->pixels[y][x]->g = s ;
			dstImagePtr->pixels[y][x]->b = s ;

			#ifndef NPU_OBSERVATION
				origImagePtr->pixels[y][x]->r = s_orig ;
				origImagePtr->pixels[y][x]->g = s_orig ;
				origImagePtr->pixels[y][x]->b = s_orig ;
			#endif
		}
		x = srcImagePtr->width - 1 ;
		HALF_WINDOW(srcImagePtr, x, y, w) ;

		#ifdef NPU_OBSERVATION
			s = sobel(w) ;
			dataFile << w[0][0] << " " ;
			dataFile << w[0][0] << " " ;
			dataFile << w[0][1] << " " ;
			dataFile << w[0][2] << " " ;
			dataFile << w[1][0] << " " ;
			dataFile << w[1][1] << " " ;
			dataFile << w[1][2] << " " ;
			dataFile << w[2][0] << " " ;
			dataFile << w[2][1] << " " ;
			dataFile << w[2][2] << std::endl ;
			dataFile << s << std::endl ;
		#endif

		#ifdef NPU_ANALOG
			inputData.clear() ;
			// Initialize the data for inputs
			inputData.push_back(w[0][0]) ;
			inputData.push_back(w[0][1]) ;
			inputData.push_back(w[0][2]) ;
			inputData.push_back(w[1][0]) ;
			inputData.push_back(w[1][1]) ;
			inputData.push_back(w[1][2]) ;
			inputData.push_back(w[2][0]) ;
			inputData.push_back(w[2][1]) ;
			inputData.push_back(w[2][2]) ;
	
			currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;
	
			s = outputData[0] ;
			s_orig = sobel(w) ;
		#endif

		#ifdef NPU_FANN
			parrotOut = fann_run(ann, (fann_type*)w) ;
			s = parrotOut[0] ;
			s_orig = sobel(w) ;
		#endif

		dstImagePtr->pixels[y][x]->r = s ;
		dstImagePtr->pixels[y][x]->g = s ;
		dstImagePtr->pixels[y][x]->b = s ;

		#ifndef NPU_OBSERVATION
			origImagePtr->pixels[y][x]->r = s_orig ;
			origImagePtr->pixels[y][x]->g = s_orig ;
			origImagePtr->pixels[y][x]->b = s_orig ;
		#endif
	}

	y = srcImagePtr->height - 1;

	for(x = 0 ; x < srcImagePtr->width ; x++) {
		HALF_WINDOW(srcImagePtr, x, y, w) ;

		#ifdef NPU_OBSERVATION
			s = sobel(w) ;
			dataFile << w[0][0] << " " ;
			dataFile << w[0][0] << " " ;
			dataFile << w[0][1] << " " ;
			dataFile << w[0][2] << " " ;
			dataFile << w[1][0] << " " ;
			dataFile << w[1][1] << " " ;
			dataFile << w[1][2] << " " ;
			dataFile << w[2][0] << " " ;
			dataFile << w[2][1] << " " ;
			dataFile << w[2][2] << std::endl ;
	
			dataFile << s << std::endl ;
		#endif

		#ifdef NPU_ANALOG
			inputData.clear() ;
	
			// Initialize the data for inputs
			inputData.push_back(w[0][0]) ;
			inputData.push_back(w[0][1]) ;
			inputData.push_back(w[0][2]) ;
			inputData.push_back(w[1][0]) ;
			inputData.push_back(w[1][1]) ;
			inputData.push_back(w[1][2]) ;
			inputData.push_back(w[2][0]) ;
			inputData.push_back(w[2][1]) ;
			inputData.push_back(w[2][2]) ;
	
			currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;
	
			s = outputData[0] ;
			s_orig = sobel(w) ;
		#endif

		#ifdef NPU_FANN
			parrotOut = fann_run(ann, (fann_type*)w) ;
			s = parrotOut[0] ;
			s_orig = sobel(w) ;
		#endif
		dstImagePtr->pixels[y][x]->r = s ;
		dstImagePtr->pixels[y][x]->g = s ;
		dstImagePtr->pixels[y][x]->b = s ;


		#ifndef NPU_OBSERVATION
			origImagePtr->pixels[y][x]->r = s_orig ;
			origImagePtr->pixels[y][x]->g = s_orig ;
			origImagePtr->pixels[y][x]->b = s_orig ;
		#endif
	}

#ifdef NPU_ANALOG
	std::string inFileName = argv[1] ;
	std::string outFile = inFileName + std::string("_analog.rgb") ;
	std::string origFileName = inFileName + std::string("_orig.rgb") ;
	dstImagePtr->saveRgbImage(outFile, 256) ;
	origImagePtr->saveRgbImage(origFileName, std::sqrt(256 * 256 + 256 * 256));
#endif

#ifdef NPU_FANN
	std::string inFileName = argv[1] ;
	std::string outFile = inFileName+ std::string("_fann.rgb") ;
	std::string origFileName = inFileName + std::string("_orig.rgb") ;
	dstImagePtr->saveRgbImage(outFile, 256) ;
	origImagePtr->saveRgbImage(origFileName, std::sqrt(256 * 256 + 256 * 256));
#endif

#ifdef NPU_OBSERVATION
	dstImagePtr->saveRgbImage("./data/observation.rgb", std::sqrt(256 * 256 + 256 * 256)) ;
	dataFile.close() ;
#endif
	
	return 0 ;
}