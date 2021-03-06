#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Compiling 32-bit Wallace-Tree Multiplier...${nc}"

if [ ! -d ../log ];then
	mkdir ../log
fi

ncverilog -C ../rtl/multiplier_32b.v > ../log/multiplier_32b.log

echo -e "${blue}Thanks you...${nc}\n"