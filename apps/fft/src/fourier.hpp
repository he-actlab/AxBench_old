/*
 * fourier.hpp
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#ifndef __FOURIER_HPP__
#define __FOURIER_HPP__


#include "complex.hpp"
#include <iostream>

void radix2DitCooleyTykeyFft(int k, int* indices, Complex* x, Complex* f, Complex* x_orig, Complex* f_orig) ;

#endif