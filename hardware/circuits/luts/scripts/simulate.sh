#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Simulating $1 LUT-ROM...${nc}"

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
python floating_gen.py $2 $3 $4 $5 $6 ../inputs/data_in_full.txt

# awk to only get the first column of the inputs full
awk '{print $1}' ../inputs/data_in_full.txt > ../inputs/data_in.txt 

# sed to change the number of repeat in the simulation
sed "s/<size>/$2/g" ../testbench/${1}_lut_tb.template > ../testbench/${1}_lut_tb.v

# run the simulation
ncverilog ../testbench/${1}_lut_tb.v #> ../log/${1}_lut_tb.log

# post-process the results to see all the additions are correct
python post_process_results.py ../outputs/data_out.txt $5 $6 $1 > ../outputs/data_out_full.txt

echo -e "${blue}Thanks you...${nc}\n"