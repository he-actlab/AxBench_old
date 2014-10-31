#!/bin/bash

BENCHMARK=kmeans

# Run the program with the FANN
echo -e "# Make the $BENCHMARK for running with the FANN...\n"
make NPU_MODE=NPU_FANN clean
make NPU_MODE=NPU_FANN
echo -en "\033[36m"
./bin/${BENCHMARK}_NPU_FANN.out ./data/peppers-512px.rgb ./cfg/${BENCHMARK}_FANN.nn
echo -en "\033[0m"
cd data
python png2rgb.py png peppers-512px.rgb_fann.rgb peppers-512px_FANN.png > /tmp/out1
python png2rgb.py png peppers-512px.rgb_orig.rgb peppers-512px_orig.png > /tmp/out2
compare -metric RMSE peppers-512px_FANN.png peppers-512px_orig.png null > tmp.log 2> tmp.err
echo -en "\033[31m"
echo "--------------------"
awk '{ 	printf("Error: %0.2f%\n",substr($2, 2, length($2) - 2) * 100) }' tmp.err
echo "--------------------"
echo -en "\033[0m"
echo ""
echo "Thank you for using ** AxBench **..."
echo ""
rm -rf tmp.err
rm -rf tmp.log
