
#include "xml_parser.hpp"



namespace anpu {

	XMLParser::XMLParser(std::string xmlName, NeuralNetworkPtr& rootPtr, bool isBias)
	{
		std::cout << "# Start processing the XML " << xmlName << std::endl;

		// First Process the neurons inside each layer (input hidden 1 hidden 2 ... hidden i output)
		rootPtr = NeuralNetworkPtr(new NeuralNetwork()) ;


		std::ifstream xmlFile ;
		xmlFile.open(xmlName.c_str()) ;


		std::string line ;

		// Read first line
		getline(xmlFile, line);
		//std::cout << "Amie: " << line << std::endl;
		std::vector<std::string> tokenizeVector ;
		boost::algorithm::split_regex(tokenizeVector, line, boost::regex("\\s")) ;
		//std::cout << "# First line is done... " << line << std::endl;

		// Create the layers and the number of neurons inside each layer
		for(unsigned int j = 0 ; j < tokenizeVector.size() ; j++)
		{
			int currLayerNeurons = boost::lexical_cast<int>(tokenizeVector[j]) ;

			// Create the layers
			LayerPtr currLayerPtr(new Layer(j)) ;

			// Add number of neurons to this layer
			for(int i = 0; i < currLayerNeurons - 1; i++)
			{
				NeuronPtr currNeuronPtr = currLayerPtr->addNeuron(i, false) ;
				currNeuronPtr->setLayerPtr(currLayerPtr) ;

			}

			// Output doesn't have any bias neuron
			if(!isBias && j != tokenizeVector.size() - 1 )
			{
				NeuronPtr currNeuronPtr = currLayerPtr->addNeuron(currLayerNeurons - 1, true) ;
				currNeuronPtr->setLayerPtr(currLayerPtr) ;
			}
			else
			{
				NeuronPtr currNeuronPtr = currLayerPtr->addNeuron(currLayerNeurons - 1, false) ;
				currNeuronPtr->setLayerPtr(currLayerPtr) ;	
			}

			rootPtr->addLayer(currLayerPtr) ;
		}


		// read lines until you see a =
		double maxWeight = 0.0;
		while(true)
		{
			getline(xmlFile, line) ;
			if(line.find("=") != std::string::npos)
				break ;

			// Start creating the connections
			tokenizeVector.clear();
			boost::algorithm::split_regex(tokenizeVector, line, boost::regex("\\s")) ;

			int srcLayerIndex 	= boost::lexical_cast<int>(tokenizeVector[0]) ;
			int srcNeuronIndex 	= boost::lexical_cast<int>(tokenizeVector[1]) ;

			int dstLayerIndex	= boost::lexical_cast<int>(tokenizeVector[2]) ;
			int dstNeuronIndex 	= boost::lexical_cast<int>(tokenizeVector[3]) ;

			double weight		= boost::lexical_cast<double>(tokenizeVector[4]) ;
			//std::cout << "Line: " << line << std::endl;

			if(fabs(weight) > maxWeight)
			{
				maxWeight = fabs(weight) ;
			}

			NeuronPtr srcNeuronPtr = rootPtr->neural_network_layers[srcLayerIndex]->layer_neurons[srcNeuronIndex] ;
			NeuronPtr dstNeuronPtr = rootPtr->neural_network_layers[dstLayerIndex]->layer_neurons[dstNeuronIndex] ;

			// Create a syanpse
			SynapsePtr currSynapsePtr(new Synapse(srcNeuronPtr, dstNeuronPtr, weight)) ;

			srcNeuronPtr->addOutgoingSynapse(currSynapsePtr) ;
			dstNeuronPtr->addIncomingSynapse(currSynapsePtr) ;
		}

		if(isBias) rootPtr->addBiasNeurons() ;

		rootPtr->neural_network_maxMagWeight = maxWeight ;

		std::cout << "# Max weight: " << rootPtr->neural_network_maxMagWeight << std::endl;

		//rootPtr->printNeuralNetwork() ;

		// Read configurations
		while(true)
		{

			std::vector<std::string> tokenizeVector ;
			boost::algorithm::split_regex(tokenizeVector, line, boost::regex("=")) ;

			// Learning Rate
			if(boost::iequals(tokenizeVector[0], "learning_rate"))
			{
				rootPtr->setLearningRate(boost::lexical_cast<double>(tokenizeVector[1])) ;
			}
			else if (boost::iequals(tokenizeVector[0], "max_epoch"))
			{
				rootPtr->setMaxEpoch(boost::lexical_cast<int>(tokenizeVector[1])) ;	
			}
			else if (boost::iequals(tokenizeVector[0], "desired_MSE"))
			{
				rootPtr->setDesiredMSE(boost::lexical_cast<double>(tokenizeVector[1])) ;	
			}
			else if (boost::iequals(tokenizeVector[0], "rprop_init_update"))
			{
				rootPtr->setRpropInitialStep(boost::lexical_cast<double>(tokenizeVector[1])) ;
			}
			else if (boost::iequals(tokenizeVector[0], "rprop_delta_min"))
			{
				rootPtr->setDeltaMin(boost::lexical_cast<double>(tokenizeVector[1])) ;
			}
			else if (boost::iequals(tokenizeVector[0], "rprop_delta_max"))
			{
				rootPtr->setDeltaMax(boost::lexical_cast<double>(tokenizeVector[1])) ;
			}
			else if (boost::iequals(tokenizeVector[0], "log"))
			{
				int isLog = boost::lexical_cast<double>(tokenizeVector[1]) ;
				if(isLog == 0)
				{
					rootPtr->isLogging(false);
				}
				else
				{
					rootPtr->isLogging(true);
				}
			}
			else if (boost::iequals(tokenizeVector[0], "activation_function_input"))
			{
				rootPtr->setInputActivationFunction(tokenizeVector[1]);
			}
			else if (boost::iequals(tokenizeVector[0], "activation_function_hiddens"))
			{
				rootPtr->setHiddenActivationFunction(tokenizeVector[1]);
			}
			else if (boost::iequals(tokenizeVector[0], "activation_function_output"))
			{
				rootPtr->setOutputActivationFunction(tokenizeVector[1]);
			}
			else if(boost::iequals(tokenizeVector[0], "training_algorithm"))
			{
				rootPtr->setTrainingAlgorithm(tokenizeVector[1]) ;
			}
			else if (boost::iequals(tokenizeVector[0], "approximation_type"))
			{
				rootPtr->setApproximationType(tokenizeVector[1]) ;
			}
			else if (boost::iequals(tokenizeVector[0], "sampling_rate"))
			{
				rootPtr->setSamplingRate(tokenizeVector[1]) ;
			}
			if(!xmlFile.good())
				break ;
			getline(xmlFile, line) ;
		}	
	}	
}