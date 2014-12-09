#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Simulating 5-taps FIR Filter...${nc}"

if [ ! -d ../log ];then
	mkdir ../log
fi

if [ ! -d ../outputs ];then
	mkdir ../outputs
fi

if [ ! -d ../inputs ];then
	mkdir ../inputs
fi

# generate two files for random numbers (call them full)
python input_gen.py images/lion_512x512.jpg ../inputs/in.txt

# sed to change the number of repeat in the simulation
updated_size=$(($1-1))
sed "s/<size>/$updated_size/g" ../testbench/fir_tb.template > ../testbench/fir_tb.v

# run the simulation
ncverilog ../testbench/fir_tb.v > ../log/fir_tb.log

# post-process the results to see all the additions are correct
python post_process_results.py ../outputs/out.txt > ../outputs/out_full.txt

echo -e "${blue}Thanks you...${nc}\n"