#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Compiling 5x5 Recurssive Gaussian...${nc}"

if [ ! -d ../log ];then
	mkdir ../log
fi

ncverilog -C ../rtl/kogge_stone_16b.v > ../log/kogge_stone_16b.log
ncverilog -C ../rtl/floating_adder_16b.v > ../log/floating_adder_16b.log
ncverilog -C ../rtl/floating_multiplier_16b.v > ../log/floating_multiplier_16b.log
ncverilog -C ../rtl/gaussian_rom.v > ../log/gaussian_rom.log
ncverilog -C ../rtl/gaussian.v > ../log/gaussian.log

echo -e "${blue}Thanks you...${nc}\n"