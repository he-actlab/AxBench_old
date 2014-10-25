#!/bin/bash -v

BENCHMARK=kmeans

# Run the program with the FANN
echo -e "# Make the $BENCHMARK for running with the FANN...\n"
make NPU_MODE=NPU_FANN clean
make NPU_MODE=NPU_FANN
./bin/${BENCHMARK}_NPU_FANN.out ./data/peppers-512px.rgb ./cfg/${BENCHMARK}_FANN.nn
cd data
python png2rgb.py png peppers-512px.rgb_fann.rgb peppers-512px_FANN.png
python png2rgb.py png peppers-512px.rgb peppers-512px_orig.png
compare -metric RMSE peppers-512px_FANN.png peppers-512px_orig.png null
