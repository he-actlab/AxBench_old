#!/bin/bash

BENCHMARK=sobel

# Run the program with the FANN
echo -e "# Make the $BENCHMARK for running with the FANN...\n"
make NPU_MODE=NPU_FANN clean
make NPU_MODE=NPU_FANN
IMAGE=peppers
./bin/${BENCHMARK}_NPU_FANN.out ./data/${IMAGE}.rgb ./cfg/${BENCHMARK}_FANN.nn 
cd data
python png2rgb.py png ${IMAGE}.rgb_orig.rgb ${IMAGE}.rgb_orig.png
python png2rgb.py png ${IMAGE}.rgb_fann.rgb ${IMAGE}.rgb_fann.png
compare -metric RMSE ${IMAGE}.rgb_orig.png ${IMAGE}.rgb_fann.png null
