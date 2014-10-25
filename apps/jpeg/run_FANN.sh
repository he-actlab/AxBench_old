#!/bin/bash

BENCHMARK=jpeg

# Run the program with the FANN
echo -e "# Make the $BENCHMARK for running with the FANN...\n"
make NPU_MODE=NPU_FANN clean
make NPU_MODE=NPU_FANN
IMAGE=peppers-512px
./bin/${BENCHMARK}_NPU_FANN.out ./data/${IMAGE}.rgb ./data/${IMAGE}_FANN.jpg ./cfg/${BENCHMARK}_FANN.nn
compare -metric RMSE ./data/${IMAGE}_FANN.jpg ./data/${IMAGE}.jpg null
