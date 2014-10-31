#!/bin/bash

BENCHMARK=kmeans

# Run the program
echo -e "# Make the $BENCHMARK for running in observation mode...\n"
make NPU_MODE=NPU_OBSERVATION clean
make NPU_MODE=NPU_OBSERVATION
echo -en "\033[36m"
./bin/${BENCHMARK}_NPU_OBSERVATION.out ./data/peppers-512px.rgb
echo -en "\033[0m"
cd data
python png2rgb.py png peppers-512px.rgb_orig.rgb peppers-512px_observation.png > /tmp/out2
echo ""
echo "Thank you for using ** AxBench **..."
echo ""
