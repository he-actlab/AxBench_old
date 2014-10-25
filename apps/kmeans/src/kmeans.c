/*
 * kmeans.c
 *
 *  Created on: May 6, 2012
 *      Author: Hadi Esmaeilzadeh <hadianeh@cs.washington.edu>
 */

#include <stdio.h>
#include <string>
#include "rgbimage.h"
#include "segmentation.h"
#include <iostream>
 #include <fstream>
#include <sstream>


// #ifdef NPU_OBSERVATION
// FILE* dataFile;
// #endif //NPU_OBSERVATION

#ifdef NPU_FANN
#include "floatfann.h"
struct fann *ann;
#endif //NPU_FANN

#ifdef NPU_SW
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


	// #ifdef NPU_OBSERVATION
	// 	int totalNumbers = atoi(argv[1]);

	// 	std::ofstream outfile ;
	// 	outfile.open("./train/kmeans_random.data");
	// 	srand(time(NULL));
	// 	outfile.precision(8);

	// 	float r = 0;

	// 	outfile << totalNumbers << std::endl ;
	// 	outfile << "6" << std::endl ;
	// 	outfile << "1" << std::endl ;

	// 	for(int i  = 0 ; i < totalNumbers ; i++)
	// 	{	
	// 		float p1_r = (float)rand() / (double)RAND_MAX ;
	// 		float p1_g = (float)rand() / (double)RAND_MAX ;
	// 		float p1_b = (float)rand() / (double)RAND_MAX ;

	// 		float p2_r = (float)rand() / (double)RAND_MAX ;
	// 		float p2_g = (float)rand() / (double)RAND_MAX ;
	// 		float p2_b = (float)rand() / (double)RAND_MAX ;

	// 		outfile << p1_r << " " << p1_g << " " << p1_b << " " ;
	// 		outfile << p2_r << " " << p2_g << " " << p2_b <<std::endl;

	// 		r = 0;
	// 		r += (p1_r - p2_r) * (p1_r - p2_r);
	// 		r += (p1_g - p2_g) * (p1_g - p2_g);
	// 		r += (p1_b - p2_b) * (p1_b - p2_b);

	// 		outfile << r ;

	// 		if(i!=totalNumbers-1)
	// 		{
	// 			outfile << "\n";
	// 		}
	// 	}
	// 	outfile.close();
	// 	exit(1);
	// #endif


	RgbImage srcImage;
	Clusters clusters;

	initRgbImage(&srcImage);

	std::string inImageName  = argv[1] ;

	std::cout << "# Input image: " << inImageName << std::endl ;

	loadRgbImage(inImageName.c_str(), &srcImage, 256);

#ifdef NPU_FANN
	ann = fann_create_from_file(argv[2]);
	printf("NPU_FANN: /root/data/kmeans.nn\n");
#endif //NPU_FANN

#ifdef NPU_SW
	
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

	#ifdef NPU_SW
		outImageName << inImageName << "_sw.rgb" ;
	#endif

	#ifdef NPU_NORMAL
		outImageName << inImageName << "_orig.rgb" ;
	#endif

	std::cout << "# Output image: " << outImageName.str() << std::endl ;
	saveRgbImage(&srcImage, outImageName.str().c_str(), 255);

	freeRgbImage(&srcImage);
	freeClusters(&clusters);

	return 0;
}

