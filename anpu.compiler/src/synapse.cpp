#include "synapse.hpp"

namespace anpu {
//#define W_DEBUG


	void Synapse::initializeSynapse()
	{
		// Rprop Initialization parameters
		synapse_previous_gradient 	= 0.0 ;
		synapse_current_gradient  	= 0.0 ;
		synapse_previous_step		= INITIAL_STEP ; // defined in datatype.hpp
	}

	void Synapse::config(int nIntBits, int nFracBits, double value)
	{

		// #ifdef W_DEBUG
		// 	std::cout << "Int bit: " << nIntBits << std::endl;
		// 	std::cout << "Frac bit: " << nFracBits << std::endl;
		// 	std::cout << "Val: "    << value << std::endl ;
		// #endif
		this->synapse_weight.config(nIntBits, nFracBits, value) ;

		// #ifdef W_DEBUG
		// 	std::cout << "Weight FP:   " << this->synapse_weight.getFp() << std::endl ;
		// 	std::cout << "Weight Fped: " << this->synapse_weight.getFpedFx() << std::endl;
		// 	std::cout << "Weight fix:  " << this->synapse_weight.getFx() << std::endl ;
		// #endif

	}
	void Synapse::config_Sc(int nIntBits, int nFracBits, double value)
	{
		this->synapse_weight_Sc.config(nIntBits, nFracBits, value) ;
	}

	FixedPoint Synapse::getWeight ()
	{
		return this->synapse_weight ;
	}
	void Synapse::setWeight (double inWeight)
	{
		this->synapse_weight.setValue(inWeight) ;
	}

	FixedPoint Synapse::getWeight_Sc()
	{
		return this->synapse_weight_Sc ;
	}
	void Synapse::updateWeight_Sc(double val)
	{
		this->synapse_weight_Sc.updateValue(val) ;
	}


	void Synapse::addSrcNeuron(NeuronPtr& sourceNeuron)
	{
		this->synapse_srcNeuronPtr = sourceNeuron ;
	}
	void Synapse::addDstNeuron(NeuronPtr& destinNeuron)
	{

		this->synapse_dstNeuronPtr = destinNeuron ;
	}
	NeuronPtr Synapse::getSrcNeuron ()
	{
		return this->synapse_srcNeuronPtr ;
	}
	NeuronPtr Synapse::getDstNeuron()
	{
		return this->synapse_dstNeuronPtr ;
	}

	void Synapse::setDeltaWeight(double deltaValue)
	{
		this->synapse_deltaWeight = deltaValue ;
	}
	double Synapse::getDeltaWeight()
	{
		return this->synapse_deltaWeight;
	}	

	void Synapse::setPreviousGradient(double inGradient)
	{
		this->synapse_previous_gradient = inGradient ;
	}
	double Synapse::getPreviousGradient()
	{
		return synapse_previous_gradient ;
	}

	void Synapse::setCurrentGradient(double inGradient)
	{
		this->synapse_current_gradient = inGradient ;
	}
			
	double Synapse::getCurrentGradient()
	{
		return this->synapse_current_gradient ;
	}

	void Synapse::setPreviousStep(double inStep)
	{
		this->synapse_previous_step = inStep ;
	}
	double Synapse::getPreviousStep()
	{
		return this->synapse_previous_step ;
	}	

	void Synapse::saveWeights()
	{
		this->synapse_weight_save = this->synapse_weight ;
	}
	void Synapse::loadWeights()
	{
		this->synapse_weight = this->synapse_weight_save ;
	}
}