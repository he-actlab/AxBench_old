#!/bin/bash

BENCHMARK=sobel

# Run the program
echo -e "# Make the $BENCHMARK for running in observation mode...\n"
make NPU_MODE=NPU_OBSERVATION clean
make NPU_MODE=NPU_OBSERVATION
IMAGE=peppers
echo -en "\033[36m"
./bin/${BENCHMARK}_NPU_OBSERVATION.out ./data/${IMAGE}.rgb ./data/${IMAGE}.data
echo -en "\033[0m"
cd data
python png2rgb.py png observation.rgb ${IMAGE}.observation.png > out1.tmp
echo ""
echo "Thank you for using ** AxBench **..."
echo ""

rm -rf out1.tmp