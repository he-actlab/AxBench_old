#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Simulating Sobel Filter (Edge Detector)...${nc}"

if [ ! -d ../log ];then
	mkdir ../log
fi

if [ ! -d ../outputs ];then
	mkdir ../outputs
fi

# input image
image_in=$1
filename=$(basename "$image_in")
filename="${filename%.*}"
width=$2
height=$3
size=$((width*height))

# convert image to txt
python img2txt.py $1 ../inputs/${filename}.txt

# update the testbench
sed -e "s/<input>/${filename}.txt/g" -e "s/<size>/${size}/g" ../testbench/sobel_tb.template > ../testbench/sobel_tb.v

# run the simulation
ncverilog ../testbench/sobel_tb.v > ../log/sobel_tb.log

# convert txt to image
python txt2img.py ../outputs/${filename}.txt ${width} ${height} ../outputs/images/${filename}.png


echo -e "${blue}Thanks you...${nc}\n"