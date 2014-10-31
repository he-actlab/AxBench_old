#!/bin/bash

BENCHMARK=fft

# Run the program
echo -e "# Make the $BENCHMARK for running in observation mode...\n"
make NPU_MODE=NPU_OBSERVATION clean
make NPU_MODE=NPU_OBSERVATION
echo -en "\033[36m"
./bin/${BENCHMARK}_NPU_OBSERVATION.out
echo -en "\033[0m"


