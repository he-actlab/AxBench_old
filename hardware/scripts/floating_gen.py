#!/usr/bin/python
import sys
import random
import math
from decimal import *

def printUsage():
	print "Usage: python floating_gen.py <items> <min> <max> <N> <Q> <output file>"
	exit(1)
pass;

def convert2fixed(value, tot_bits, frac_bits):

  (frac_value, int_value)  = math.modf(value)
  flag = 0
  if(abs(int_value) >= pow(2.0, tot_bits-frac_bits-1)):
    int_value = pow(2.0, tot_bits-frac_bits-1)-1

  if(abs(frac_value) > ((math.pow(2.0,frac_bits)-1) / math.pow(2.0,frac_bits))):
    frac_value = (math.pow(2.0,frac_bits)-1) / math.pow(2.0,frac_bits)
    flag = 1;
  else:
    frac_value = (frac_value * math.pow(2.0,frac_bits))

  int_value  = int(int_value)
  if (flag==0):
    frac_value = round(frac_value)
  else:
    frac_value = (math.pow(2.0,frac_bits)-1)
  flag = 0

  int_str   = '{0:032b}'.format(int(int_value)) # one bit for sign bit
  frac_str  = '{0:032b}'.format(int(frac_value))

  int_str  = int_str[32-(tot_bits-frac_bits)+1:32]
  frac_str = frac_str[32-(frac_bits):32]

  if(value >=0):
    return "0" + int_str + frac_str
  else:
    return "1" + int_str + frac_str
pass;

def convert2dec(value_str, tot_bits, frac_bits):
  sign_str=value_str[:1]
  int_str=value_str[1:tot_bits-frac_bits]
  flt_str=value_str[tot_bits-frac_bits:]

  if(sign_str=='1'):
    return -1.0 * float(int(int_str,2) + int(flt_str,2)/float(math.pow(2.0,frac_bits)))
  else:
    return 1.0 * float(int(int_str,2) + int(flt_str,2)/float(math.pow(2.0,frac_bits)))
pass;

if(len(sys.argv) != 7):
	printUsage()

n 			=	int(sys.argv[1])
min_float	=	float(sys.argv[2])
max_float	=	float(sys.argv[3])
tot_bits 	= 	int(sys.argv[4])
float_bits 	= 	int(sys.argv[5])
out_file 	=   open(sys.argv[6], "w")

print "# Generating (%d,%d)-floating numbers in binary [%f, %f]" % (tot_bits, float_bits, min_float, max_float)
for i in range(n):
	curr_float = random.uniform(min_float, max_float)
	out_file.write("%s // %s <--> %s\n" % (convert2fixed(curr_float, tot_bits, float_bits), str(convert2dec(convert2fixed(curr_float, tot_bits, float_bits), tot_bits, float_bits)), curr_float))

out_file.close()

