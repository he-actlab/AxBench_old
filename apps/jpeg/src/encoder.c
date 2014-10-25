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




#ifdef NPU_SW
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



// std::cout << "BEFORE" << std::endl ;


// 	#ifdef RANDOM_DATA_COLLECTION

// 		std::cout << "WHAT" << std::endl ;
// 		std::ofstream randomData ;
// 		randomData.open("./train/jpeg_random.data");
// 		srand(time(NULL));

// 		randomData << "500000" << std::endl;
// 		randomData << "64" << std::endl;
// 		randomData << "64" << std::endl ;
		
// 		for(int i = 0 ; i < 500000 ; i++)
// 		{
// 			for(int j = 0 ; j < 64 ; j++)
// 			{
// 				Y1[j] = rand() % 256 ;
// 				Y1[j] = Y1[j] - 128 ;

// 				randomData << Y1[j] / 256.0 ;

// 				if(j != 63)
// 				{
// 					randomData << " " ;
// 				}
// 				else
// 				{
// 					randomData << "\n" ;
// 				}
// 			}

// 			// Operation
// 			//levelShift(Y1);
// 			dct(Y1);
// 			quantization(Y1, ILqt);



// 			for(int j = 0 ; j < 64 ; j++)
// 			{

// 				randomData << Temp[j] / 256.0 ;

// 				if(j != 63)
// 				{
// 					randomData << " " ;
// 				}
// 				else
// 				{
// 					randomData << "\n" ;
// 				}
// 			}
// 		}
// 		randomData.close() ;
// 		exit(1) ;
// 	#endif

	// #ifdef RANDOM_DATA_COLLECTION

	// 	dct_data = fopen("./train/dct.data", "w") ;
	// 	quantization_data = fopen("./train/quantization.data", "w") ;

	// 	/* Writing Marker Data */
	// 	outputBuffer = writeMarkers(outputBuffer, imageFormat, srcImage->w, srcImage->h);
	// 	int i;
	// 	int j;

	// 	for (i = 0 ; i < MAX_RANDOM ; i++)
	// 	{
	// 		for(j = 0 ; j < BLOCK_SIZE ;j++)
	// 		{
	// 			Y1[j] = rand() % 256 ;
	// 		}

	// 		encodeMcu(imageFormat, outputBuffer);
	// 	}
	// #else
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
	// #endif


	// #ifdef RANDOM_DATA_COLLECTION
	// 	fclose(dct_data);
	// 	fclose(quantization_data) ;
	// #endif
	/* Close Routine */
	closeBitstream(outputBuffer);

	return outputBuffer;
}

UINT8* encodeMcu(
	UINT32 imageFormat,
	UINT8 *outputBuffer
) {
	

// #ifdef RANDOM_DATA_COLLECTION
// 	levelShift(Y1);
// 	dct(Y1);
// 	quantization(Y1, ILqt);
// #endif
#ifdef NPU_NORMAL
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);

	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);

	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);

	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);

	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);

	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);

	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);

	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);
	// NPU_Q_INST(Y1[0]);

	//MAGIC_INST;
	levelShift(Y1);
	dct(Y1);
	quantization(Y1, ILqt);
	//MAGIC_INST;

	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);

	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);

	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);

	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);

	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);

	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);

	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);

	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
	// NPU_Q_INST(Temp[0]);
#endif //NPU_NORMAL
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
#endif //NPU_FANN

#ifdef NPU_SW


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

#endif //NPU_SW

	outputBuffer = huffman(1, outputBuffer);

	return outputBuffer;
}
