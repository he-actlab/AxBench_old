#ifndef __ANPU_NEURALNETWORK_HPP__
#define __ANPU_NEURALNETWORK_HPP__

#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <vector>
#include <cmath>
#include <random>
#include <time.h>
#include <algorithm>

#include "layer.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "boost/lexical_cast.hpp"
#include "datatype.hpp"

//#define NN_DEBUG 

namespace anpu {

	class NeuralNetwork
	{
		public:

			NeuralNetwork()
			{
				this->neural_network_learningRate		= LEARNING_RATE ; // defined in datatype.hpp
				this->neural_network_desiredMSE			= DESIRED_ERROR ;
				this->neural_network_maxEpoch      		= MAX_EPOCH ;
				this->neural_network_rpropInitialStep 	= INITIAL_STEP ;
				this->neural_network_deltaMax 			= DELTA_MAX ;
				this->neural_network_deltaMin 			= DELTA_MIN;
				this->neural_network_sampling_rate		= 1 ;


				this->neural_network_trainType			= BP ;
				this->neural_network_approxType 		= FP ;

				this->neural_network_nIOBits			= 16 ;
				this->neural_network_nWBits				= 16 ;

				this->neural_network_isLog  			= true ;

				this->neural_network_maxMagWeight		= 0.0 ;


				// Mean Square Error
				this->neural_network_mseNum				= 0 ;
				this->neural_network_MseFp				= 0.0 ;
				this->neural_network_MseLpSc 			= 0.0 ;
				this->neural_network_MseLp 				= 0.0 ;
			}


			void 	setLearningRate				(double learningRate) ;
			double 	getLearningRate				( ) ;

			void   	setDesiredMSE				(double desiredMSE) ;
			double 	getDesiredMSE				( ) ;

			void	setMaxEpoch					(int inMaxEpoch) ;
			double 	getMaxEpoch					( ) ;

			void   	setRpropInitialStep			(double initStep) ;
			double 	getRpropInitialStep			( ) ;

			void	setDeltaMin 				(double deltaMin) ;
			double	getDeltaMin					( ) ;

			void  	setDeltaMax					(double deltaMax) ;
			double	getDeltaMax					( ) ;

			void	isLogging					(bool inIsLog) ;

			void 	setInputActivationFunction		(std::string activationName) ;
			void 	setHiddenActivationFunction		(std::string activationName) ;
			void 	setOutputActivationFunction		(std::string activationName) ;

			void 	setSamplingRate					(std::string inSamplingRate) ;
			void	setTrainingAlgorithm			(std::string inTrainingAlgorithm) ;
			void	setApproximationType			(std::string inApproxType) ;

			void	addLayer 				(LayerPtr& currLayer) ;
			void 	addBiasNeurons			( ) ;


			// Training Functions
			void 	feedForward 			() ;
			// BACKPROP functions
			void 	backpropagate			() ;
			void 	updateWeights 			() ;
			// RPROP functions
			void 	rpropUpdateWeights		() ;


			void normalizeTest(double min, double max) ;
			void normalizeTrain(double min, double max) ;

			// Mean Squared Error Functions
			void 	computeMSE				(TrainType tt) ;
			double	getMSE					(ApproxType tt) ;
			void 	clearMSE				( ) ;


			void 	runNeuralNetwork		(std::vector<double>& inInputs, std::vector<double>& outOutputs, int iBits, int wBits, ApproxType at) ;
			void    randomizeWeights        (double min, double max) ;
			void 	fillTrainingData		(std::string trainFileName) ;
			void 	fillTestingData			(std::string testFileName) ;
			void	shuffleTrainingSet		() ;
			void 	initializeNeuralNetwork	() ;
			void 	trainNeuralNetwork   	(std::string logFilename, int max_epoch_print) ;

			// scaling input and outputs
			void 	scaleInputs				(double minVal, double maxVal, double scMinVal, double scMaxVal) ;
			void  	scaleOutputs			(double minVal, double maxVal, double scMinVal, double scMaxVal) ;
			double 	scale 					(double oldVal, double minVal, double maxVal, double scMinVal, double scMaxVal) ;


			double testNN() ;


			// Configuration of neural network with different IO/Weight BITS
			void 	config					(int nIBitsStart, int nWBitsStart) ;
			void	scaleWeights 			() ;


			double 	test 					() ;



			// Save Neural Network
			void 	writeXML 				( std::string xmlFileName, ApproxType at) ;	


			// Debugging Functions
			void        printNeuralNetwork		() ;
			std::string printLog() ;
			std::string printInputActivationFunction() ;
			std::string printHiddensActivationFunction()	;
			std::string printOutputActivationFunction()	;

			void loadWeights() ;
			void saveWeights() ;


			// Public member
			std::vector<LayerPtr>				neural_network_layers ;
			double								neural_network_maxMagWeight ;
		private:
			double								neural_network_learningRate ;
			double								neural_network_desiredMSE ;
			int 								neural_network_maxEpoch ;
			double								neural_network_rpropInitialStep ;
			double								neural_network_deltaMin ;
			double								neural_network_deltaMax ;
			bool								neural_network_isLog ;
			double								neural_network_sampling_rate ;
			TrainType							neural_network_trainType ;
			ApproxType 							neural_network_approxType ;


			std::vector<std::vector<double> >	neural_network_inputData ;
			std::vector<double>					min_inputs;
			std::vector<double>					max_inputs;
			std::vector<std::vector<double> >	neural_network_outputData ;
			std::vector<double>					min_outputs;
			std::vector<double>					max_outputs;


			std::vector<std::vector<double> >	neural_network_testInputData ;
			std::vector<double>					min_test_inputs;
			std::vector<double>					max_test_inputs;
			std::vector<std::vector<double> >	neural_network_testOutputData ;
			std::vector<double>					min_test_outputs;
			std::vector<double>					max_test_outputs;

			
			int 								neural_network_mseNum ;
			double								neural_network_MseFp 	;
			double								neural_network_MseLp 	;
			double								neural_network_MseLpSc 	;
			

			

			int 								neural_network_nIOBits ;
			int 								neural_network_nWBits ;
	} ;
}

#endif
