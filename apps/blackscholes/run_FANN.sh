#!/bin/bash

BENCHMARK=blackscholes

# Run the program with the FANN
echo -e "# Make the $BENCHMARK for running with the FANN...\n"
make NPU_MODE=NPU_FANN clean
make NPU_MODE=NPU_FANN
echo -en "\033[36m"
./bin/${BENCHMARK}_NPU_FANN.out ./data/out.txt ./data/prices_sw.txt ./cfg/${BENCHMARK}_FANN.nn
echo -en "\033[0m"
cd data
echo -en "\033[31m"
echo "--------------------"
python process_data.py npu_fann_diff.data
echo "--------------------"
echo -en "\033[0m"
echo ""
echo "Thank you for using ** AxBench **..."
echo ""
cd ..