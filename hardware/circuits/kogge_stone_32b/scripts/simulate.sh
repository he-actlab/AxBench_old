#!/bin/bash
red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m' # No Color

echo -e "\n${blue}******************************************"
echo -e "*            AxBench Hardware            *"
echo -e "******************************************${nc}"

echo -e "${green} # Simulating 32-bit Kogge-Stone Adder...${nc}"

ncverilog ../testbench/kogge_stone_32b_tb.v > ../log/kogge_stone_32b_tb.log

echo -e "${blue}Thanks you...${nc}\n"