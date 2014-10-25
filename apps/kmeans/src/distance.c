/*
 * distance.c
 *
 *  Created on: May 7, 2012
 *      Author: Hadi Esmaeilzadeh <hadianeh@cs.washington.edu>
 */

#include "distance.h"
#include <math.h>
#include <map>


// #ifdef NPU_OBSERVATION
// #include <stdio.h>
// extern FILE* dataFile;
// #endif //NPU_OBSERVATION

#ifdef NPU_FANN
#include "floatfann.h"
extern struct fann *ann;
fann_type parrotIn[3 + 3];
fann_type* parrotOut;
#endif //NPU_FANN

#ifdef NPU_SW
#include "neuralnetwork.hpp"
extern boost::shared_ptr<anpu::NeuralNetwork> currNeuralNetworkPtr ;
extern int iBits ;
extern int wBits ;
extern ApproxType at ;
extern bool isBeforeNoise ;
extern double meanBefore ;
extern double sigmaBefore ; 
extern bool isAfterNoise ;
extern double meanAfter ;
extern double sigmaAfter ;
#endif

float euclideanDistance(RgbPixel* p, Centroid* c1) {
	float r;

	r = 0;
	r += (p->r - c1->r) * (p->r - c1->r);
	r += (p->g - c1->g) * (p->g - c1->g);
	r += (p->b - c1->b) * (p->b - c1->b);

	return sqrt(r);
}

int pickCluster(RgbPixel* p, Centroid* c1) {
	float d1;

	d1 = euclideanDistance(p, c1);

	if (p->distance <= d1)
		return 0;

	return 1;
}

void assignCluster(RgbPixel* p, Clusters* clusters) {
	int i = 0;

	float d;


#ifdef NPU_FANN
	parrotIn[0] = p->r;
	parrotIn[1] = p->g;
	parrotIn[2] = p->b;

	parrotIn[3] = clusters->centroids[i].r;
	parrotIn[4] = clusters->centroids[i].g;
	parrotIn[5] = clusters->centroids[i].b;
	parrotOut = fann_run(ann, parrotIn);
	d = parrotOut[0];
#endif //NPU_FANN

#ifdef NPU_SW

	std::vector<double> inputData ;
	std::vector<double> outputData ;


	// Initialize input / output neurons
	inputData.push_back(p->r) ;
	inputData.push_back(p->g) ;
	inputData.push_back(p->b) ;

	inputData.push_back(clusters->centroids[i].r) ;
	inputData.push_back(clusters->centroids[i].g) ;
	inputData.push_back(clusters->centroids[i].b) ;
	currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;

	d = outputData[0] ;

#endif

#ifdef NPU_NORMAL
	d = euclideanDistance(p, &clusters->centroids[i]);
#endif

	p->distance = d;

	for(i = 1; i < clusters->k; ++i) {
		#ifdef NPU_FANN
				parrotIn[0] = p->r;
				parrotIn[1] = p->g;
				parrotIn[2] = p->b;

				parrotIn[3] = clusters->centroids[i].r;
				parrotIn[4] = clusters->centroids[i].g;
				parrotIn[5] = clusters->centroids[i].b;
				parrotOut = fann_run(ann, parrotIn);
				d = parrotOut[0];
		#endif //NPU_FANN


		#ifdef NPU_SW
			inputData.clear() ;


			// Initialize input / output neurons
			inputData.push_back(p->r) ;
			inputData.push_back(p->g) ;
			inputData.push_back(p->b) ;
		
			inputData.push_back(clusters->centroids[i].r) ;
			inputData.push_back(clusters->centroids[i].g) ;
			inputData.push_back(clusters->centroids[i].b) ;
			currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;
		
			d = outputData[0] ;
		#endif

		#ifdef NPU_NORMAL
				d = euclideanDistance(p, &clusters->centroids[i]);
		#endif

		if (d < p->distance) {
			p->cluster = i;
			p->distance = d;
		}
	}
}

