#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Simulating (32, 15)-Floating Point Adder...${nc}"

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
python floating_gen.py $1 -250.0 250.0 32 15 ../inputs/a_input_full.txt
python floating_gen.py $1 -250.0 250.0 32 15 ../inputs/b_input_full.txt

# awk to only get the first column of the inputs full
awk '{print $1}' ../inputs/a_input_full.txt > ../inputs/a_input.txt 
awk '{print $1}' ../inputs/b_input_full.txt > ../inputs/b_input.txt

# sed to change the number of repeat in the simulation
sed "s/<size>/$1/g" ../testbench/floating_multiplier_32b_tb.template > ../testbench/floating_multiplier_32b_tb.v

# run the simulation
ncverilog ../testbench/floating_multiplier_32b_tb.v > ../log/floating_multiplier_32b_tb.log

# post-process the results to see all the additions are correct
python post_process_results.py ../outputs/c_output.txt 32 15 > ../outputs/c_output_full.txt

echo -e "${blue}Thanks you...${nc}\n"