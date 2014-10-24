#ifndef __ANPU_ACTIVATIONFUNCTION_HPP__
#define __ANPU_ACTIVATIONFUNCTION_HPP__

#include "stdlib.h"
#include <iostream>
#include <cmath>

#include "boost/shared_ptr.hpp"
#include "boost/algorithm/string.hpp"

#include "datatype.hpp"

// Fixed Point Type
#include "fixedpoint.hpp"

namespace anpu {

	class ActivationFunction {
		public:
			virtual ~ActivationFunction()
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Destructing the general activation function!" << std::endl ;
			}
			// Pure abstract member function to compute the output of the activation function
			virtual FixedPoint compute(FixedPoint x)  = 0 ;
			virtual double diffCompute(double x)  = 0 ; 
			virtual double diff2Compute(double x) = 0 ;
			std::string name ;
			double slope ;
			double shift ;
			friend std::ostream& operator<< (std::ostream& stream, const boost::shared_ptr< ActivationFunction >& activationFunction);
	} ;

	class Sigmoid : public ActivationFunction
	{
		public:
			Sigmoid(std::string name, double slope, double shift)
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Constructing Sigmoid activation function..." << std::endl ;
				this->name = name ;
				this->slope 	= slope ;
				this->shift 	= shift ;
			}
			~Sigmoid()
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Destructing Sigmoid activation function..." << std::endl ;
			}
			FixedPoint compute(FixedPoint x);
			double diffCompute(double x);
			double diff2Compute(double x) ;
	} ;


	class SigmoidAnalog : public ActivationFunction
	{
		public:
			SigmoidAnalog(std::string name, double slope, double shift)
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Constructing Sigmoid activation function..." << std::endl ;
				this->name = name ;
				this->slope 	= slope ;
				this->shift 	= shift ;
			}
			~SigmoidAnalog()
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Destructing Sigmoid activation function..." << std::endl ;
			}
			FixedPoint compute(FixedPoint x);
			double diffCompute(double x);
			double diff2Compute(double x) ;
	} ;

	class Linear : public ActivationFunction
	{
		public:
			Linear(std::string name, double slope, double shift)
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Constructing Linear activation function..." << std::endl ;
				this->name = name ; 
				this->slope 	= slope ;
				this->shift 	= shift ;
			}
			~Linear()
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Destructing Linear activation function..." << std::endl ;
			}
			FixedPoint compute(FixedPoint x);
			double diffCompute(double x);
			double diff2Compute(double x) ;

	} ;

	class SigmoidSymmetric : public ActivationFunction
	{
		public:
			SigmoidSymmetric(std::string name, double slope, double shift)
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Constructing Sigmoid Symmetric activation function..." << std::endl ; 
				this->name = name ;
				this->slope 	= slope ;
				this->shift 	= shift ;
			}
			~SigmoidSymmetric()
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Destructing Sigmoid Symmetric activation function..." << std::endl ; 
			}
			FixedPoint compute(FixedPoint x);
			double diffCompute(double x);
			double diff2Compute(double x) ;
	} ;


	class SigmoidSymmetricAnalog : public ActivationFunction
	{
		public:
			SigmoidSymmetricAnalog(std::string name, double slope, double shift)
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Constructing Sigmoid Symmetric Analog activation function..." << std::endl ; 
				this->name = name ;
				this->slope 	= slope ;
				this->shift 	= shift ;
			}
			~SigmoidSymmetricAnalog()
			{
				if(DEBUG_ACTIVATION_FUNCTION) std::cout << "# Destructing Sigmoid Symmetric Analog activation function..." << std::endl ; 
			}
			FixedPoint compute(FixedPoint x);
			double diffCompute(double x);
			double diff2Compute(double x) ;
	} ;

	class ActivationFunctionFactory
	{
		public:
			static boost::shared_ptr< ActivationFunction >  createActivationFunction(std::string name, double slope, 
																						double shift) ;
	} ;
}

#endif
