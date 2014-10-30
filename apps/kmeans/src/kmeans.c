/*
 * kmeans.c
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */


#include <stdio.h>
#include <string>
#include "rgbimage.h"
#include "segmentation.h"
#include <iostream>
#include <fstream>
#include <sstream>



#ifdef NPU_FANN
	#include "floatfann.h"
	struct fann *ann;
#endif

#ifdef NPU_ANALOG
	#include "neuralnetwork.hpp"
	#include "xml_parser.hpp"
	boost::shared_ptr<anpu::NeuralNetwork> currNeuralNetworkPtr ;
	int iBits ;
	int wBits ;
	ApproxType at ;
	bool isBeforeNoise ;
	double meanBefore ;
	double sigmaBefore ;
	
	bool isAfterNoise ;
	double meanAfter ;
	double sigmaAfter ;
#endif

int main (int argc, const char* argv[]) {

	RgbImage srcImage;
	Clusters clusters;

	initRgbImage(&srcImage);

	std::string inImageName  = argv[1] ;

	loadRgbImage(inImageName.c_str(), &srcImage, 256);

#ifdef NPU_FANN
	ann = fann_create_from_file(argv[2]);
	printf("# Creating the NN from the FANN configuration file...");
#endif

#ifdef NPU_ANALOG
	iBits = atoi(argv[3]) ;
	wBits = atoi(argv[4]) ;
	int isFp = atoi(argv[5]) ;

	if(isFp == 0)
	{
		at = LP ;
	}
	else
	{
		at = FP ;
	}
	anpu::XMLParser currXMLParser(argv[2], currNeuralNetworkPtr, false) ;
#endif

	initClusters(&clusters, 6, 1);

	segmentImage(&srcImage, &clusters, 1);
	
	std::stringstream outImageName ;
	
	#ifdef NPU_FANN
		outImageName << inImageName  << "_fann.rgb" ;
	#endif

	#ifdef NPU_ANALOG
		outImageName << inImageName << "_analog.rgb" ;
	#endif

	#ifdef NPU_OBSERVATION
		outImageName << inImageName << "_orig.rgb" ;
	#endif

	saveRgbImage(&srcImage, outImageName.str().c_str(), 255);

	freeRgbImage(&srcImage);
	freeClusters(&clusters);

	return 0;
}

