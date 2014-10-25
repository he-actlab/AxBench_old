#include "neuralnetwork.hpp"
#include "xml_parser.hpp"
#include "datatype.hpp"
#include "fann.h"

typedef boost::shared_ptr<anpu::NeuralNetwork> NeuralNetworkPtr ;


int main(int argc, char *argv[])
{

	std::string benchName			= argv[1] ;

	unsigned int max_epochs      	= atoi(argv[2]) ;
	unsigned int max_cdlm_epochs	= 500 ;
	double 		 desired_error		= 0.000001 ;
	const unsigned int epochs_between_reports	=  1000 ;

	std::ofstream logFile;
	std::string logFileName = benchName + std::string("_results.log") ;
	logFile.open(logFileName) ;
	logFile.precision(22) ;

	std::string trainFileName = std::string("../train/") + benchName + std::string("_train.data") ;
	std::string testFileName = std::string("../train/") + benchName + std::string("_test.data") ;


	// FANN Training
	//---------------------------------------------------------
		// Topology of the Neural Network
		const unsigned int num_layers	=	3	;

		// Number of Neurons per Layer
		const unsigned int num_input				=  9 ;
		const unsigned int num_neurons_hidden		=  8 ;
		const unsigned int num_output				=  1 ;
		//------------------------------------------------	


		// Create the Neural Network
		struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output) ;
	
		// Set the activation function
		fann_set_activation_function_hidden(ann, FANN_SIGMOID);
		fann_set_activation_function_output(ann, FANN_SIGMOID) ;
	
		// Training
		fann_train_on_file(ann, trainFileName.c_str(), max_epochs, epochs_between_reports, desired_error) ;
		std::string fannSave = benchName + std::string("_FANN.nn") ;
		fann_save(ann, fannSave.c_str()) ;
		logFile << fann_get_MSE(ann)  << "\t" ;

		// Testing
		struct fann_train_data *data = fann_read_train_from_file(testFileName.c_str()) ;
		fann_reset_MSE(ann) ;
		fann_test_data(ann, data) ;
		logFile << fann_get_MSE(ann)  << "\t" ;
		fann_destroy(ann) ;
	//---------------------------------------------------------

}