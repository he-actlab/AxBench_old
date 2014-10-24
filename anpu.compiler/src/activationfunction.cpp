#include "activationfunction.hpp"

namespace anpu {

	#define AF_DEBUG 1

	boost::shared_ptr< ActivationFunction >  ActivationFunctionFactory::createActivationFunction(std::string name, double slope, double shift)
	{
		if(boost::iequals(name, "sigmoid"))
		{
			return ( boost::shared_ptr< ActivationFunction >(new Sigmoid("sigmoid", slope, shift)) ) ;
		}
		else if(boost::iequals(name, "sigmoidanalog"))
		{
			return ( boost::shared_ptr< ActivationFunction >(new SigmoidAnalog("sigmoidanalog", slope, shift)) ) ;
		}
		else if (boost::iequals(name, "linear"))
		{
			return ( boost::shared_ptr< ActivationFunction >(new Linear("linear", slope, shift)) ) ;
		}
		else if (boost::iequals(name, "sigmoidsymmetric"))
		{	
			return ( boost::shared_ptr< ActivationFunction >(new SigmoidSymmetric("sigmoidsymmetric", slope, shift)) ) ;
		}
		else if (boost::iequals(name, "sigmoidsymmetricanalog"))
		{	
			return ( boost::shared_ptr< ActivationFunction >(new SigmoidSymmetricAnalog("sigmoidsymmetric", slope, shift)) ) ;
		}
		else
		{
			std::cerr << "# Unrecognized activation function " << name << "!" << std::endl ;
			std::cerr << "# The prograpm will end. Please change the configuration file" << std::endl ;
			std::cerr << "# and re-run the program." << std::endl ;
			exit (EXIT_FAILURE);
		}
		return boost::shared_ptr< ActivationFunction >() ;
	}

	FixedPoint Sigmoid::compute(FixedPoint x)
	{
		FixedPoint y ;
		double val = 1.0f / (1.0f + exp(-2.0f * x.getFp())) ;


		y.setFp(val) ;
		val = 1.0f/ (1.0f + exp(-2.0f * x.getFpedFx())) ;
		y.setFpedFx(val) ;

		// Compute the Y = Sigmoid(slope * x + shift)
		return y ; 
	}

	double Sigmoid::diffCompute(double x)
	{
		//std::cout << "Slope: " << this->slope << std::endl ;
		//std::cout << "Slope: " << this->slope << std::endl ;
		//std::cout << "X: " << x << std::endl; 
		double val = 2.0f * this->slope * x * ( 1.0f  - x ) ;
		return val ;
	}


	double Sigmoid::diff2Compute(double x)
	{
		double val = 2.0f * this->slope * x * ( 1.0f  - x ) * (1.0f - 2 * x) ;
		return val ;
	}


	FixedPoint SigmoidAnalog::compute(FixedPoint x)
	{
		FixedPoint y ;


		double valFp ;
		double valFped ;

		double currentPointFp    = x.getFp() ;
		double currentPointFped  = x.getFpedFx() ;


		if (currentPointFp <= -10.0)
		{
			valFp 	= 0.0 ;
		}
		else if(currentPointFp > -10.0 && currentPointFp <= -0.6)
		{
			valFp 	= 0.5315 - sqrt(-0.027 * currentPointFp - 0.004) ;
		}
		else if(currentPointFp > -0.6 && currentPointFp <= 0.6)
		{
			valFp = 0.12 * currentPointFp + 0.5 ;
		}
		else if(currentPointFp > 0.6 && currentPointFp <= 10.0)
		{
			valFp = 0.47 + sqrt(-0.004 + 0.027 * currentPointFp) ;
		}
		else
		{
			valFp = 1.0 ;
		}
		y.setFp(valFp) ;


		if (currentPointFped <= -10.0)
		{
			valFped 	= 0.0 ;
		}
		else if(currentPointFped > -10.0 && currentPointFped <= -0.6)
		{
			valFped 	= 0.5315 - sqrt(-0.027 * currentPointFped - 0.004) ;
		}
		else if(currentPointFped > -0.6 && currentPointFped <= 0.6)
		{
			valFped = 0.12 * currentPointFped + 0.5 ;
		}
		else if(currentPointFped > 0.6 && currentPointFped <= 10.0)
		{
			valFped = 0.47 + sqrt(-0.004 + 0.027 * currentPointFped) ;
		}
		else
		{
			valFped = 1.0 ;
		}
		y.setFpedFx(valFped) ;

		return y ; 
	}

	double SigmoidAnalog::diffCompute(double x)
	{
		double val ;

		if(x > 0.015748 && x <= 0.4210)
		{
			val = 0.0135 / ( -x + 0.5315);
		}
		else if(x > 0.4210 && x <= 0.572)
		{
			val = 0.12;
		}
		else if(x > 0.572 && x <= 0.9857)
		{
			val = 0.0135 / (x - 0.47);
		}
		else
		{
			val = 0.0 ;
		}

		return val ;
		// double val = 2.0f * this->slope * x * ( 1.0f  - x ) ;
		// return val ;
	}


	double SigmoidAnalog::diff2Compute(double x)
	{
		double val = 4.0f * this->slope * this->slope * x * ( 1.0f  - x ) * (1.0f - 2 * x) ;
		return val ;
	}


	FixedPoint SigmoidSymmetric::compute(FixedPoint x)
	{
		FixedPoint y ;
		double val = 2.0f / (1.0f + exp(-2.0f * x.getFp())) - 1.0f ;
		y.setFp(val) ;
		val = 2.0f / (1.0f + exp(-2.0f * x.getFpedFx())) - 1.0f ;
		y.setFpedFx(val) ;
		return y ; 
	}

	double SigmoidSymmetric::diffCompute(double x)
	{

		double val = this->slope * (1.0f - (x * x));
		return val ;
	}

	double SigmoidSymmetric::diff2Compute(double x)
	{

		double val = this->slope * 2.0f * x * ( x * x - 1);
		return val ;
	}



	FixedPoint SigmoidSymmetricAnalog::compute(FixedPoint x)
	{
		FixedPoint y ;


		double valFp ;
		double valFped ;

		double currentPointFp    = x.getFp() ;
		double currentPointFped  = x.getFpedFx() ;


		if (currentPointFp <= -10.0)
		{
			valFp 	= -1.0 ;
		}
		else if(currentPointFp > -10.0 && currentPointFp <= -0.6)
		{
			valFp 	= 2 * (0.5315 - sqrt(-0.027 * currentPointFp - 0.004))  - 1.0 ;
		}
		else if(currentPointFp > -0.6 && currentPointFp <= 0.6)
		{
			valFp = 2 * ( 0.12 * currentPointFp + 0.5 ) - 1.0 ;
		}
		else if(currentPointFp > 0.6 && currentPointFp <= 10.0)
		{
			valFp = 2 * ( 0.47 + sqrt(-0.004 + 0.027 * currentPointFp) ) - 1.0 ;
		}
		else
		{
			valFp = 1.0 ;
		}
		y.setFp(valFp) ;


		if (currentPointFped <= -10.0)
		{
			valFped 	= -1.0 ;
		}
		else if(currentPointFped > -10.0 && currentPointFped <= -0.6)
		{
			valFped = 2 * (0.5315 - sqrt(-0.027 * currentPointFped - 0.004) )  - 1.0;
		}
		else if(currentPointFped > -0.6 && currentPointFped <= 0.6)
		{
			valFped = 2 * (0.12 * currentPointFped + 0.5) - 1.0 ;
		}
		else if(currentPointFped > 0.6 && currentPointFped <= 10.0)
		{
			valFped = 2 * (0.47 + sqrt(-0.004 + 0.027 * currentPointFped) ) - 1.0 ;
		}
		else
		{
			valFped = 1.0 ;
		}
		y.setFpedFx(valFped) ;

		return y ;
	}

	double SigmoidSymmetricAnalog::diffCompute(double x)
	{

		double val ;

		if(x > -0.968504 && x <= -0.158)
		{
			val = 2 * (0.0135 / ( -x + 0.5315));
		}
		else if(x > -0.158 && x <= 0.144)
		{
			val = 2 * (0.12);
		}
		else if(x > 0.144 && x <= 0.9714)
		{
			val = 2 * (0.0135 / (x - 0.47));
		}
		else
		{
			val = 0.0 ;
		}

		return val ;
	}

	double SigmoidSymmetricAnalog::diff2Compute(double x)
	{

		double val = this->slope * 2.0f * x * ( x * x - 1);
		return val ;
	}


	FixedPoint Linear::compute(FixedPoint x)
	{
		FixedPoint y ;
		double val = x.getFp() ;
		y.setFp(val) ;
		val = x.getFpedFx() ;
		y.setFpedFx(val) ;
		return y ;
	}

	double Linear::diffCompute(double x)
	{
		return this->slope ;
	}

	double Linear::diff2Compute(double x)
	{
		return 0.0f;
	}


	std::ostream& operator<< (std::ostream& stream, const boost::shared_ptr< ActivationFunction >& activationFunction)
	{
		stream << "# Function:  " << activationFunction->name << std::endl ;
		stream << "# Slope:     " << activationFunction->slope << std::endl ;
		stream << "# Shift:     " << activationFunction->shift << std::endl ;
		stream << "-----------------------------------------------" << std::endl ;
		return stream ;
	}

}