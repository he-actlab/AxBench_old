#ifndef __ANPU_DATATYPE_HPP__
#define __ANPU_DATATYPE_HPP__

typedef float NN_DATA ;
enum ApproxType { FP, LP, LP_SC };
enum TrainType { BP, RP, CDLM, CDLM_SC, NA };

// Parameter for Weight Randomization
#define MIN_WEIGHT -1.0
#define MAX_WEIGHT 1.0


#define MAX_EPOCH		10000
#define DESIRED_ERROR	0.0001

// BACKPROP Parameter
#define LEARNING_RATE	0.01

// RPROP Parameter
#define DELTA_MIN		0.000001
#define DELTA_MAX		50
#define ETA_PLUS 		1.2
#define ETA_MINUS		0.5
#define INITIAL_STEP	0.1


// Input layer activation function properties
#define LINEAR_SLOPE_INPUT_LAYER 1.0
#define LINEAR_SHIFT_INPUT_LAYER 0.0
#define SIGMOID_SLOPE_INPUT_LAYER 0.5
#define SIGMOID_SHIFT_INPUT_LAYER 0.0
#define SYMMETRIC_SLOPE_INPUT_LAYER 0.5
#define SYMMETRIC_SHIFT_INPUT_LAYER 0.0


// Hidden layer activation function properties
#define LINEAR_SLOPE_HIDDEN_LAYER 1.0
#define LINEAR_SHIFT_HIDDEN_LAYER 0.0
#define SIGMOID_SLOPE_HIDDEN_LAYER 0.5
#define SIGMOID_SHIFT_HIDDEN_LAYER 0
#define SYMMETRIC_SLOPE_HIDDEN_LAYER 0.5
#define SYMMETRIC_SHIFT_HIDDEN_LAYER 0


// Output layer activation function properties
#define LINEAR_SLOPE_OUTPUT_LAYER 0.5
#define LINEAR_SHIFT_OUTPUT_LAYER 0
#define SIGMOID_SLOPE_OUTPUT_LAYER 0.5
#define SIGMOID_SHIFT_OUTPUT_LAYER 0
#define SYMMETRIC_SLOPE_OUTPUT_LAYER 0.5
#define SYMMETRIC_SHIFT_OUTPUT_LAYER 0


// Utility macro functions
#define fann_max(x, y) (((x) > (y)) ? (x) : (y))
#define fann_min(x, y) (((x) < (y)) ? (x) : (y))
#define fann_abs(value) (((value) > 0) ? (value) : -(value))


//Debugging Purpose
#define DEBUG_XML 0
#define DEBUG_NEURALNETWORK 0
#define DEBUG_SYNAPSE 0
#define DEBUG_NEURON 0
#define DEBUG_LAYER 0
#define DEBUG_ACTIVATION_FUNCTION 0

#endif