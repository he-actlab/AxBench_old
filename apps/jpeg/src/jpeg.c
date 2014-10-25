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


#ifdef NPU_SW
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
	printf("NPU_FANN: nn/jpeg.nn\n");
#endif //NPU_FANN

#ifdef NPU_SW
	std::string nn = argv[3] ;
	iBits = atoi(argv[4]) ;
	wBits = atoi(argv[5]) ;
	int isFp  = atoi(argv[6]) ;
	
	if(isFp == 0)
		at = LP ;
	else
		at = FP ;

	anpu::XMLParser currXMLParser(nn, currNeuralNetworkPtr, false) ;
#endif //NPU_SW

	qualityFactor = 1024;
	imageFormat = GRAY;


	RgbImage srcImage;
	initRgbImage(&srcImage);
	if (loadRgbImage(inputFileName, &srcImage) == 0) {
		printf("Error! Oops: Cannot load the input image!\n");

		//ptlcall_kill();
		return -1;
	}

	makeGrayscale(&srcImage);

	outputBuffer = (UINT8 *) malloc(OUT_BUFFER_SIZE * sizeof(UINT8));

	/* Start the simulation */
	//ptlcall_switch_to_sim();
	//ptlcall_single_flush("-snapshot-now");
	//printf("Starting the simulation ...\n");
	//printf("The first magic instruction!\n");

	//MAGIC_INST_START;

	outputBufferPtr = outputBuffer;
	outputBufferPtr = encodeImage(
		&srcImage, outputBufferPtr, qualityFactor, imageFormat
	);

	//MAGIC_INST_STOP;
	//printf("The last magic instruction!\n");
	//printf("Stopping the simulation!\n");
	//ptlcall_single_flush("-snapshot-now");
	//ptlcall_single_flush("-stop");
	/* Stop the simulation */

	freeRgbImage(&srcImage);

	fp = fopen(outputFileName, "wb");
	if (fp != NULL) {
		fwrite(outputBuffer, 1, outputBufferPtr - outputBuffer, fp);
		fclose(fp);
	}
	free(outputBuffer);

	//ptlcall_kill();
	return 0;
}

