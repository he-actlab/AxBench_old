#ifndef __ANPU_SYNAPSE_HPP__
#define __ANPU_SYNAPSE_HPP__

#include "neuron.hpp"

#include "datatype.hpp"

namespace anpu {
	

	class Neuron ;
	typedef boost::shared_ptr<Neuron> 	NeuronPtr ;


	class Synapse {	

		public:

			Synapse(NeuronPtr srcPtr, NeuronPtr dstPtr, double weight)
			{
				synapse_srcNeuronPtr = srcPtr ;
				synapse_dstNeuronPtr = dstPtr ;
				synapse_weight.setValue(weight) ;

				// Rprop Initialization parameters
				synapse_previous_gradient 	= 0.0 ;
				synapse_current_gradient  	= 0.0 ;
				synapse_previous_step		= INITIAL_STEP ; // defined in datatype.hpp
			}


			void initializeSynapse() ;

			void config(int nIntBits, int nFracBits, double value) ;
			void config_Sc(int nIntBits, int nFracBits, double value) ;

			FixedPoint getWeight () ;
			void setWeight (double inWeight) ;

			FixedPoint getWeight_Sc() ;
			void updateWeight_Sc(double val) ;

			void addSrcNeuron(NeuronPtr& sourceNeuron) ;
			void addDstNeuron(NeuronPtr& destinNeuron) ; 
			NeuronPtr getSrcNeuron() ;
			NeuronPtr getDstNeuron() ;

			void setDeltaWeight(double deltaWeight) ;
			double getDeltaWeight() ;

			void setPreviousGradient(double inGradient) ;
			double getPreviousGradient() ;

			void setCurrentGradient(double inGradient) ;
			double getCurrentGradient() ;

			void setPreviousStep(double inStep) ;
			double getPreviousStep() ;	


			void saveWeights() ;
			void loadWeights() ;		

		private:
			FixedPoint						synapse_weight ;
			FixedPoint						synapse_weight_save ;
			FixedPoint						synapse_weight_Sc ;

			NeuronPtr						synapse_srcNeuronPtr ;
			NeuronPtr  						synapse_dstNeuronPtr ;


			// RPROP Variables
			double							synapse_previous_step ;
			double							synapse_previous_gradient ;
			double							synapse_current_gradient ;

			// BackProp Variables
			double							synapse_deltaWeight ;
			
	} ;
}


#endif