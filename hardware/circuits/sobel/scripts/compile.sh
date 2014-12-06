#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Compiling Sobel Filter (Edge Detector)...${nc}"

if [ ! -d ../log ];then
	mkdir ../log
fi

ncverilog -C ../rtl/exoor.v > ../log/exoor.log
ncverilog -C ../rtl/fa.v > ../log/fa.log
ncverilog -C ../rtl/not_11b.v > ../log/not_11b.log
ncverilog -C ../rtl/mux_8b.v > ../log/mux_8b.log
ncverilog -C ../rtl/mux_11b.v > ../log/mux_11b.log
ncverilog -C ../rtl/sobel_add_nb.v > ../log/sobel_add_nb.log
ncverilog -C ../rtl/sobel.v > ../log/sobel.log

echo -e "${blue}Thanks you...${nc}\n"