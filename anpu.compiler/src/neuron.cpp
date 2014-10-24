#include "neuron.hpp"


namespace anpu {

//#define NEURON_DEBUG 1


	void Neuron::initializeNeuron()
	{
		if(this->neuron_isBias == true)
		{
			this->neuron_value.setValue(1.0) ;
			this->neuron_value_Sc.setValue(1.0) ;
		}
		else
		{
			this->neuron_value.setValue(0.0) ;
			this->neuron_value_Sc.setValue(0.0) ;
		}

		this->neuron_lambda   = 1 ;
		this->neuron_gradient = 0.0 ;


		std::vector<SynapsePtr>::iterator sIt ;
		for(sIt = this->neuron_outgoingSynapses.begin() ; sIt != this->neuron_outgoingSynapses.end() ; sIt++)
		{
			(*sIt)->initializeSynapse() ;
		}


	}

	FixedPoint Neuron::getValue ()
	{
		if(this->neuron_isBias == true)
		{
			FixedPoint y ;
			y.setValue(1.0) ;
			return y;
		}
		else
			return this->neuron_value ;
	}
	FixedPoint Neuron::getValue_Sc ()
	{
		if(this->neuron_isBias == true)
		{
			FixedPoint y ;
			y.setValue(1.0) ;
			return y;
		}
		else
			return this->neuron_value_Sc ;
	}

	std::string Neuron::getActivationFunctionName()
	{
		return this->neuron_activationFunctionPtr->name ;
	}

	void Neuron::setActivationFunction	(boost::shared_ptr<ActivationFunction>& inActivationFunctionPtr)
	{
		this->neuron_activationFunctionPtr = inActivationFunctionPtr ;
	}

	void Neuron::writeOutgoingSynapses(std::ofstream& inFile, ApproxType at)
	{
		std::vector<SynapsePtr>::iterator sIt ;
		inFile.precision(22) ;

		for(sIt = this->neuron_outgoingSynapses.begin() ; sIt != this->neuron_outgoingSynapses.end() ; sIt++)
		{
			inFile << this->neuron_layerPtr->layer_index << " " ;
			inFile << this->neuron_index << " " ;

			NeuronPtr currDstNeuron = (*sIt)->getDstNeuron() ;
			inFile << currDstNeuron->neuron_layerPtr->layer_index << " " ;
			inFile << currDstNeuron->neuron_index << " " ;

			if(at == FP)
				inFile << (*sIt)->getWeight().getFp() << "\n" ;
			else if(at == LP)
				inFile << (*sIt)->getWeight().getFpedFx() << "\n" ;
			else if (at == LP_SC)
				inFile << (*sIt)->getWeight_Sc().getFpedFx() << "\n" ;
		}
	}

	double Neuron::getValueFpedFx ()
	{
		if(this->neuron_isBias == true)
		{
			return 1.0 ;
		}
		else
			return this->neuron_value.getFpedFx() ;
	}


	double	Neuron::getValueFp ()
	{
		if(this->neuron_isBias == true)
		{
			return 1.0 ;
		}
		else
			return this->neuron_value.getFp() ;
	}

 	void Neuron::setValue(double inValue)
	{
		if(this->neuron_isBias == true)
		{
			this->neuron_value.setValue(1.0) ;
		}
		else
		{
			this->neuron_value.setValue(inValue) ;
		}
	}

	void Neuron::updateValue(double value)
	{
		#ifdef NEURON_DEBUG
			 std::cout << "neuron value: " << this->neuron_value.getFp() << std::endl;
			 std::cout << "neuron_sc value: " << this->neuron_value_Sc.getFp() << std::endl;
		#endif

		this->neuron_value.updateValue(value) ;
		this->neuron_value_Sc.updateValue(value) ;

		#ifdef NEURON_DEBUG
			std::cout << "neuron value: " << this->neuron_value.getFp() << std::endl;
			std::cout << "neuron_sc value: " << this->neuron_value_Sc.getFp() << std::endl;
		#endif
		
		// if(NEURON_DEBUG)
		// {
		// 	std::string tempInput ;
		// 	std::getline(std::cin, tempInput);
		// }

	}


	void Neuron::setLayerPtr				(LayerPtr inLayerPtr)
	{
		this->neuron_layerPtr = inLayerPtr ;
	}
	void Neuron::addIncomingSynapse(SynapsePtr inSynapsePtr)
	{
		this->neuron_incomingSynapses.push_back(inSynapsePtr) ;
	}

	void Neuron::addOutgoingSynapse(SynapsePtr outSynapsePtr)
	{
		this->neuron_outgoingSynapses.push_back(outSynapsePtr) ;
	}

	// Compute the value of the Neuron
	void Neuron::evaluate()
	{
		// In case the neuron is either input or bias
		if (this->sizeIncomingSynapses() == 0)
		{
			if(this->neuron_value.getFp() != 1.0)
			{
				std::cerr << "Bias neurons should have the value 1.0" << std::endl;
				exit(0);
			}
			return;
		} 

		std::vector<SynapsePtr>::iterator synapseIt ;
		FixedPoint totalValue ;
		FixedPoint totalValue_Sc ;

		#ifdef NEURON_DEBUG
			std::cout << "totalValue: " << totalValue.getFp() << std::endl;
		#endif

		for(synapseIt = this->neuron_incomingSynapses.begin(); synapseIt != this->neuron_incomingSynapses.end() ; synapseIt++)
		{
			NeuronPtr currNeuronPtr = (*synapseIt)->getSrcNeuron() ;
			FixedPoint currNeuronValue = currNeuronPtr->getValue() ;
			FixedPoint currWeightValue = (*synapseIt)->getWeight() ;

			#ifdef NEURON_DEBUG
				{
					std::cout << "Neuron Value FP: " << currNeuronValue.getFp() 	<< std::endl;
					std::cout << "Neuron Value FpedFx: " << currNeuronValue.getFpedFx() << std::endl;

					std::cout << "Weight Value FP: " << currWeightValue.getFp() << std::endl;
					std::cout << "Weight Value FPedFx: " << currWeightValue.getFpedFx() << std::endl;
				}
			#endif
			
			FixedPoint multiplyValue   =  currNeuronValue * currWeightValue ;
			totalValue = totalValue + multiplyValue ;

			#ifdef NEURON_DEBUG
			{
				std::cout << "multiplyValue FP:     " << multiplyValue.getFp() << std::endl;
				std::cout << "multiplyValue FPedFx: " << multiplyValue.getFpedFx() << std::endl;
				std::cout << "Total Value FP:       " << totalValue.getFp() << std::endl ;
				std::cout << "Total Value FPedFx:   " << totalValue.getFpedFx() << std::endl ;
			}
			#endif



			FixedPoint currNeuronValue_Sc = currNeuronPtr->getValue_Sc() ;
			FixedPoint currWeightValue_Sc = (*synapseIt)->getWeight_Sc() ;
			FixedPoint multiplyValue_Sc   =  currNeuronValue_Sc * currWeightValue_Sc ;
			totalValue_Sc = totalValue_Sc + multiplyValue_Sc ;

		

		}



		// double sum = totalValue.getFp() * this->neuron_activationFunctionPtr->slope ;
		// double max_sum = 150 / this->neuron_activationFunctionPtr->slope ;

		// if(sum > max_sum)
		// {
		// 	//std::cout << "@@@@@@@@";
		// 	//exit(0) ;
		// 	sum = max_sum ;
		// }
		// else if(sum < -max_sum)
		// {
		// 	//std::cout << "32##@##@#";
		// 	//exit(0);
		// 	sum = -max_sum ;
		// }


		// totalValue.setValue(sum);


		// Apply the activationFunction
		FixedPoint ActivationValue = this->neuron_activationFunctionPtr->compute(totalValue) ;
		FixedPoint ActivationValue_Sc = this->neuron_activationFunctionPtr->compute(totalValue_Sc) ;
		//std::cout << "Current Value:: " << this->neuron_value.getFp() << std::endl ;
		//std::cout << "Activation:: "    << ActivationValue.getFp() << std::endl ;


		#ifdef NEURON_DEBUG
		{
			std::cout << "total Value final: " << totalValue.getFp() << std::endl ;
			std::cout << "total value final getFpedFx: " << totalValue.getFpedFx() << std::endl ;
			std::cout << "ActivationValue FP:   " << ActivationValue.getFp() << std::endl ;
			std::cout << "ActivationValue FPedFx:   " << ActivationValue.getFpedFx() << std::endl ;
			std::cout <<"--------------------------------------------------" << std::endl;
		}
		#endif


		// Update the neuron value and also quantize it
		this->neuron_value.setFp(ActivationValue.getFp()) ;
		this->neuron_value_Sc.setFp(ActivationValue_Sc.getFp()) ;

		#ifdef NEURON_DEBUG
			std::cout << "neuron value before updateFpedFx: " << ActivationValue.getFpedFx() << std::endl;
		#endif

		this->neuron_value.updateFpedFx(ActivationValue.getFpedFx()) ;
		this->neuron_value_Sc.updateFpedFx(ActivationValue_Sc.getFpedFx()) ;



		#ifdef NEURON_DEBUG
		{
			std::cout << "neuron value FP:     " << this->neuron_value.getFp() << std::endl ;
			std::cout << "neuron value FPedFx: " << this->neuron_value.getFpedFx() << std::endl ;
			std::string tempInput ;
			std::getline(std::cin, tempInput);
		}
		#endif

	}

	void Neuron::print()
	{
		std::cout << this->getValue().getFp() << "\t\t" << this->neuron_targetValue << std::endl ;
		//std::cout << this->neuron_activationFunctionPtr->name << "\n" ;
		std::vector<SynapsePtr>::iterator sIt ;
		for(sIt = this->neuron_incomingSynapses.begin() ; sIt != this->neuron_incomingSynapses.end() ; sIt++)
		{
			NeuronPtr srcNeuronPtr = (*sIt)->getSrcNeuron() ;
			FixedPoint srcNeuronValue = srcNeuronPtr->getValue() ;



			std::cout << "\t\t\t(" << (*sIt)->getSrcNeuron()->getLayerPtr()->layer_index ;
			std::cout << "," << (*sIt)->getSrcNeuron()->neuron_index << ") "  << "Value:: " << srcNeuronValue.getFp() << "\t";
			std::cout << "WeightFp:: " << (*sIt)->getWeight().getFp() << "\tMul:: " << ((*sIt)->getWeight().getFp() *  srcNeuronValue.getFp()) << std::endl;
		}
	}

	void Neuron::randomizeIncomingSynapses(double min, double max)
	{
		std::vector<SynapsePtr>::iterator sIt ;
		//srand(time()) ;
		for(sIt = this->neuron_incomingSynapses.begin() ; sIt != this->neuron_incomingSynapses.end() ; sIt++)
		{
			float random = min + (float)rand()/((float)RAND_MAX/(max - min));
			(*sIt)->setWeight(random) ;
		}
	}

	// Function related to connected synapses to this neuron
	inline unsigned int Neuron::sizeIncomingSynapses()
	{
		return neuron_incomingSynapses.size() ;
	}

	inline unsigned int Neuron::sizeOutgoingSynapses()
	{
		return neuron_outgoingSynapses.size() ;
	}

	std::vector<SynapsePtr>::iterator Neuron::beginOutgoingSynapses()
	{
		return this->neuron_outgoingSynapses.begin() ;
	}

	std::vector<SynapsePtr>::iterator Neuron::endOutgoingSynapses()
	{
		return this->neuron_outgoingSynapses.end() ;
	}

	std::vector<SynapsePtr>::iterator Neuron::beginIncomingSynapses()
	{
		return this->neuron_incomingSynapses.begin() ;
	}

	std::vector<SynapsePtr>::iterator Neuron::endIncomingSynapses()
	{
		return this->neuron_incomingSynapses.end() ;
	}

	// Functions related to the layer to which this neuron belongs
	void Neuron::assignLayerPtr(LayerPtr inLayerPtr)
	{
		this->neuron_layerPtr = inLayerPtr ;
	}


	inline NN_DATA Neuron::getActivationFunctionSlope() const
	{
		return this->neuron_activationFunctionPtr->slope ;
	}
	inline NN_DATA Neuron::getActivationFunctionShift() const
	{
		return this->neuron_activationFunctionPtr->shift ;
	}

	LayerPtr Neuron::getLayerPtr()
	{
		return this->neuron_layerPtr ;
	}
	void Neuron::setTargetValue(double targetValue)
	{
		this->neuron_targetValue = targetValue ;
	}

	double Neuron::getTargetValue() const
	{
		return this->neuron_targetValue ;
	}

	double Neuron::computeDiff(double x)
	{
		return this->neuron_activationFunctionPtr->diffCompute(x) ;
	}

	double Neuron::compute2Diff(double x)
	{
		return this->neuron_activationFunctionPtr->diff2Compute(x) ;
	}

	void Neuron::setDelta(double delta)
	{
		this->neuron_delta = delta ;
	}
	double Neuron::getDelta() const
	{
		return this->neuron_delta ;
	}

	

	

	void Neuron::setGradient(double inGradient)
	{
		this->neuron_gradient = inGradient ;
	}
	double Neuron::getGradient()
	{
		return this->neuron_gradient ;
	}

	void Neuron::config(int nIOBits, int nIntWBits, int nFracWBits, ApproxType at)
	{
		std::vector<SynapsePtr>::iterator sIt ;
		//std::cout << "IO: "<< nIOBits << std::endl ;

		for(sIt = this->neuron_outgoingSynapses.begin() ; sIt != this->neuron_outgoingSynapses.end() ; sIt++)
		{
			switch(at)
			{	
				case LP:
					(*sIt)->config(nIntWBits, nFracWBits, (*sIt)->getWeight().getFp()) ;
					break ;
				case LP_SC:
					(*sIt)->config_Sc(nIntWBits, nFracWBits, (*sIt)->getWeight().getFp()) ;
					break ;
			}

		}
		switch(at) {	
			case LP:
				this->neuron_value.config(1, nIOBits, this->neuron_value.getFp()) ;
				break ;
			case LP_SC:
				this->neuron_value_Sc.config(1, nIOBits, this->neuron_value_Sc.getFp()) ;
		}
	}

	void Neuron::updateLambda(double lambda)
	{
		neuron_lambda = lambda ;
	}
}
