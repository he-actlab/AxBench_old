#ifndef __ANPU_XML_PARSER_HPP__
#define __ANPU_XML_PARSER_HPP__



#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>


#include "activationfunction.hpp"
#include "neuron.hpp"
#include "synapse.hpp"
#include "layer.hpp"
#include "neuralnetwork.hpp"
#include "datatype.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "boost/lexical_cast.hpp"
#include "xml_parser.hpp"


namespace anpu {

	typedef boost::shared_ptr<NeuralNetwork>  NeuralNetworkPtr ;

	class XMLParser {
		public:
			XMLParser(std::string xmlName, NeuralNetworkPtr& rootPtr, bool isBias) ;

	};

}

#endif