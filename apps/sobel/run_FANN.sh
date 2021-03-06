#!/bin/bash

BENCHMARK=sobel

# Run the program with the FANN
echo -e "# Make the $BENCHMARK for running with the FANN...\n"
make NPU_MODE=NPU_FANN clean
make NPU_MODE=NPU_FANN
IMAGE=peppers
echo -en "\033[36m"
./bin/${BENCHMARK}_NPU_FANN.out ./data/${IMAGE}.rgb ./cfg/${BENCHMARK}_FANN.nn 
echo -en "\033[0m"
cd data
python png2rgb.py png ${IMAGE}.rgb_orig.rgb ${IMAGE}.rgb_orig.png > out1.tmp
python png2rgb.py png ${IMAGE}.rgb_fann.rgb ${IMAGE}.rgb_fann.png > out2.tmp
compare -metric RMSE ${IMAGE}.rgb_orig.png ${IMAGE}.rgb_fann.png null > tmp.log 2> tmp.err
echo -en "\033[31m"
echo "--------------------"
awk '{ 	printf("Sobel Error: %0.2f%\n",substr($2, 2, length($2) - 2) * 100) }' tmp.err
echo "--------------------"
echo -en "\033[0m"
echo ""
echo "Thank you for using ** AxBench **..."
echo ""
rm -rf tmp.err
rm -rf tmp.log
rm -rf out1.tmp
rm -rf out2.tmp