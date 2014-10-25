#!/bin/bash

BENCHMARK=blackscholes

# Run the program with the FANN
echo -e "# Make the $BENCHMARK for running with the FANN...\n"
make NPU_MODE=NPU_FANN clean
make NPU_MODE=NPU_FANN
./bin/${BENCHMARK}_NPU_FANN.out ./data/out.txt ./data/prices_sw.txt ./cfg/${BENCHMARK}_FANN.nn
cd data
python process_data.py npu_fann_diff.data
cd ..