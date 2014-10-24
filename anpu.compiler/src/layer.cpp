#include "layer.hpp"

namespace anpu {

//#define LAYER_DEBUG

	NeuronPtr Layer::addNeuron (int index, bool isBias)
	{
		// Create the neuron pointer
		boost::shared_ptr<Neuron> currNeuronPtr(new Neuron(index, isBias)) ;
		this->layer_neurons.push_back(currNeuronPtr) ;
		return currNeuronPtr ;
	}


	void Layer::initializeLayer( )
	{
		std::vector<NeuronPtr>::iterator nIt ;

		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{
			(*nIt)->initializeNeuron() ;
		}
	}


	void Layer::evaluateNeurons()
	{
		std::vector<NeuronPtr>::iterator nIt ;

		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{
			(*nIt)->evaluate() ;
		}
	}


	void Layer::printNeurons()
	{

		int index = 0 ;
		std::vector<NeuronPtr>::iterator nIt ;

		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{
			std::cout << "\t" << (*nIt)->neuron_index << "\t" ;
			index++;
			(*nIt)->print() ;
		}

	}

	void Layer::setActivationFunction(boost::shared_ptr<ActivationFunction> inActivationFunctionPtr)
	{
		std::vector<NeuronPtr>::iterator nIt ;

		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{
			(*nIt)->setActivationFunction(inActivationFunctionPtr) ;
		}
	}

	void Layer::assignNeuronsValue(std::vector<double>& inDataVector)
	{	
		// We don't assign anything to bias neuron
		for(unsigned int index = 0 ;index < inDataVector.size() ; index++)
		{
			this->layer_neurons[index]->updateValue(inDataVector[index]) ;
		}
	}

	void Layer::assignNeuronsTargetValue(std::vector<double>& inDataVector)
	{
		
		// We don't assign anything to bias neuron
		for(unsigned int index = 0 ;index < inDataVector.size() ; index++)
		{
			this->layer_neurons[index]->setTargetValue(inDataVector[index]) ;
		}
	}
	void Layer::getNeuronsValue(std::vector<double>& outDataVector, ApproxType at)
	{
		unsigned int index = 0 ;
		outDataVector.clear() ;
		for(;index < this->layer_neurons.size() ; index++)
		{
			switch(at)
			{
				case FP:
				{
					outDataVector.push_back(this->layer_neurons[index]->getValueFp()) ;
					break ;
				}
				case LP:
				{
					outDataVector.push_back(this->layer_neurons[index]->getValueFpedFx()) ;
					break ;
				}
			}
			
		}
	}


	void Layer::assignRandomValuesIncomingSynapses(double min, double max)
	{

		std::vector<NeuronPtr>::iterator nIt ;

		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{
			(*nIt)->randomizeIncomingSynapses(min, max) ;
		}

	}

	void Layer::writeOutgoingSynapses(std::ofstream& inFile, ApproxType at)
	{
		std::vector<NeuronPtr>::iterator nIt ;

		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{
			(*nIt)->writeOutgoingSynapses(inFile, at) ;
		}

	}

	std::string Layer::getActivationFunction( )
	{
		return this->layer_neurons[0]->getActivationFunctionName() ;
	}

	void Layer::scaleWeights()
	{
		double mean = 0.5 ;
		double lambda ;
		double lambdaOld ;
		double weightAbs ;
		int first ;
		int num ;

		std::vector<NeuronPtr>::iterator nIt ;
		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{	

			lambda = 1 ;
			first  = 1 ;
			mean   = 0.5 ;


			while((mean > 0.5 || mean < 0.25 || first == 1))
			{

				first = 0 ;
				lambdaOld = lambda ;
				if(mean > 0.5)
				{
					lambda = lambdaOld * 2 ;
				}
				else if (mean < 0.25)
				{
					lambda = lambdaOld / 2 ;
				}

				mean = 0 ;
				num = 0 ;
				std::vector<SynapsePtr>::iterator inSynapseIt ;
				for(inSynapseIt = (*nIt)->beginIncomingSynapses() ; 
						inSynapseIt != (*nIt)->endIncomingSynapses() ; inSynapseIt++)
				{

					(*inSynapseIt)->updateWeight_Sc((lambdaOld/lambda) * (*inSynapseIt)->getWeight_Sc().getFp()) ;
					weightAbs = fann_abs((*inSynapseIt)->getWeight_Sc().getFp()) ;
					if(weightAbs > 0.1)
					{
						mean += weightAbs ;
						num++ ;
					}
				}

				mean = mean / num ;
			}

			(*nIt)->updateLambda(lambda) ;
		}
	}

	void Layer::config(int nIOBits, int nIntWBits, int nFracWBits, ApproxType at)
	{

		std::vector<NeuronPtr>::iterator nIt ;
		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{
			(*nIt)->config(nIOBits, nIntWBits, nFracWBits, at) ;
		}
	}

	// It is just for the output layer so there is no bias neuron
	// you have to compute MSE for all the neurons in the output layer
	void Layer::computeMSE(double& totalFp, double& totalLp, double& totalLpSc, TrainType tt, int& MseNum)
	{
		std::vector<NeuronPtr>::iterator nIt ;

		double diff ;
		double diffFp ;
		double diffLp ;
		double diffLpSc ;


		#ifdef LAYER_DEBUG
			std::cout << "TotalFp:   " << totalFp << std::endl;
			std::cout << "TotalLp:   " << totalLp << std::endl;
			std::cout << "TotalLpSc: " << totalLpSc << std::endl;
		#endif


		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end(); nIt++)
		{
			diffFp = ((*nIt)->getTargetValue()		- 	((*nIt)->getValue().getFp())) ;
			diffLp = ((*nIt)->getTargetValue()   	- 	((*nIt)->getValue().getFpedFx())) ;
			diffLpSc = ((*nIt)->getTargetValue() 	- 	((*nIt)->getValue_Sc().getFpedFx())) ;



			#ifdef LAYER_DEBUG
				std::cout << "diffFp:   " << diffFp << std::endl;
				std::cout << "diffLp:   " << diffLp << std::endl;
				std::cout << "diffLpSc: " << diffLpSc << std::endl;
			#endif



			if(boost::iequals((*nIt)->getActivationFunctionName(), "sigmoidsymmetric"))
			{
				diffFp 		/= 2. ;
				diffLp 		/= 2. ;
				diffLpSc 	/= 2. ;
			}

			totalFp += (diffFp * diffFp) ;
			totalLp += (diffLp * diffLp) ;
			totalLpSc += (diffLpSc * diffLpSc) ;
			MseNum++ ;

			switch (tt)
			{
				case BP:
					diff = diffFp ;
					break ;
				case RP:
					diff = diffFp ;
					break ;
				case CDLM:
					diff = diffLp ;
					break ;
				case CDLM_SC:
					diff = diffLpSc ;
					break ;
			}

		

			(*nIt)->setGradient(diff * (*nIt)->computeDiff((*nIt)->getValue().getFp())) ;

			#ifdef LAYER_DEBUG
				std::cout << "Value in: " << (*nIt)->getValue().getFp() << std::endl ;
				std::cout << "diff: " << diff << std::endl ;
				std::cout << "Gradient: " << (*nIt)->getGradient() << std::endl;
			#endif
			
		}
	}


	void Layer::backpropagate(double learningRate)
	{
		std::vector<NeuronPtr>::iterator nIt ;

		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{	
			std::vector<SynapsePtr>::iterator inSynapseIt ;

			double totalOutgoingErrors = 0.0 ;

			for(inSynapseIt = (*nIt)->beginOutgoingSynapses() ; 
					inSynapseIt != (*nIt)->endOutgoingSynapses() ; inSynapseIt++)
			{
				NeuronPtr dstNeuronPtr 		= (*inSynapseIt)->getDstNeuron() ;
				double   dstNeuronDelta 	= dstNeuronPtr->getGradient() ;

				double synapseCurrentGradient = dstNeuronDelta * (*nIt)->getValue().getFp() ;
		

				//std::cout << "Dst Delta:: " << dstNeuronDelta << std::endl ;
				//std::cout << "Neuron:: " << (*nIt)->getValue().getFp() << std::endl ;
				// std::cout << "Current Gradient:: " << (*inSynapseIt)->getCurrentGradient() << std::endl ;
				// std::cout << "Synapse Gradient:: " << synapseCurrentGradient << std::endl ;

				(*inSynapseIt)->setCurrentGradient((*inSynapseIt)->getCurrentGradient() + synapseCurrentGradient) ; // used for RPROP
				// std::cout << "Current Gradient:: " << (*inSynapseIt)->getCurrentGradient() << std::endl ;
				//std::string tempInput ;
				//std::getline(std::cin, tempInput);

				(*inSynapseIt)->setDeltaWeight(learningRate * synapseCurrentGradient); // used for BP


				// Meanwhile compute the delta of this neuron also
				totalOutgoingErrors += ((*inSynapseIt)->getWeight().getFp() * dstNeuronDelta);
			}

			// gradient = total outgoing weight * diff
			
			(*nIt)->setGradient(totalOutgoingErrors * (*nIt)->computeDiff((*nIt)->getValue().getFp())) ;

			//std::cout << "Get Gradient: " << (*nIt)->getGradient() << std::endl ;
			
			
		}
	}

	void Layer::rpropUpdateWeights (double& maxMagWeight, double deltaMin, double deltaMax)
	{

		double step ;
		double pStep ;
		double grad ;
		double pGrad ;
		double sameSign ;


		maxMagWeight = 0.0 ;
		int temp = 10 ; // nWBits

		std::vector<NeuronPtr>::iterator nIt ;
		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{	
			std::vector<SynapsePtr>::iterator inSynapseIt ;
			for(inSynapseIt = (*nIt)->beginOutgoingSynapses() ; 
					inSynapseIt != (*nIt)->endOutgoingSynapses() ; inSynapseIt++)
			{


				// Step may not be zero
				pStep = fann_max((*inSynapseIt)->getPreviousStep(), 0.0001) ;
				//std::cout << "Previous Step: " << (*inSynapseIt)->getPreviousStep() << std::endl ;
				grad  = (*inSynapseIt)->getCurrentGradient() ;
				pGrad = (*inSynapseIt)->getPreviousGradient() ;


				// 1) Adjust the step-sizes
				//std::cout << "Current Grad: " << grad << std::endl ;
				//std::cout << "Prev Grad: " << pGrad << std::endl ;
				sameSign = grad * pGrad ;

				//std::cout << "# Increase Factor: " << ETA_PLUS << std::endl ;
				//std::cout << "# Decrease Factor: " << ETA_MINUS << std::endl ;
				//std::cout << "# Delta Max:       " << deltaMax << std::endl ;

				if(sameSign >= 0.0)
				{
					step = fann_min(pStep * ETA_PLUS , deltaMax) ;
				}
				else
				{
					step = fann_max(pStep * ETA_MINUS, deltaMin) ;
					grad = 0.0 ;
				}

				//std::cout << "Step: " << step << std::endl ;
				//std::cout << "Grad: " << grad << std::endl ;
				// 2) Update the weights

				if(grad < 0.0)
				{
					(*inSynapseIt)->setWeight((*inSynapseIt)->getWeight().getFp() - step) ;

					if((*inSynapseIt)->getWeight().getFp() < -10.0)
						(*inSynapseIt)->setWeight(-10.0) ;

					(*inSynapseIt)->updateWeight_Sc((*inSynapseIt)->getWeight().getFp() - step) ;

					if( (*inSynapseIt)->getWeight().getFp() < ( -1 * pow((double)2, temp) + 1 ) )
					{
						(*inSynapseIt)->setWeight( (-1 * pow((double)2, temp) + 1) ) ;
						(*inSynapseIt)->updateWeight_Sc( (-1 * pow((double)2, temp) + 1) ) ;
					}
				}
				else //if(grad > 0.0)
				{	
					(*inSynapseIt)->setWeight((*inSynapseIt)->getWeight().getFp() + step) ;

					if((*inSynapseIt)->getWeight().getFp() > 10.0)
						(*inSynapseIt)->setWeight(10.0) ;

					(*inSynapseIt)->updateWeight_Sc((*inSynapseIt)->getWeight().getFp() + step) ;

					if( (*inSynapseIt)->getWeight().getFp() > (pow((double)2, temp) - 1 ) )
					{
						(*inSynapseIt)->setWeight( (pow((double)2, temp) - 1) ) ;
						(*inSynapseIt)->updateWeight_Sc( (pow((double)2, temp) - 1) ) ;
					}	
				}
				
				(*inSynapseIt)->setPreviousStep(step) ;
				(*inSynapseIt)->setPreviousGradient(grad) ;
				(*inSynapseIt)->setCurrentGradient(0.0) ;



				//std::cout << "Weight: " << (*inSynapseIt)->getWeight().getFp() << std::endl ;
				

				if(fann_abs((*inSynapseIt)->getWeight().getFp()) > maxMagWeight)
				{
					//std::cout << "Max: " << maxMagWeight << std::endl ;
					maxMagWeight = abs(int((*inSynapseIt)->getWeight().getFp())) ;
					//std::cout << "Max: " << maxMagWeight << std::endl ;
				}
			}
		}
	}


	void Layer::updateOutgoingWeights( )
	{
		std::vector<NeuronPtr>::iterator nIt ;

		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{	
			std::vector<SynapsePtr>::iterator inSynapseIt ;
			for(inSynapseIt = (*nIt)->beginOutgoingSynapses() ; 
					inSynapseIt != (*nIt)->endOutgoingSynapses() ; inSynapseIt++)
			{
				(*inSynapseIt)->setWeight((*inSynapseIt)->getWeight().getFp() + (*inSynapseIt)->getDeltaWeight()) ;
				(*inSynapseIt)->updateWeight_Sc((*inSynapseIt)->getWeight().getFp() + (*inSynapseIt)->getDeltaWeight()) ;
			}
		}
	}


	
	void Layer::loadWeights()
	{
		std::vector<NeuronPtr>::iterator nIt ;

		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{	
			std::vector<SynapsePtr>::iterator inSynapseIt ;
			for(inSynapseIt = (*nIt)->beginOutgoingSynapses() ; 
					inSynapseIt != (*nIt)->endOutgoingSynapses() ; inSynapseIt++)
			{
				(*inSynapseIt)->loadWeights() ;
			}
		}
	}

	void Layer::saveWeights()
	{

		std::vector<NeuronPtr>::iterator nIt ;

		for(nIt = this->layer_neurons.begin() ; nIt != this->layer_neurons.end() ; nIt++)
		{	
			std::vector<SynapsePtr>::iterator inSynapseIt ;
			for(inSynapseIt = (*nIt)->beginOutgoingSynapses() ; 
					inSynapseIt != (*nIt)->endOutgoingSynapses() ; inSynapseIt++)
			{
				(*inSynapseIt)->saveWeights() ;
			}
		}

	}
	


	

} //end namespace