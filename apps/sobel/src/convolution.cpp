/*
 * convolution.cpp
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include "convolution.hpp"
#include <cmath>


static float kx[][3] =
		{
			{ -1, -2, -1 },
			{  0,  0,  0 },
			{  1,  2,  1 }
		} ;

static float ky[][3] =
		{
			{ -1, 0, 1 },
			{ -2, 0, 2 },
			{ -1, 0, 1 }
		} ;

float convolve(float w[][3], float k[][3])
{
	float r ;
	r = 0.0 ;
	for( int j = 0 ; j < 3 ; j++ )
		for ( int i = 0 ; i < 3 ; i++ )
		{
			r += w[i][j] * k[j][i] ;
		}
	return r ;
}

float sobel(float w[][3])
{
	float sx ;
	float sy ;
	float s  ;

	for(int i = 0 ; i < 3 ; i++)
		for(int j = 0 ; j < 3 ; j++)
			if(DEBUG) std::cout << "w_ " << i << j << ": " << w[i][j] << std::endl;
	sx = convolve(w, ky) ;
	if(DEBUG) std::cout << "sx: " << sx << std::endl ;
	sy = convolve(w, kx) ;
	if(DEBUG) std::cout << "sy: " << sy << std::endl ;

	s = sqrt(sx * sx + sy * sy) ;
	if (s >= (256 / sqrt(256 * 256 + 256 * 256)))
		s = 255 / sqrt(256 * 256 + 256 * 256);
	return s ;
}