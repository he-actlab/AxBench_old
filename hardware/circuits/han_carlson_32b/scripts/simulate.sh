#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Simulating 32-bit Han-Carlson Adder...${nc}"

if [ ! -d ../log ];then
	mkdir ../log
fi

if [ ! -d ../outputs ];then
	mkdir ../outputs
fi

ncverilog ../testbench/han_carlson_32b_tb.v > ../log/han_carlson_32b_tb.log

echo -e "${blue}Thanks you...${nc}\n"