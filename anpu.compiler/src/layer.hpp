#ifndef __ANPU_LAYER_HPP__
#define __ANPU_LAYER_HPP__

#include <iostream>
#include <string>
#include <vector>

#include <boost/make_shared.hpp>


#include "activationfunction.hpp"
#include "neuron.hpp"

#include <algorithm>



namespace anpu
{
	class Neuron ;
	typedef boost::shared_ptr<Neuron> NeuronPtr ;

	class Layer {

		public:
			Layer (int inIndex ) {layer_index = inIndex ;}


			NeuronPtr 	addNeuron 							(int index, bool isBias) ;


			void		initializeLayer						( ) ;
			void 		evaluateNeurons						() ;
			void		printNeurons						( ) ;
			void 		setActivationFunction				(boost::shared_ptr<ActivationFunction> inActivationFunctionPtr);

			void		assignNeuronsValue					(std::vector<double>& inDataVector) ;
			void		assignNeuronsTargetValue			(std::vector<double>& inDataVector) ;
			void 		getNeuronsValue						(std::vector<double>& outDataVector, ApproxType at) ;


			void 		assignRandomValuesIncomingSynapses	(double min, double max) ;

			void		writeOutgoingSynapses				(std::ofstream& inFile, ApproxType at) ;
			std::string getActivationFunction				( ) ;


			void 		scaleWeights						( ) ;
			void 		config								(int nIOBits, int nIntWBits, int nFracWBits, ApproxType at) ;



			void		computeMSE							(double& totalFp, double& totalLp, double& totalLpSc, TrainType tt, int& mseNum) ;
			void		backpropagate						(double learningRate) ;




			void 		updateOutgoingWeights				( ) ;	
			void		rpropUpdateWeights					(double& maxMagWeight, double deltaMin, double deltaMax) ;



			void 		loadWeights() ;
			void		saveWeights() ;


			
			
 
			std::vector<NeuronPtr>					layer_neurons ;
			int layer_index ;
	} ;
}

#endif