#!/bin/bash

BENCHMARK=jmeint

# Run the program with the FANN
echo -e "# Make the $BENCHMARK for running with the FANN...\n"
make NPU_MODE=NPU_FANN clean
make NPU_MODE=NPU_FANN
./bin/${BENCHMARK}_NPU_FANN.out 100000 ./cfg/${BENCHMARK}_FANN.nn
