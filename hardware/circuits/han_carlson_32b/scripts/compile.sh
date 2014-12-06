#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Compiling 32-bit Han-Carlson Adder...${nc}"

if [ ! -d ../log ];then
	mkdir ../log
fi

ncverilog -C ../rtl/han_carlson_32b.v > ../log/han_carlson_32b.log

echo -e "${blue}Thanks you...${nc}\n"