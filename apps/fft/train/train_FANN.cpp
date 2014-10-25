#include "neuralnetwork.hpp"
#include "xml_parser.hpp"
#include "datatype.hpp"
#include "fann.h"
#include <boost/algorithm/string/regex.hpp>


typedef boost::shared_ptr<anpu::NeuralNetwork> NeuralNetworkPtr ;


int main(int argc, char *argv[])
{

	std::string benchName								= argv[1] ;

	unsigned int 			max_epochs      			= atoi(argv[2]) ;
	double 		 			desired_error				= 0.000001 ;
	const unsigned int 		epochs_between_reports		= 1000 ;

	std::ofstream logFile;
	std::string logFileName = std::string("log/") + benchName + std::string("_results.log") ;
	logFile.open(logFileName) ;
	logFile.precision(22) ;


	std::string trainFileName 	= std::string("../data/") + benchName + std::string("_train.data") ;
	std::string testFileName 	= std::string("../data/") + benchName + std::string("_test.data") ;


	//********************** FANN Training ********************
	//---------------------------------------------------------
		// Topology of the Neural Network
		const unsigned int num_layers	=	4	;

		// Number of Neurons per Layer
		const unsigned int num_input				=  1 ;
		const unsigned int num_neurons_hidden_1		=  4 ;
		const unsigned int num_neurons_hidden_2		=  4 ;
		const unsigned int num_output				=  2 ;
		//------------------------------------------------	

		// Create the Neural Network
		struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden_1, num_neurons_hidden_2,  num_output) ;
	
		// Set the activation function
		fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
		fann_set_activation_function_output(ann, FANN_LINEAR) ;
	
		// Training
		fann_train_on_file(ann, trainFileName.c_str(), max_epochs, epochs_between_reports, desired_error) ;
		std::string fannSave = std::string("../cfg/") + benchName + std::string("_FANN.nn") ;
		fann_save(ann, fannSave.c_str()) ;
		logFile << fann_get_MSE(ann)  << "\t" ;

		// Testing
		struct fann_train_data *data = fann_read_train_from_file(testFileName.c_str()) ;
		fann_reset_MSE(ann) ;
		fann_test_data(ann, data) ;
		logFile << fann_get_MSE(ann)  << std::endl ;
		fann_destroy(ann) ;
}