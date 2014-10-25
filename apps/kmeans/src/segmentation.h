/*
 * segmentation.h
 *
 *  Created on: May 6, 2012
 *      Author: Hadi Esmaeilzadeh <hadianeh@cs.washington.edu>
 */

#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include "rgbimage.h"

typedef struct {
   float r;
   float g;
   float b;
   int n;
} Centroid;

typedef struct {
   int k;
   Centroid* centroids;
} Clusters;

int initClusters(Clusters* clusters, int k, float scale);
void segmentImage(RgbImage* image, Clusters* clusters, int n);
void freeClusters(Clusters* clusters);

#endif /* SEGMENTATION_H_ */
