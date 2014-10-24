#include "neuralnetwork.hpp"


namespace anpu {

	void NeuralNetwork::setLearningRate(double learningRate)
	{
		this->neural_network_learningRate = learningRate ;
	}
	double NeuralNetwork::getLearningRate( )
	{
		return this->neural_network_learningRate ;
	}

	void NeuralNetwork::setDesiredMSE (double inDesiredMSE)		
	{ 
		this->neural_network_desiredMSE	= inDesiredMSE ; 
	}
	double NeuralNetwork::getDesiredMSE ( )
	{
		return this->neural_network_desiredMSE ;
	}

	void NeuralNetwork::setMaxEpoch	(int inMaxEpoch) 
	{ 
		this->neural_network_maxEpoch 	= inMaxEpoch ; 
	}
	double NeuralNetwork::getMaxEpoch ()
	{
		return this->neural_network_maxEpoch ;
	}

	void NeuralNetwork::setRpropInitialStep(double initStep)
	{
		this->neural_network_rpropInitialStep = initStep ;
	}
	double NeuralNetwork::getRpropInitialStep( )
	{
		return this->neural_network_rpropInitialStep ;
	}

	void NeuralNetwork::setDeltaMin(double deltaMin)
	{
		this->neural_network_deltaMin = deltaMin ;
	}

	double NeuralNetwork::getDeltaMin( )
	{
		return this->neural_network_deltaMin ;
	}

	void NeuralNetwork::setDeltaMax(double deltaMax)
	{
		this->neural_network_deltaMax = deltaMax ;
	}
	double NeuralNetwork::getDeltaMax( )
	{
		return this->neural_network_deltaMax ;
	}

	void NeuralNetwork::isLogging(bool inIsLog)
	{
		this->neural_network_isLog = inIsLog ;
	}

	void NeuralNetwork::setInputActivationFunction		(std::string activationName)
	{

		double slope ;
		double shift ;

		if(boost::iequals(activationName, "sigmoid"))
		{
			slope = SIGMOID_SLOPE_INPUT_LAYER ;
			shift = SIGMOID_SHIFT_INPUT_LAYER ;
		}
		else if (boost::iequals(activationName, "linear"))
		{
			slope = LINEAR_SLOPE_INPUT_LAYER ;
			shift = LINEAR_SHIFT_INPUT_LAYER ;
		}
		else if (boost::iequals(activationName, "sigmoidsymmetric"))
		{	
			slope = SYMMETRIC_SLOPE_INPUT_LAYER ;
			shift = SYMMETRIC_SHIFT_INPUT_LAYER ;
		}
		else if (boost::iequals(activationName, "sigmoidanalog"))
		{	
			slope = SYMMETRIC_SLOPE_INPUT_LAYER ;
			shift = SYMMETRIC_SHIFT_INPUT_LAYER ;
		}
		else if (boost::iequals(activationName, "sigmoidsymmetricanalog"))
		{	
			slope = SYMMETRIC_SLOPE_INPUT_LAYER ;
			shift = SYMMETRIC_SHIFT_INPUT_LAYER ;
		}
		else
		{
			std::cerr << "# Unrecognized activation function " << activationName << "!" << std::endl ;
			std::cerr << "# The prograpm will end. Please change the configuration file" << std::endl ;
			std::cerr << "# and re-run the program." << std::endl ;
			exit (EXIT_FAILURE);
		}

		boost::shared_ptr<ActivationFunctionFactory> currActivationFunctionPtr(new ActivationFunctionFactory()) ; 
		this->neural_network_layers[0]->setActivationFunction(currActivationFunctionPtr->createActivationFunction(activationName, slope, shift)) ;
	}


	void NeuralNetwork::setHiddenActivationFunction		(std::string activationName)
	{

		double slope ;
		double shift ;

		if(boost::iequals(activationName, "sigmoid"))
		{
			slope = SIGMOID_SLOPE_HIDDEN_LAYER ;
			shift = SIGMOID_SHIFT_HIDDEN_LAYER ;
		}
		else if (boost::iequals(activationName, "linear"))
		{
			slope = LINEAR_SLOPE_HIDDEN_LAYER ;
			shift = LINEAR_SHIFT_HIDDEN_LAYER ;
		}
		else if (boost::iequals(activationName, "sigmoidsymmetric"))
		{	
			slope = SYMMETRIC_SLOPE_HIDDEN_LAYER ;
			shift = SYMMETRIC_SHIFT_HIDDEN_LAYER ;
		}
		else if (boost::iequals(activationName, "sigmoidanalog"))
		{	
			slope = SYMMETRIC_SLOPE_HIDDEN_LAYER ;
			shift = SYMMETRIC_SHIFT_HIDDEN_LAYER ;
		}
		else if (boost::iequals(activationName, "sigmoidsymmetricanalog"))
		{	
			slope = SYMMETRIC_SLOPE_INPUT_LAYER ;
			shift = SYMMETRIC_SHIFT_INPUT_LAYER ;
		}
		else
		{
			std::cerr << "# Unrecognized activation function " << activationName << "!" << std::endl ;
			std::cerr << "# The prograpm will end. Please change the configuration file" << std::endl ;
			std::cerr << "# and re-run the program." << std::endl ;
			exit (EXIT_FAILURE);
		}

		for(unsigned int index = 1 ;  index < this->neural_network_layers.size() - 1 ; index++)
		{
			boost::shared_ptr<ActivationFunctionFactory> currActivationFunctionPtr(new ActivationFunctionFactory()) ; 
			this->neural_network_layers[index]->setActivationFunction(currActivationFunctionPtr->createActivationFunction(activationName, slope, shift)) ;
		}
	}


	void NeuralNetwork::setOutputActivationFunction		(std::string activationName) 
	{
		double slope ;
		double shift ;

		if(boost::iequals(activationName, "sigmoid"))
		{
			slope = SIGMOID_SLOPE_OUTPUT_LAYER ;
			shift = SIGMOID_SHIFT_OUTPUT_LAYER ;
		}
		else if (boost::iequals(activationName, "linear"))
		{
			slope = LINEAR_SLOPE_OUTPUT_LAYER ;
			shift = LINEAR_SHIFT_OUTPUT_LAYER ;
		}
		else if (boost::iequals(activationName, "sigmoidsymmetric"))
		{	
			slope = SYMMETRIC_SLOPE_OUTPUT_LAYER ;
			shift = SYMMETRIC_SHIFT_OUTPUT_LAYER ;
		}
		else if (boost::iequals(activationName, "sigmoidanalog"))
		{	
			slope = SYMMETRIC_SLOPE_OUTPUT_LAYER ;
			shift = SYMMETRIC_SHIFT_OUTPUT_LAYER ;
		}
		else if (boost::iequals(activationName, "sigmoidsymmetricanalog"))
		{	
			slope = SYMMETRIC_SLOPE_INPUT_LAYER ;
			shift = SYMMETRIC_SHIFT_INPUT_LAYER ;
		}
		else
		{
			std::cerr << "#  activation function " << activationName << "!" << std::endl ;
			std::cerr << "# The prograpm will end. Please change the configuration file" << std::endl ;
			std::cerr << "# and re-run the program." << std::endl ;
			exit (EXIT_FAILURE);
		}
		boost::shared_ptr<ActivationFunctionFactory> currActivationFunctionPtr(new ActivationFunctionFactory()) ; 
		this->neural_network_layers[this->neural_network_layers.size()-1]->setActivationFunction(currActivationFunctionPtr->createActivationFunction(activationName, slope, shift)) ;
	}

	void NeuralNetwork::setSamplingRate(std::string inSamplingRate)
	{
		this->neural_network_sampling_rate = boost::lexical_cast<double>(inSamplingRate) ;
	}

	void NeuralNetwork::setTrainingAlgorithm(std::string inTrainingAlgorithm)
	{
		if(boost::iequals(inTrainingAlgorithm, "rprop"))
		{
			this->neural_network_trainType = RP ;
		}
		else if(boost::iequals(inTrainingAlgorithm, "backprop"))
		{
			this->neural_network_trainType = BP ;
		}
		else if(boost::iequals(inTrainingAlgorithm, "cdlm"))
		{
			this->neural_network_trainType = CDLM ;
		}
		else if(boost::iequals(inTrainingAlgorithm, "cdlm_sc"))
		{
			this->neural_network_trainType = CDLM_SC;
		}
	}

	void NeuralNetwork::setApproximationType(std::string inApproxType)
	{
		if(boost::iequals(inApproxType, "fp"))
		{
			this->neural_network_approxType = FP ;
		}
		else if(boost::iequals(inApproxType, "lp"))
		{
			this->neural_network_approxType = LP ;
		}
		else if(boost::iequals(inApproxType, "lp_sc"))
		{
			this->neural_network_approxType = LP_SC ;
		}	
	}


	void NeuralNetwork::addLayer (LayerPtr& currLayer)
	{
		this->neural_network_layers.push_back(currLayer) ;
	}


	void NeuralNetwork::addBiasNeurons()
	{
		unsigned int layerIndex = 0 ;

		// Add a bias neuorns and connect it to all the neurons of the next layer
		for(layerIndex = 0 ; layerIndex < this->neural_network_layers.size() - 1 ; layerIndex++)
		{
			int lastNeuronIndex = this->neural_network_layers[layerIndex]->layer_neurons.size();
			NeuronPtr currBiasPtr = this->neural_network_layers[layerIndex]->addNeuron(lastNeuronIndex, true) ;
			currBiasPtr->setLayerPtr(this->neural_network_layers[layerIndex]) ;

			std::vector<NeuronPtr>::iterator nIt ;

			for(nIt = this->neural_network_layers[layerIndex+1]->layer_neurons.begin() ; 
					nIt != this->neural_network_layers[layerIndex+1]->layer_neurons.end() ; nIt++)
			{
				boost::shared_ptr<Synapse> currSyanpse(new Synapse(currBiasPtr, (*nIt), 1.0)) ;
				currBiasPtr->addOutgoingSynapse(currSyanpse) ;
				(*nIt)->addIncomingSynapse(currSyanpse) ;
			}
		}
	}


	void NeuralNetwork::feedForward ()
	{
		// Start from the first layer to the last
		std::vector<LayerPtr>::iterator lIt ;

		for(lIt = this->neural_network_layers.begin() + 1 ; lIt != this->neural_network_layers.end() ; lIt++)
		{
			// It goes over all the neurons in this layer and evaluate them
			(*lIt)->evaluateNeurons() ;
		}
	}


	void NeuralNetwork::backpropagate()
	{

		// Start from the last hidden layer to the input layer
		// and calculate the delta of each synapse
		for (int i = this->neural_network_layers.size() - 2 ; i >= 0 ; i--)
		{
			this->neural_network_layers[i]->backpropagate(this->neural_network_learningRate) ;
		}

	}

	void NeuralNetwork::updateWeights()
	{
		std::vector<LayerPtr>::iterator lIt ;

		// You dont need to go to the output layer
		// output layer doesn't have any outgoing edge
		for(lIt = this->neural_network_layers.begin() ; lIt != this->neural_network_layers.end() - 1 ; lIt++)
		{
			(*lIt)->updateOutgoingWeights() ;
		}
	}


	// iRrop- algorithm
	void NeuralNetwork::rpropUpdateWeights()
	{
		this->neural_network_maxMagWeight = 0 ;
		for (int i = this->neural_network_layers.size() - 2 ; i >= 0 ; i--)
		{
			this->neural_network_layers[i]->rpropUpdateWeights(this->neural_network_maxMagWeight,
																this->neural_network_deltaMin,
																this->neural_network_deltaMax ) ;
		}
	}


	void NeuralNetwork::computeMSE(TrainType tt)
	{
		// Compute MSE for the output layer
		this->neural_network_layers[this->neural_network_layers.size() - 1]->computeMSE(
					this->neural_network_MseFp, this->neural_network_MseLp, this->neural_network_MseLpSc, tt, this->neural_network_mseNum) ;
	}


	double NeuralNetwork::getMSE(ApproxType at)
	{
		double val;

		switch(at){
		case FP:
			val =  (double) this->neural_network_MseFp / this->neural_network_mseNum ;
			break;
		case LP:
			val =  (double) this->neural_network_MseLp / this->neural_network_mseNum;
			break;
		case LP_SC:
			val =  (double) this->neural_network_MseLpSc / this->neural_network_mseNum; 
			break;
		}
		return val;
	}

	void NeuralNetwork::clearMSE()
	{
		this->neural_network_mseNum				= 0 ;
		this->neural_network_MseFp				= 0.0 ;
		this->neural_network_MseLpSc 			= 0.0 ;
		this->neural_network_MseLp 				= 0.0 ;
	}

	void NeuralNetwork::runNeuralNetwork(std::vector<double>& inInputs, std::vector<double>& outOutputs
			, int iBits, int wBits, ApproxType at)
	{

		switch(at)
		{
			case FP:
			{
				// Assign the values all the neurons inside this layer
				this->neural_network_layers[0]->assignNeuronsValue(inInputs) ;

				//this->printNeuralNetwork() ;

				// Run the neural network
				this->feedForward() ;

				//this->printNeuralNetwork() ;

				// Get the computed values of the neurons in the output layer
				this->neural_network_layers[this->neural_network_layers.size() - 1]->getNeuronsValue(outOutputs, at) ;
				//std::string tempLine;
				//std::getline(std::cin, tempLine);
				break;
			}
			case LP:
			{

				this->config(iBits, wBits);

				// Assign the values all the neurons inside this layer
				this->neural_network_layers[0]->assignNeuronsValue(inInputs) ;
		
				
				// Run the neural network
				this->feedForward() ;
		
				// Get the computed values of the neurons in the output layer
				this->neural_network_layers[this->neural_network_layers.size() - 1]->getNeuronsValue(outOutputs, at) ;
				break ;
			}
		}

		

	}




	void NeuralNetwork::randomizeWeights(double min, double max)
	{
		// We do not need to call this function for the input layer as it does not have any incoming synapses
		// this->neural_network_inputLayer->assignRandomValuesIncomingSynapses(min, max) ;

		// Randomize the weight of the incoming synapses for all the hidden layers
		std::vector<LayerPtr>::iterator lIt ;
		for(lIt = this->neural_network_layers.begin() ; lIt != this->neural_network_layers.end() ; lIt++)
		{
			(*lIt)->assignRandomValuesIncomingSynapses(min, max) ;
		}

	}

	void NeuralNetwork::normalizeTest(double min, double max)
	{
		std::cout << "mair" << std::endl ;
		for(int i = 0 ; i < this->neural_network_testInputData.size() ; i ++)
		{
			for(int j = 0 ; j < this->neural_network_testInputData[0].size() ; j ++)
			{

				this->neural_network_testInputData[i][j] = ((this->neural_network_testInputData[i][j] - this->min_test_inputs[j]) 
															/ (double) (this->max_test_inputs[j] - this->min_test_inputs[j])) * 
														(max - min) + min ;
			}

			for(int j = 0 ; j < this->neural_network_testOutputData[0].size() ; j ++)
			{

				this->neural_network_testOutputData[i][j] = ((this->neural_network_testOutputData[i][j] - this->min_test_outputs[j]) 
															/ (double) (this->max_test_outputs[j] - this->min_test_outputs[j])) * 
														(max - min) + min ;
			}
		}
	}


	void NeuralNetwork::normalizeTrain(double min, double max)
	{


		for(int i = 0 ; i < this->neural_network_inputData.size() ; i ++)
		{
			

			for(int j = 0 ; j < this->neural_network_inputData[0].size() ; j ++)
			{

				if(this->max_inputs[j] == this->min_inputs[j])
				{
					this->neural_network_inputData[i][j] = 0.0 ;
				}
				else
				{


					this->neural_network_inputData[i][j] = ((this->neural_network_inputData[i][j] - this->min_inputs[j]) 
															/ (double) (this->max_inputs[j] - this->min_inputs[j])) * 
														(max - min) + min ;
				}
			}

			for(int j = 0 ; j < this->neural_network_outputData[0].size() ; j ++)
			{

				if(this->max_outputs[j] == this->min_outputs[j])
				{
					this->neural_network_outputData[i][j] = 0.0 ;
				}
				else
				{
					this->neural_network_outputData[i][j] = ((this->neural_network_outputData[i][j] - this->min_outputs[j]) 
															/ (double) (this->max_outputs[j] - this->min_outputs[j])) * 
														(max - min) + min ;
				}
			}
		}
		std::cout << "Done!" << std::endl ;

	}


	void NeuralNetwork::fillTrainingData(std::string trainFileName)
	{
		// Open the file for reading
		std::ifstream trainFile; 
		trainFile.open(trainFileName.c_str()) ;

		std::string line ;
		// First line is number of training sets
		getline(trainFile, line) ;
		int numberOfTrainingData = boost::lexical_cast<int>(line) ;
		//std::cout << "# Number of iterations: " << numberOfTrainingData << "\n" ;

		// Second line (separate with space) is number of inputs and outputs
		getline(trainFile, line) ;
		std::cout << "# Number of inputs:  " << line << "\n" ;
		getline(trainFile, line) ;
		std::cout << "# Number of outputs: " << line << "\n" ;



		bool isFirst = true ;
		// one line input data , one line output data
		for(int i = 0 ; i < numberOfTrainingData ; i++)
		{
			// Inputs
			getline(trainFile, line) ;
			std::vector<std::string> currStringInputData;
			std::vector<double> currInputData ;
			std::vector<std::string>::iterator sIt;
			boost::algorithm::split_regex(currStringInputData, line, boost::regex("\\s+")) ;

			for(sIt = currStringInputData.begin() ; sIt != currStringInputData.end() ; sIt++)
			{
				double val = boost::lexical_cast<double>(*sIt) ;
				currInputData.push_back(val);
			}

			this->neural_network_inputData.push_back(currInputData) ;




			// Outputs
			getline(trainFile, line) ;
			std::vector<std::string> currStringOutputData;
			std::vector<double> currOutputData ;
			boost::algorithm::split_regex(currStringOutputData, line, boost::regex("\\s+")) ;

			for(sIt = currStringOutputData.begin() ; sIt != currStringOutputData.end() ; sIt++)
			{
				double val = boost::lexical_cast<double>(*sIt) ;
				currOutputData.push_back(val);
			}

			this->neural_network_outputData.push_back(currOutputData) ;


			if(isFirst)
			{
				isFirst = false;
				std::vector<double>::iterator it;

				for(it = currInputData.begin() ; it != currInputData.end() ; it++)
				{
					this->min_inputs.push_back((*it));
					this->max_inputs.push_back((*it));
					//std::cout << "Insert In: " << (*it) << std::endl ;
				}

				for(it = currOutputData.begin() ; it != currOutputData.end() ; it++)
				{
					this->min_outputs.push_back((*it));
					this->max_outputs.push_back((*it));
					//std::cout << "Insert Out: " << (*it) << std::endl ;
				}
			}
			else
			{
				for(int i = 0 ; i < min_inputs.size() ; i++)
				{
					if(currInputData[i] < this->min_inputs[i])
						this->min_inputs[i] = currInputData[i];
					if(currInputData[i] > this->max_inputs[i])
						this->max_inputs[i] = currInputData[i];

					//std::cout << "Curr Input: " << currInputData[i] << std::endl ;
					//std::cout << "Min Input: " << this->min_inputs[i] << std::endl ;
					//std::cout << "Max Input: " << this->max_inputs[i] << std::endl ;


				}

				for(int i = 0 ; i < min_outputs.size() ; i++)
				{
					if(currOutputData[i] < this->min_outputs[i])
						this->min_outputs[i] = currOutputData[i];
					if(currOutputData[i] > this->max_outputs[i])
						this->max_outputs[i] = currOutputData[i];
					//std::cout << "Curr Output: " << currOutputData[i] << std::endl ;
					//std::cout << "Min Output: " << this->min_outputs[i] << std::endl ;
					//std::cout << "Max Output: " << this->max_outputs[i] << std::endl ;
				}

				//std::string tempLine;
				//std::getline(std::cin, tempLine);
			}
		}


		// for(int i = 0 ; i < min_inputs.size() ; i++)
		// {
		// 	std::cout << "Min Input: " << this->min_inputs[i] << std::endl ;
		// 	std::cout << "Max Input: " << this->max_inputs[i] << std::endl ;
		// }
		// for(int i = 0 ; i < min_outputs.size() ; i++)
		// {
		// 	std::cout << "Min outputs: " << this->min_outputs[i] << std::endl ;
		// 	std::cout << "Max outputs: " << this->max_outputs[i] << std::endl ;
		// }
		// std::string tempLine;
		// std::getline(std::cin, tempLine);

		trainFile.close();
	}


	void NeuralNetwork::fillTestingData(std::string testFileName)
	{
		// Open the file for reading
		std::ifstream testFile; 
		testFile.open(testFileName.c_str()) ;

		std::string line ;
		// First line is number of training sets
		getline(testFile, line) ;
		int numberOfTestingData = boost::lexical_cast<int>(line) ;
		//std::cout << "# Number of iterations: " << numberOfTrainingData << "\n" ;

		// Second line (separate with space) is number of inputs and outputs
		getline(testFile, line) ;
		std::cout << "# Number of inputs:  " << line << "\n" ;
		getline(testFile, line) ;
		std::cout << "# Number of outputs: " << line << "\n" ;


		bool isFirst = true ;

		// one line input data , one line output data
		for(int i = 0 ; i < numberOfTestingData ; i++)
		{
			// Inputs
			getline(testFile, line) ;
			std::vector<std::string> currStringInputData;
			std::vector<double> currInputData ;
			std::vector<std::string>::iterator sIt;
			boost::algorithm::split_regex(currStringInputData, line, boost::regex("\\s+")) ;

			for(sIt = currStringInputData.begin() ; sIt != currStringInputData.end() ; sIt++)
			{
				double val = boost::lexical_cast<double>(*sIt) ;
				currInputData.push_back(val);
			}

			this->neural_network_testInputData.push_back(currInputData) ;

			// Outputs
			getline(testFile, line) ;
			std::vector<std::string> currStringOutputData;
			std::vector<double> currOutputData ;
			boost::algorithm::split_regex(currStringOutputData, line, boost::regex("\\s+")) ;

			for(sIt = currStringOutputData.begin() ; sIt != currStringOutputData.end() ; sIt++)
			{
				double val = boost::lexical_cast<double>(*sIt) ;
				currOutputData.push_back(val);
			}

			this->neural_network_testOutputData.push_back(currOutputData) ;

			if(isFirst)
			{
				isFirst = false;
				std::vector<double>::iterator it;

				for(it = currInputData.begin() ; it != currInputData.end() ; it++)
				{
					this->min_test_inputs.push_back((*it));
					this->max_test_inputs.push_back((*it));
				}

				for(it = currOutputData.begin() ; it != currOutputData.end() ; it++)
				{
					this->min_test_outputs.push_back((*it));
					this->max_test_outputs.push_back((*it));
				}
			}
			else
			{
				for(int i = 0 ; i < min_test_inputs.size() ; i++)
				{
					if(currInputData[i] < this->min_test_inputs[i])
						this->min_test_inputs[i] = currInputData[i];
					if(currInputData[i] > this->max_test_inputs[i])
						this->max_test_inputs[i] = currInputData[i];
				}

				for(int i = 0 ; i < min_test_outputs.size() ; i++)
				{
					if(currOutputData[i] < this->min_test_outputs[i])
						this->min_test_outputs[i] = currOutputData[i];
					if(currOutputData[i] > this->max_test_outputs[i])
						this->max_test_outputs[i] = currOutputData[i];
				}
			}
		}

		testFile.close();
	}


	// Shuffling is done only for BACKPROPR INCREMENTAL
	// It doesn't need to be done for the RPROP as it is
	// a batch algorithm
	// We use the modern algorithm introduced by Richard Durstenfeld
	void NeuralNetwork::shuffleTrainingSet()
	{
		// Initialize the seed
		srand(time(NULL)) ;

		for(int i = this->neural_network_inputData.size() - 1 ; i > 0 ; i--)
		{
			// generate a random number between 0 <= j <= i
			int j =  ( rand() % (i + 1) ) ;
			std::swap(this->neural_network_inputData[i], this->neural_network_inputData[j]) ;
			std::swap(this->neural_network_outputData[i], this->neural_network_outputData[j]) ;
		}

	}


	void NeuralNetwork::initializeNeuralNetwork()
	{
		std::vector<LayerPtr>::iterator lIt ;
		for(lIt = this->neural_network_layers.begin() ; lIt != this->neural_network_layers.end() ; lIt++)
		{
				(*lIt)->initializeLayer() ;
		}

	}

	double NeuralNetwork::testNN()
	{

		for(int i = 0 ; i < this->neural_network_inputData.size() ; i++)
		{
			this->neural_network_layers[0]->assignNeuronsValue(this->neural_network_inputData[i]) ;
			this->neural_network_layers[this->neural_network_layers.size() - 1]->assignNeuronsTargetValue(this->neural_network_outputData[i]) ;

			this->feedForward() ;

			this->computeMSE(CDLM) ;
		}

		return this->getMSE(LP) ;
	}

	void NeuralNetwork::trainNeuralNetwork(std::string logFilename, int max_epoch_print)
	{

		TrainType tt = this->neural_network_trainType ;

		// Create the log file
		std::ofstream logFile ;
		logFile.open(logFilename) ;
		logFile.precision(22) ;


		int outputNeuronSize = this->neural_network_layers[this->neural_network_layers.size() - 1]->layer_neurons.size() ;
		int trainingSize     = this->neural_network_inputData.size() ;
		unsigned int maximum_sample = (int)(this->neural_network_inputData.size() * this->neural_network_sampling_rate) ;
		//std::cout << "# Total trainig set size: 	" << trainingSize << std::endl ;
		//std::cout << "# Sampled training set size: 	" << maximum_sample << std::endl ;
 		//std::cout << "# Output neurons size: 		" << outputNeuronSize << std::endl ;
		std::cout << "--------------------------------------------" << std::endl ;
		

		double mseLpSave = 100000.0 ;

		for(int i = 0 ; i < this->neural_network_maxEpoch ; i++)
		{

			// Clear MSE
			this->clearMSE() ;

			#ifdef NN_DEBUG
			{
				std::cout << "Clearing MSE..." << std::endl;
				std::cout << "Maximum Sample: "  << maximum_sample << std::endl;
			}
			#endif

			for(unsigned int j = 0 ; j < maximum_sample ; j++)
			{

				// Update the Neuron(_SC) value to the new value
				this->neural_network_layers[0]->assignNeuronsValue(this->neural_network_inputData[j]) ;
				// Assign the desired value to the output layer (target value)
				this->neural_network_layers[this->neural_network_layers.size() - 1]->assignNeuronsTargetValue(this->neural_network_outputData[j]) ;

				//if(this->neural_network_trainType == CDLM)
				//	this->printNeuralNetwork() ;

				//#ifdef NN_DEBUG 
					//std::cout << "Assigning the values..." << std::endl;
				//#endif

				// Run the network
				this->feedForward() ;

				//if(this->neural_network_trainType == CDLM)
				//	this->printNeuralNetwork() ;


				//if(this->neural_network_trainType == CDLM)
				//{
				//	std::string tempLine ;
				//	std::getline(std::cin, tempLine);
				//}

				//if(NN_DEBUG) std::cout << "feedForward is done..." << std::endl;


				// Compute the MSE and assign the diff to the output neurons
				this->computeMSE(tt) ;
				//if(NN_DEBUG) std::cout << "ComputeMSE is done..." << std::endl;


				// Compute the delta for the hidden layer
				this->backpropagate() ;
				//if(NN_DEBUG) std::cout << "BP is done..." << std::endl;

				// Backpropagate the error if the training algorithm is back propagation
				if(this->neural_network_trainType == BP)
				{
					this->updateWeights() ;
				}
			}

			// iRprop- rprop is a batch algorithm
			// it means after all the training sample we try to update weights
			// CDLM is running based on the RP training type
			if (this->neural_network_trainType != BP)
			{
				this->rpropUpdateWeights() ;
				#ifdef NN_DEBUG
				{
					std::cout << "IO:     "  << this->neural_network_nIOBits << std::endl;
					std::cout << "Weight: "  << this->neural_network_nWBits << std::endl ;
				}
				#endif
				this->config(this->neural_network_nIOBits, this->neural_network_nWBits) ;
			}

			if ( (((i % 1) == 0) || (i == (this->neural_network_maxEpoch - 1)))  && (tt == CDLM) && (i > 4) ) { // for cdlm training
				if (this->neural_network_MseLp <= mseLpSave) { // save good weights occasionally
					mseLpSave = this->neural_network_MseLp ;
					//std::cout << "save weights. mseLpSave = " << mseLpSave / this->neural_network_mseNum << "\n";
					this->saveWeights();
					//cout << "save weights. mseLpSave = " << mseLpSave / mseNum << "\n";
				} 
				if ( i == (this->neural_network_maxEpoch - 1) ) { // restore best weights at the end
					std::cout << "restoring saved weights with MSE " << mseLpSave / this->neural_network_mseNum << "\n";
					this->neural_network_MseLp = mseLpSave;
					this->loadWeights();
					this->config(this->neural_network_nIOBits, this->neural_network_nWBits);
				}

			}


			if((i % max_epoch_print) == 0 || i == (this->neural_network_maxEpoch - 1))
			{
				std::cout << i << "\t\t" << "MSEFp: 		" << std::fixed << this->getMSE(FP) << std::endl ;
				std::cout << i << "\t\t" << "MSELp: 		" << std::fixed << this->getMSE(LP) << std::endl ;
				std::cout << i << "\t\t" << "MSELpSc:		" << std::fixed << this->getMSE(LP_SC) << std::endl ;

				logFile << i << "\t\t" << "MSEFp: 	" << std::fixed << this->getMSE(FP) << "\n" ;
				logFile << i << "\t\t" << "MSELp: 	" << std::fixed << this->getMSE(LP) << "\n" ;
				logFile << i << "\t\t" << "MSELpSc:	" << std::fixed << this->getMSE(LP_SC) << "\n" ;
			}
		}

		logFile.close() ;
	}



	void NeuralNetwork::scaleInputs(double minVal, double maxVal, double scMinVal, double scMaxVal)
	{
		for(unsigned i = 0 ; i < this->neural_network_inputData.size() ; i++)
		{
			for(unsigned j = 0 ; j < this->neural_network_inputData[i].size() ; j++)
			{
				this->neural_network_inputData[i][j] = this->scale(this->neural_network_inputData[i][j], minVal, maxVal, scMinVal, scMaxVal) ;
			}
		}

		for(unsigned i = 0 ; i < this->neural_network_testInputData.size() ; i++)
		{
			for(unsigned j = 0 ; j < this->neural_network_testInputData[i].size() ; j++)
			{
				this->neural_network_testInputData[i][j] = this->scale(this->neural_network_testInputData[i][j], minVal, maxVal, scMinVal, scMaxVal) ;
			}
		}
	}

	void NeuralNetwork::scaleOutputs(double minVal, double maxVal, double scMinVal, double scMaxVal)
	{
		for(unsigned int i = 0 ; i < this->neural_network_outputData.size() ; i++)
		{
			for(unsigned j = 0 ; j < this->neural_network_outputData[i].size() ; j++)
			{
				this->neural_network_outputData[i][j] = this->scale(this->neural_network_outputData[i][j], minVal, maxVal, scMinVal, scMaxVal) ;
			}
		}

		for(unsigned int i = 0 ; i < this->neural_network_testOutputData.size() ; i++)
		{
			for(unsigned j = 0 ; j < this->neural_network_testOutputData[i].size() ; j++)
			{
				this->neural_network_testOutputData[i][j] = this->scale(this->neural_network_testOutputData[i][j], minVal, maxVal, scMinVal, scMaxVal) ;
			}
		}

	}

	double NeuralNetwork::scale(double oldVal, double minVal, double maxVal, double scMinVal, double scMaxVal)
	{
		double newVal ;
		double oldRange = (maxVal - minVal) ;
		double newRange = (scMaxVal - scMinVal) ;

		double temp = ((oldVal - minVal) / oldRange) * newRange + scMinVal ;

		if(temp < scMinVal)
		{
			newVal = scMinVal ;
		}
		else if(temp > scMaxVal)
		{
			newVal = scMaxVal ;
		}
		else
		{
			newVal = temp ;
		}
		return newVal ;
	}


	void NeuralNetwork::config(int nIBitsStart, int nWBitsStart)
	{
		this->neural_network_nIOBits = nIBitsStart ;
		this->neural_network_nWBits  = nWBitsStart ;


		int nIntWBits ;
		int nFracWBits ;


		// split nWBits into nIntWBits, nFracWBits for LP version using maxMagWeight
		if( this->neural_network_maxMagWeight == 0)
		{
			nIntWBits = 1 ;
			nFracWBits = this->neural_network_nWBits ;
		}
		else
		{
			nIntWBits = int(log2(this->neural_network_maxMagWeight)) + 1 ;
			//std::cout << "Int: " << nIntWBits << std::endl ;
			//std::cout << "MaxMag: " <<  this->neural_network_maxMagWeight << std::endl ;
			//std::cout << "W bit: " << this->neural_network_nWBits << std::endl ;
			std::string tempInput ;
			//std::getline(std::cin, tempInput);


			if(nIntWBits >= this->neural_network_nWBits)
			{
				nIntWBits = this->neural_network_nWBits ;
				nFracWBits = 0 ;
			}
			else
			{
				nFracWBits = this->neural_network_nWBits - nIntWBits ;
			}
		}


		// Configure all neurons array
		std::vector<LayerPtr>::iterator lIt ;
		for(lIt = this->neural_network_layers.begin() ; lIt != this->neural_network_layers.end() ; lIt++)
		{
			(*lIt)->config(this->neural_network_nIOBits, nIntWBits, nFracWBits, LP) ;
			(*lIt)->config(this->neural_network_nIOBits, 1, this->neural_network_nWBits, LP_SC) ;
		}


		// Scale incoming set of weights
		// this->scaleWeights() ;

	}

	void NeuralNetwork::scaleWeights()
	{
		
		// For all the hidden and output layers
		for(unsigned int lIndex = 1 ; lIndex < this->neural_network_layers.size() ; lIndex++)
		{
			this->neural_network_layers[lIndex]->scaleWeights() ;
		}
	}


	void NeuralNetwork::writeXML ( std::string xmlFileName, ApproxType at)
	{

		std::ofstream xmlFile ;
		xmlFile.open(xmlFileName.c_str()) ;
		xmlFile.precision(22) ;

		std::vector<LayerPtr>::iterator lIt;
		for(lIt = this->neural_network_layers.begin() ; lIt != this->neural_network_layers.end() - 1 ; lIt++)
		{
			// All the neurons
			xmlFile << (*lIt)->layer_neurons.size() << " " ;
		}	
		xmlFile << (*lIt)->layer_neurons.size() << "\n" ;

		// Start writing the connections ...
		for(lIt = this->neural_network_layers.begin() ; lIt != this->neural_network_layers.end() - 1 ; lIt++)
		{
			(*lIt)->writeOutgoingSynapses(xmlFile, at) ;
		}


		// Start writing the configurations
		xmlFile << "learning_rate=" << std::fixed << this->neural_network_learningRate <<  "\n" ;
		xmlFile << "max_epoch=" << std::fixed << this->neural_network_maxEpoch << "\n" ;
		xmlFile << "desired_MSE=" << std::fixed << this->neural_network_desiredMSE << "\n" ;
		xmlFile << "rprop_init_update=" << std::fixed << this->neural_network_rpropInitialStep << "\n" ;
		xmlFile << "rprop_delta_min=" << std::fixed << this->neural_network_deltaMin << "\n" ;
		xmlFile << "rprop_delta_max=" << std::fixed << this->neural_network_deltaMax << "\n" ;
		xmlFile << "log=" << this->printLog() << "\n" ;
		xmlFile << "activaiton_function_input=" << this->printInputActivationFunction() << "\n" ;
		xmlFile << "activation_function_output=" << this->printOutputActivationFunction()  << "\n" ;
		xmlFile << "activation_function_hiddens=" << this->printHiddensActivationFunction();

		xmlFile.close();
	}


	void NeuralNetwork::printNeuralNetwork()
	{

		std::cout << "Input Layer:   \n" ;
		this->neural_network_layers[0]->printNeurons() ;
		std::cout << "-------------------------------------------------------\n" ;

		std::vector<LayerPtr>::iterator lIt ;
		for(lIt = this->neural_network_layers.begin() + 1; lIt != this->neural_network_layers.end() - 1 ; lIt++)
		{
			std::cout << "Hidden Layers: \n" ;
			(*lIt)->printNeurons() ;
			std::cout << "-------------------------------------------------------\n" ;
		}
		
		std::cout << "Output Layer:  \n" ;
		this->neural_network_layers[this->neural_network_layers.size() - 1]->printNeurons() ;
	}

	std::string NeuralNetwork::printLog()
	{
		if(this->neural_network_isLog)
			return "1" ;
		else
			return "0" ;
	}

	std::string NeuralNetwork::printInputActivationFunction()
	{
		return this->neural_network_layers[0]->getActivationFunction() ;
	}
	std::string NeuralNetwork::printHiddensActivationFunction()
	{
		return this->neural_network_layers[1]->getActivationFunction() ;
	}
	std::string NeuralNetwork::printOutputActivationFunction()
	{
		return this->neural_network_layers[this->neural_network_layers.size() - 1]->getActivationFunction() ;
	}


	double 	NeuralNetwork::test ()
	{
		this->clearMSE() ;

		for(unsigned int j = 0 ; j < this->neural_network_testInputData.size() ; j++)
		{
			// Update the Neuron(_SC) value to the new value
			this->neural_network_layers[0]->assignNeuronsValue(this->neural_network_testInputData[j]) ;
			// Assign the desired value to the output layer (target value)
			this->neural_network_layers[this->neural_network_layers.size() - 1]->assignNeuronsTargetValue(this->neural_network_testOutputData[j]) ;
		
			this->feedForward() ;

			this->computeMSE(BP) ;
		}

		return (double) this->neural_network_MseFp / this->neural_network_mseNum ;
	}

	void NeuralNetwork::loadWeights()
	{
		std::vector<LayerPtr>::iterator lIt ;
		for(lIt = this->neural_network_layers.begin(); lIt != this->neural_network_layers.end() - 1; lIt++)
		{
			(*lIt)->loadWeights() ;
		}
	}

	void NeuralNetwork::saveWeights()
	{
		std::vector<LayerPtr>::iterator lIt ;
		for(lIt = this->neural_network_layers.begin(); lIt != this->neural_network_layers.end() - 1; lIt++)
		{
			(*lIt)->saveWeights() ;
		}
	}
	
}