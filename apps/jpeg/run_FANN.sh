#!/bin/bash

BENCHMARK=jpeg

# Run the program with the FANN
echo -e "# Make the $BENCHMARK for running with the FANN...\n"
make NPU_MODE=NPU_FANN clean
make NPU_MODE=NPU_FANN
IMAGE=peppers-512px
echo -en "\033[36m"
./bin/${BENCHMARK}_NPU_FANN.out ./data/${IMAGE}.rgb ./data/${IMAGE}_FANN.jpg ./cfg/${BENCHMARK}_FANN.nn
echo -en "\033[0m"
compare -metric RMSE ./data/${IMAGE}_FANN.jpg ./data/${IMAGE}.jpg null > tmp.log 2> tmp.err
echo -en "\033[31m"
echo "--------------------"
awk '{ 	printf("JPEG Error: %0.2f%\n",substr($2, 2, length($2) - 2) * 100) }' tmp.err
echo "--------------------"
echo -en "\033[0m"
echo ""
echo "Thank you for using ** AxBench **..."
echo ""
rm -rf tmp.err
rm -rf tmp.log
