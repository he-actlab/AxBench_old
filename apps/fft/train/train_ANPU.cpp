#include "neuralnetwork.hpp"
#include "xml_parser.hpp"
#include "datatype.hpp"
#include "fann.h"
#include <boost/algorithm/string/regex.hpp>


#define DEBUG 0

typedef boost::shared_ptr<anpu::NeuralNetwork> NeuralNetworkPtr ;


int main(int argc, char *argv[])
{

	std::string benchName						= argv[1] ;


	int  max_epochs        			  			= 8000 ;
	const unsigned int epochs_between_reports 	= 1000 ;
	const unsigned int desired_error            = 0.0000001 ;


	std::ofstream logFile;
	std::string logFileName = benchName + std::string("_results.log") ;
	logFile.open(logFileName) ;
	logFile.precision(22) ;

	// Will be used with python to generate curves
	std::ofstream logFile_data;
	std::string logFileName_data = benchName + std::string(".csv") ;
	logFile_data.open(logFileName_data) ;
	logFile_data.precision(22) ;

	std::string trainFileName = std::string("../train/") + benchName + std::string("_train.data") ;
	std::string testFileName = std::string("../train/") + benchName + std::string("_test.data") ;



	// Baseline Full Precision no topology restriction
	//-----------------------------------------------------------------------
	std::cout << "\033[1;31m ************** START FULL PRECISION WITHOUT ANY TOPOLOGY RESTRICTION ******************* \033[0m\n";
	NeuralNetworkPtr npuBaseLineFP;
	std::string digitalXML = benchName + std::string("_NPU.xml") ;
	std::cout << "# Read XML: " << digitalXML << std::endl ; 
	anpu::XMLParser npuBaseLineFpXMLParser(digitalXML, npuBaseLineFP, true) ;

	// Fill training and testing input/output data
	npuBaseLineFP->fillTrainingData(trainFileName) ;


	std::cout << "# Filled the training data..." << std::endl ;

	// Configuration
	npuBaseLineFP->randomizeWeights(-0.1,0.1) ;
	npuBaseLineFP->setDesiredMSE(desired_error) ;
	npuBaseLineFP->setTrainingAlgorithm("rprop") ;


	npuBaseLineFP->setApproximationType("FP") ;
	npuBaseLineFP->setMaxEpoch(max_epochs) ;

	// Activation Function Setup
	npuBaseLineFP->setInputActivationFunction("linear") ;
	npuBaseLineFP->setHiddenActivationFunction("sigmoidsymmetric") ;
	npuBaseLineFP->setOutputActivationFunction("sigmoidsymmetric") ;


	// Training
	std::string fpLog 	= std::string("./log/") + benchName  + std::string("_NPU_Baseline_Fp.log") ;


	std::cout << "# Start training of the network..." << std::endl ;
	npuBaseLineFP->trainNeuralNetwork(fpLog, epochs_between_reports) ;
	std::cout << "# Trained of the network..." << std::endl ;

	logFile << npuBaseLineFP->getMSE(FP) << "\t" ;
	std::string baseOut = std::string("./xml/") + benchName + std::string("_NPU_Baseline_Fp.xml") ;		
	npuBaseLineFP->writeXML(baseOut, FP) ;

	std::cout << "\033[1;31m ************************************************************************************ \033[0m\n";
	//-----------------------------------------------------------------------

	//CDLM training
	int nIBitsStart = 4 ;
	int nIBitsStop  = 16 ;
	int nWBitsStart = 4 ;
	int nWBitsStop  = 16 ; 
	std::vector<std::vector<double> > mseTrainCdlm ; // Store the MSE of the training step
	//-----------------------------------------------------------------------
	for(int i = nWBitsStart ; i <= nWBitsStop; i++)
	{

		std::vector<double> tempTrainCdlm ;
		std::vector<double> tempTestCdlm ;

		for(int j = nIBitsStart ; j <= nIBitsStop ; j++)
		{
			// Load the Previously stored neural network
			NeuralNetworkPtr npuCdlm ;
			anpu::XMLParser  npuCdlmXMLParser(baseOut, npuCdlm, false) ;

			// Fill training and testing input/output data
			npuCdlm->fillTrainingData(trainFileName) ;
			//npuCdlm->fillTestingData(testFileName) ;
			//npuBaseLineFP->normalizeTrain(0.0, 0.8); //  normalize the training input

			//std::string tempInput ;
			//std::getline(std::cin, tempInput);

			// Activation Function Setup
			npuCdlm->setInputActivationFunction("linear") ;
			npuCdlm->setHiddenActivationFunction("sigmoidsymmetric") ;
			npuCdlm->setOutputActivationFunction("sigmoidsymmetric") ;
	
			// Configuration
			npuCdlm->setDesiredMSE(desired_error) ;
			npuCdlm->setTrainingAlgorithm("cdlm") ;
	
			// Step1: Discretize the weights
			npuCdlm->config(j, i) ; // IO, Weight

			std::cout<< "Weight bit: " <<  i << std::endl ;
			std::cout << "IO bit:    " << j << std::endl ; 


			// Get the bit-chopping CDLM
			double chopping_error = npuCdlm->testNN() ;

			std::cout << "# Insert the LP MSE:  " << chopping_error << std::endl ;
			tempTrainCdlm.push_back(chopping_error) ;

			npuCdlm.reset() ;
			std::cout << "----------------------------------------------------\n" ;
		}

		mseTrainCdlm.push_back(tempTrainCdlm) ;
		//mseTestCdlm.push_back(tempTestCdlm) ;
	}



	for(int i = 4 ; i <= 16; i++)
	{
		logFile_data << i << " " ;
	}
	logFile_data << "\n";

	for(int i = 0 ; i < 13; i++)
	{
		for(int j = 0 ; j < 13 ; j++)
		{
			logFile_data << mseTrainCdlm[i][j];
			logFile <<      mseTrainCdlm[i][j];
			if (j != 12)
			{
				logFile_data << " " ;
				logFile << " " ;
			}
		}
		logFile_data << std::endl ;
		logFile << std::endl ;
	}

	logFile_data.close() ;
	logFile.close() ;
}