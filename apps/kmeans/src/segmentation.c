/*
 * segmentation.c
 *
 *  Created on: May 6, 2012
 *      Author: Hadi Esmaeilzadeh <hadianeh@cs.washington.edu>
 */

#include "segmentation.h"
#include <stdio.h>
#include <stdlib.h>

#include "distance.h"

int initClusters(Clusters* clusters, int k, float scale) {
	int i;
	float x;

	clusters->centroids = (Centroid*)malloc(k * sizeof(Centroid));

	if (clusters == NULL) {
		printf("Warning: Oops! Cannot allocate memory for the clusters!\n");

		return 0;
	}

	clusters->k = k;
//	printf("===========================\n");
//	printf("k = %d\n", clusters->k);
	for (i = 0; i < clusters->k; i ++) {
		x = (((float)rand())/RAND_MAX) * scale;
		clusters->centroids[i].r = x;

		x = (((float)rand())/RAND_MAX) * scale;
		clusters->centroids[i].g = x;

		x = (((float)rand())/RAND_MAX) * scale;
		clusters->centroids[i].b = x;

		clusters->centroids[i].n = 0;

//		printf("c[%d] = (%f, %f, %f; %d)\n", i,
//			clusters->centroids[i].r,
//			clusters->centroids[i].g,
//			clusters->centroids[i].b,
//			clusters->centroids[i].n
//		);
	}
//	printf("===========================\n");

	return 1;
}

void freeClusters(Clusters* clusters) {
	if (clusters != NULL)
		free(clusters->centroids);
}

void segmentImage(RgbImage* image, Clusters* clusters, int n) {
	int i;
	int x, y;
	int c;

	for (i = 0; i < n; ++i) {
		for (y = 0; y < image->h; y++) {
			for (x = 0; x < image->w; x++) {
				assignCluster(&image->pixels[y][x], clusters);
			}
		}

		/** Recenter */
		for (c  = 0; c < clusters->k; ++c) {
			clusters->centroids[c].r = 0.;
			clusters->centroids[c].g = 0.;
			clusters->centroids[c].b = 0.;
			clusters->centroids[c].n = 0;
		}
		for (y = 0; y < image->h; y++) {
			for (x = 0; x < image->w; x++) {
				clusters->centroids[image->pixels[y][x].cluster].r += image->pixels[y][x].r;
				clusters->centroids[image->pixels[y][x].cluster].g += image->pixels[y][x].g;
				clusters->centroids[image->pixels[y][x].cluster].b += image->pixels[y][x].b;
				clusters->centroids[image->pixels[y][x].cluster].n += 1;
			}
		}
		for (c  = 0; c < clusters->k; ++c) {
			if (clusters->centroids[c].n != 0) {
				clusters->centroids[c].r = clusters->centroids[c].r / clusters->centroids[c].n;
				clusters->centroids[c].g = clusters->centroids[c].g / clusters->centroids[c].n;
				clusters->centroids[c].b = clusters->centroids[c].b / clusters->centroids[c].n;
			}
		}
	}

	for (y = 0; y < image->h; y++) {
		for (x = 0; x < image->w; x++) {
			image->pixels[y][x].r = clusters->centroids[image->pixels[y][x].cluster].r;
			image->pixels[y][x].g = clusters->centroids[image->pixels[y][x].cluster].g;
			image->pixels[y][x].b = clusters->centroids[image->pixels[y][x].cluster].b;
		}
	}

//	printf("===========================\n");
//	printf("k = %d\n", clusters->k);
//	for (i = 0; i < clusters->k; i ++) {
//		printf("c[%d] = (%f, %f, %f; %d)\n", i,
//			clusters->centroids[i].r,
//			clusters->centroids[i].g,
//			clusters->centroids[i].b,
//			clusters->centroids[i].n
//		);
//	}
//	printf("===========================\n");

}

