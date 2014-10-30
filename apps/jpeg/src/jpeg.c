/*
 * jpeg.c
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatype.h"
#include "jpegconfig.h"
#include "prototype.h"

#include "rgbimage.h"


#include "floatfann.h"

#include "neuralnetwork.hpp"
#include "xml_parser.hpp"

#define OUT_BUFFER_SIZE 500000 // in bytes


#ifdef RANDOM_DATA_COLLECTION
	#include "stdio.h"
	FILE* dct_data ;
	FILE* quantization_data ;
#endif

struct fann *ann;
boost::shared_ptr<anpu::NeuralNetwork> currNeuralNetworkPtr ;


#ifdef NPU_ANALOG
	int iBits ;
	int wBits ;
	ApproxType at ;

	bool isBeforeNoise;
	double meanBefore ;
	double sigmaBefore ;

	bool isAfterNoise;
	double meanAfter ;
	double sigmaAfter;

#endif

int main (int argc, const char* argv[]) {
	char inputFileName[32], outputFileName[32];
	UINT32 qualityFactor;
	UINT32 imageFormat;

	UINT8 *outputBuffer;
	FILE *fp;

	UINT8 *outputBufferPtr;

	strcpy(inputFileName, argv[1]);
	strcpy(outputFileName, argv[2]);

#ifdef NPU_FANN
	std::string nn = argv[3] ;
	ann = fann_create_from_file(nn.c_str());
	printf("# Creating the NN from the FANN configuration file...\n");
#endif //NPU_FANN

#ifdef NPU_ANALOG
	std::string nn = argv[3] ;
	iBits = atoi(argv[4]) ;
	wBits = atoi(argv[5]) ;
	int isFp  = atoi(argv[6]) ;
	
	if(isFp == 0)
		at = LP ;
	else
		at = FP ;

	anpu::XMLParser currXMLParser(nn, currNeuralNetworkPtr, false) ;
#endif

	qualityFactor = 1024;
	imageFormat = GRAY;


	RgbImage srcImage;
	initRgbImage(&srcImage);
	if (loadRgbImage(inputFileName, &srcImage) == 0) {
		printf("Error! Oops: Cannot load the input image!\n");
		return -1;
	}

	makeGrayscale(&srcImage);

	outputBuffer = (UINT8 *) malloc(OUT_BUFFER_SIZE * sizeof(UINT8));


	outputBufferPtr = outputBuffer;
	outputBufferPtr = encodeImage(
		&srcImage, outputBufferPtr, qualityFactor, imageFormat
	);


	freeRgbImage(&srcImage);

	fp = fopen(outputFileName, "wb");
	if (fp != NULL) {
		fwrite(outputBuffer, 1, outputBufferPtr - outputBuffer, fp);
		fclose(fp);
	}
	free(outputBuffer);
	return 0;
}

