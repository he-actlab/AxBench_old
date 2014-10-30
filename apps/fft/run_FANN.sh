#!/bin/bash

BENCHMARK=fft

# Run the program with the FANN
echo -e "# Make the $BENCHMARK for running with the FANN...\n"
make NPU_MODE=NPU_FANN clean
make NPU_MODE=NPU_FANN
echo -en "\033[36m"
./bin/${BENCHMARK}_NPU_FANN.out ./cfg/${BENCHMARK}_FANN.nn
echo -en "\033[0m"


