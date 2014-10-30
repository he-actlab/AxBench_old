/*
 * encoder.c
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include "datatype.h"
#include "jpegconfig.h"
#include "prototype.h"

#include "rgbimage.h"


#include "floatfann.h"
#include "neuralnetwork.hpp"



#ifdef RANDOM_DATA_COLLECTION
	extern FILE* dct_data ;
	extern FILE* quantization_data ;

	#define MAX_RANDOM 10000 
#endif


extern boost::shared_ptr<anpu::NeuralNetwork> currNeuralNetworkPtr ;

UINT8	Lqt [BLOCK_SIZE];
UINT8	Cqt [BLOCK_SIZE];
UINT16	ILqt [BLOCK_SIZE];
UINT16	ICqt [BLOCK_SIZE];

INT16	Y1 [BLOCK_SIZE];
INT16	Y2 [BLOCK_SIZE];
INT16	Y3 [BLOCK_SIZE];
INT16	Y4 [BLOCK_SIZE];
INT16	CB [BLOCK_SIZE];
INT16	CR [BLOCK_SIZE];
INT16	Temp [BLOCK_SIZE];
UINT32 lcode = 0;
UINT16 bitindex = 0;

INT16 global_ldc1;
INT16 global_ldc2;
INT16 global_ldc3;

fann_type parrotIn[BLOCK_SIZE];
fann_type* parrotOut;
extern struct fann *ann;


#ifdef NPU_ANALOG
	extern int iBits ;
	extern int wBits ;
	extern ApproxType at ;
	
	extern bool isBeforeNoise;
	extern double meanBefore ;
	extern double sigmaBefore ;
	
	extern bool isAfterNoise;
	extern double meanAfter ;
	extern double sigmaAfter;

#endif

UINT8* encodeImage(
	RgbImage* srcImage,
	UINT8 *outputBuffer,
	UINT32 qualityFactor,
	UINT32 imageFormat
) {
	

	global_ldc1 = 0;
	global_ldc2 = 0;
	global_ldc3 = 0;




	/** Quantization Table Initialization */
	initQuantizationTables(qualityFactor);

	srand(time(NULL));


	UINT16 i, j;
	/* Writing Marker Data */
	outputBuffer = writeMarkers(outputBuffer, imageFormat, srcImage->w, srcImage->h);
	for (i = 0; i < srcImage->h; i += 8) {
		for (j = 0; j < srcImage->w; j += 8) {
			readMcuFromRgbImage(srcImage, j, i, Y1);
			/* Encode the data in MCU */
			outputBuffer = encodeMcu(imageFormat, outputBuffer);
		}
	}

	/* Close Routine */
	closeBitstream(outputBuffer);

	return outputBuffer;
}

UINT8* encodeMcu(
	UINT32 imageFormat,
	UINT8 *outputBuffer
) {
	
#ifdef NPU_OBSERVATION
	levelShift(Y1);
	dct(Y1);
	quantization(Y1, ILqt);
#endif
#ifdef NPU_FANN
	levelShift(Y1);
	for (int i = 0; i < BLOCK_SIZE; ++i)
		parrotIn[i] = Y1[i] / 256.;

	parrotOut = fann_run(ann, (fann_type*)parrotIn);
	for (int i = 0; i < BLOCK_SIZE; ++i)
		//Temp[i] = Y1[i] * 256.;
		Temp[i] = parrotOut[i] * 256.;

	for(int i = 8 ; i < BLOCK_SIZE; i++)
		Temp[i] = 0.0 ;
#endif

#ifdef NPU_ANALOG
	levelShift(Y1);
 
	// create the input/output map to get the data
	std::vector<double> inputData ;
	std::vector<double> outputData ;

	// Initialize input / output neurons
	for(int i = 0 ; i < 64 ; i++)
	{
		inputData.push_back(Y1[i] / 256.);
	}

	currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;

	for(int i = 0 ; i < 64 ; i++)
	{
		Temp[i] = outputData[i] * 256.0 ;
	}
	for(int i = 4 ; i < BLOCK_SIZE; i++)
		Temp[i] = 0.0 ;

#endif

	outputBuffer = huffman(1, outputBuffer);

	return outputBuffer;
}
