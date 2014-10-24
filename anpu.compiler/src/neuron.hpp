#ifndef __ANPU_NEURON_HPP__
#define __ANPU_NEURON_HPP__




// ANPU header files
#include "activationfunction.hpp"
#include "layer.hpp"
#include "synapse.hpp"
#include "datatype.hpp"
#include <random>
#include <time.h>
#include <iostream>
#include <fstream>


#include <random>


// Standard libraries
#include <vector>


// Boost Libraries
#include <boost/shared_ptr.hpp>

#include <chrono>
#include <ctime>


#define NOISE_BIAS	0.05


namespace anpu {

	class Layer ;
	class Synapse ;
	typedef boost::shared_ptr<Synapse> SynapsePtr ;
	typedef boost::shared_ptr<Layer> LayerPtr ;

}

namespace anpu {
	
	class Neuron {
		public:

			Neuron (int index, bool isBias)
			{
				this->neuron_index 					= index ;
				this->neuron_value.setValue(1.0) ;
				this->neuron_value_Sc.setValue(1.0) ;
				this->neuron_isBias					= isBias ;
				this->neuron_lambda					= 1 ;
				this->neuron_gradient				= 0.0 ;
			}


			void				initializeNeuron			() ;
			FixedPoint 			getValue 					() ;
			FixedPoint			getValue_Sc					() ;


			std::string 		getActivationFunctionName 	() ;
			void				setActivationFunction		(boost::shared_ptr<ActivationFunction>& inActivationFunctionPtr) ;
			

			void 				writeOutgoingSynapses		(std::ofstream& inFile, ApproxType at) ;


			double 				getValueFpedFx 			() ;
			double				getValueFp 				() ;
			void 				setValue 				(double value) ;
			void				updateValue				(double value) ;

			
			void				setLayerPtr				(LayerPtr inLayerPtr) ;


			


			// Set/Get Functions
			std::string 		getName () const ;
			void 				setName (std::string& inName) ;
			int 				getNeuronIndex() const ;
			void 				setNeuronIndex(int index) ;
			
			void 				addIncomingSynapse(SynapsePtr inSynapsePtr) ;
			void 				addOutgoingSynapse(SynapsePtr outSynapsePtr) ;
			void				evaluate() ;
			
			void				print() ;
			void 				randomizeIncomingSynapses(double min, double max) ;

			void				config(int nIOBits, int nIntWBits, int nFracWBits, ApproxType at) ;


			void setTargetValue(double targetValue) ;
			double getTargetValue() const ;

			// Function related to the connected synapses to this neuron
			unsigned int sizeIncomingSynapses() ;
			unsigned int sizeOutgoingSynapses() ;
			std::vector<SynapsePtr>::iterator beginOutgoingSynapses() ;
			std::vector<SynapsePtr>::iterator endOutgoingSynapses() ;
			std::vector<SynapsePtr>::iterator beginIncomingSynapses() ;
			std::vector<SynapsePtr>::iterator endIncomingSynapses() ;			

			// Functions related to the layer to which this neuron belongs 
			void assignLayerPtr(LayerPtr inLayerPtr) ;
			std::string getLayerName() ;
			LayerPtr    getLayerPtr() ;


			NN_DATA getActivationFunctionSlope() const ;
			NN_DATA getActivationFunctionShift() const ;
			NN_DATA getActivationFunctionThreshold() const ;
			
			double computeDiff(double x) ;
			double compute2Diff(double x) ;

			

			void setDelta(double delta) ;
			double getDelta() const ;

			


			void setGradient(double) ;
			double getGradient() ;

			void updateLambda(double lambda) ;


			int neuron_index ;


			FixedPoint								neuron_value_before_noise ;

		private:

			FixedPoint								neuron_value ;

			FixedPoint								neuron_value_Sc ;

			double									neuron_lambda ;


			double									neuron_noise_before ;
			
			LayerPtr								neuron_layerPtr ;
			double 									neuron_targetValue ;
			double									neuron_gradient ;
			NN_DATA									neuron_delta ;
			int 									neuron_inputNeuronsLimit ; 
			bool									neuron_isBias ;
 
			boost::shared_ptr<ActivationFunction> 	neuron_activationFunctionPtr ; 

			std::vector<SynapsePtr> 				neuron_incomingSynapses ; 
			std::vector<SynapsePtr> 				neuron_outgoingSynapses ;
	} ;
}

#endif
