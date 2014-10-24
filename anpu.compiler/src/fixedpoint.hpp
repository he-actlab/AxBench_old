#ifndef __FIXEDPOINT_HPP__
#define __FIXEDPOINT_HPP__

#include <iostream>
#include <math.h>
#include <string>



class FixedPoint {

protected:
		int nIntBits 	; 		// number of bits before the decimal (no sign)
		int nFracBits 	;	 	// number of bits after the decimal


		double	fp 		;		// full precision value
		long	fx 		;		// fixed point value
		double	fpedFx	;		// analog value (full precision internal ops)


public:


	FixedPoint() {
		nIntBits	=	0 ;
		nFracBits	=	0 ;
		fp 			=	0.0	;
		fx 			= 	0 ;
		fpedFx		=	0.0	;
	}


	void config(int nIntBits, int nFracBits, double val) 
	{
		this->nIntBits 	= nIntBits ;
		this->nFracBits	= nFracBits ;
		this->updateValue(val) ;
	}

	void setNIntBits(int nIntBits)
	{
		this->nIntBits = nIntBits ;

		// TODO: Adjust the value after changing the bit width
	}
	int getNIntBits() 
	{
		return this->nIntBits ;
	}

	void setNFracBits(int nFracBits)
	{
		this->nFracBits = nFracBits ;

		// TODO: Adjust the value after changing the bit width
	}
	int getNFracBits() 
	{
		return this->nFracBits ;
	}

	void setFp (double fpValue)
	{
		this->fp = fpValue ;
	}
	double getFp()
	{
		return fp ;
	}

	void setFpedFx(double fpValue)
	{
		this->fpedFx = fpValue ;
	}
	double getFpedFx()
	{
		return this->fpedFx ;
	}

	void setFx(long value)
	{
		this->fx = value ;
	}
	long getFx()
	{
		return this->fx ;
	}

	void updateFx(double value)
	{
		this->fx = int (value * pow((double)2, this->nFracBits)) ;
	}

	void updateFpedFx(double value) // updates based on nIntBits and nFracBits
	{

		// Fraction part
		this->fpedFx = double(int(value * pow((double)2.0, this->nFracBits))) / (pow((double)2.0, this->nFracBits)) ;

		// Integer part
		int sign = 1 ;
		if ( this->fpedFx < 0 )
		{
			sign = -1 ;
			this->fpedFx = this->fpedFx * sign ;
		}
		if ( this->fpedFx > pow((double)2.0, this->nIntBits)) // Saturate
		{
			this->fpedFx = pow((double)2.0, this->nIntBits) - 1 + (pow((double)2.0, this->nFracBits) - 1) / pow ((double)2.0, this->nFracBits) ;
		}

		this->fpedFx = this->fpedFx * sign ;
	}
	double getFxInFp()
	{
		return double(fx) / (pow((double)2.0, this->nFracBits)) ;
	}

	void setValue(double val)
	{
		setFp(val) ;
		setFpedFx(val) ;
		setFx(val) ;
	}

	void updateValue(double val)
	{
		setFp(val) ;
		updateFpedFx(val) ;
		updateFx(val) ;
	}



	friend FixedPoint operator+(FixedPoint& fixed0, FixedPoint& fixed1)
	{
		FixedPoint r ;

		r.fp = fixed0.fp + fixed1.fp ;
		r.fpedFx = fixed0.fpedFx + fixed1.fpedFx ;

		if(fixed0.nFracBits == fixed1.nFracBits)
		{
			r.fx = fixed0.fx + fixed1.fx ;
			r.nFracBits = fixed0.nFracBits ;
		}
		else if (fixed0.nFracBits > fixed1.nFracBits)
		{
			r.fx = fixed0.fx + fixed1.fx * (pow((double)2.0, fixed0.nFracBits - fixed1.nFracBits)) ;
			r.nFracBits = fixed0.nFracBits ;
		}
		else
		{
			r.fx = fixed1.fx + fixed0.fx * (pow((double)2.0, fixed1.nFracBits - fixed0.nFracBits)) ;
		}

		if(fixed0.nIntBits > fixed1.nIntBits)
		{
			r.nIntBits = fixed0.nIntBits ;
		}
		else if(fixed0.nIntBits < fixed1.nIntBits)
		{
			r.nIntBits = fixed1.nIntBits ;
		}
		else
		{
			r.nIntBits = fixed0.nIntBits + 1 ;
		}

		return r ;

	}


	friend FixedPoint operator*(FixedPoint& fixed0, FixedPoint& fixed1)
	{

		FixedPoint r ;

		r.fp = fixed0.fp * fixed1.fp ;

		r.fpedFx = fixed0.fpedFx * fixed1.fpedFx ;

		r.fx = fixed0.fx * fixed1.fx ;

		r.nFracBits = fixed0.nFracBits + fixed1.nFracBits ;
		r.nIntBits  = fixed0.nIntBits  + fixed1.nIntBits ;

		return r ;
	}

} ;





#endif
