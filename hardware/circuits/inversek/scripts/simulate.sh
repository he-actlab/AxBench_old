#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Simulating Inverse Kinematics...${nc}"

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
python floating_gen.py $1 -0.5 0.5 32 15 ../inputs/x_input_full.txt
python floating_gen.py $1 -0.5 0.5 32 15 ../inputs/y_input_full.txt

# awk to only get the first column of the inputs full
awk '{print $1}' ../inputs/x_input_full.txt > ../inputs/x_input.txt 
awk '{print $1}' ../inputs/y_input_full.txt > ../inputs/y_input.txt


# sed to change the number of repeat in the simulation
sed "s/<size>/$1/g" ../testbench/inversek_tb.template > ../testbench/inversek_tb.v

ncverilog ../testbench/inversek_tb.v > ../log/inversek_tb.log

# post-process the results to see all the additions are correct
python post_process_results.py ../outputs/theta_output.txt 32 15 > ../outputs/theta_output_full.txt

echo -e "${blue}Thanks you...${nc}\n"