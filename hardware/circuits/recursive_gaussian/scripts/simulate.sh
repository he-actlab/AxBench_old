#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Simulating 5x5 Recurssive Gaussian ...${nc}"

if [ ! -d ../log ];then
	mkdir ../log
fi

if [ ! -d ../outputs ];then
	mkdir ../outputs
fi


if [ ! -d ../outputs/images ];then
	mkdir ../outputs/images
fi

# input image
image_in=$1
filename=$(basename "$image_in")
filename="${filename%.*}"
width=$2
height=$3
size=$((width*height))

# convert image to txt
#python gaussian_in.py $1 16 12 ../inputs/${filename}_in.txt 

# update the testbench
#sed -e "s/<input>/${filename}_in.txt/g" -e "s/<size>/${size}/g" -e "s/<output>/${filename}_out.txt/g" ../testbench/gaussian_tb.template > ../testbench/gaussian_tb.v

# run the simulation
ncverilog ../testbench/gaussian_tb.v > ../log/gaussian_tb.log

# convert txt to image
python post_process.py ../outputs/${filename}_out.txt $1 16 12 ../outputs/images/${filename}.png


echo -e "${blue}Thanks you...${nc}\n"