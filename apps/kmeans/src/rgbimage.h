/*
 * rgbimage.h
 *
 *  Created on: May 1, 2012
 *      Author: Hadi Esmaeilzadeh <hadianeh@cs.washington.edu>
 */

#ifndef RGB_IMAGE_H_
#define RGB_IMAGE_H_

typedef struct {
   float r;
   float g;
   float b;
   int cluster;
   float distance;
} RgbPixel;

typedef struct {
   int w;
   int h;
   RgbPixel** pixels;
   char* meta;
} RgbImage;

void initRgbImage(RgbImage* image);
int loadRgbImage(const char* fileName, RgbImage* image, float scale);
int saveRgbImage(RgbImage* image, const char* fileName, float scale);
void freeRgbImage(RgbImage* image);

void makeGrayscale(RgbImage* rgbImage);

#endif /* RGB_IMAGE_H_ */
