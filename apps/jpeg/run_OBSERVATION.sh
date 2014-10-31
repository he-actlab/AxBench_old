#!/bin/bash

BENCHMARK=jpeg

# Run the program
echo -e "# Make the $BENCHMARK for running in observation mode...\n"
make NPU_MODE=NPU_OBSERVATION clean
make NPU_MODE=NPU_OBSERVATION
IMAGE=peppers-512px
echo -en "\033[36m"
./bin/${BENCHMARK}_NPU_OBSERVATION.out ./data/${IMAGE}.rgb ./data/${IMAGE}_observation.jpg
echo -en "\033[0m"
echo ""
echo "Thank you for using ** npu.bench **..."
echo ""