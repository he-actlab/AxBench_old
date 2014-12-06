#!/usr/bin/python

import sys
import math
from decimal import *

def convert2dec(value_str, tot_bits, frac_bits):
  sign_str=value_str[:1]
  int_str=value_str[1:tot_bits-frac_bits]
  flt_str=value_str[tot_bits-frac_bits:]

  if(sign_str=='1'):
    return -1.0 * float(int(int_str,2) + int(flt_str,2)/float(math.pow(2.0,frac_bits)))
  else:
    return 1.0 * float(int(int_str,2) + int(flt_str,2)/float(math.pow(2.0,frac_bits)))
pass;

input_file 	= open(sys.argv[1])
tot_bits    = int(sys.argv[2])
frac_bits   = int(sys.argv[3])
lines 		= input_file.readlines()

for l in lines:
	l = l.rstrip()
	splitted = l.split()
	a = convert2dec(splitted[0], tot_bits, frac_bits)
	b = convert2dec(splitted[1], tot_bits, frac_bits)
	c = convert2dec(splitted[2], tot_bits, frac_bits)

	print "%s(%s) * %s(%s) = %s(%s)" % (a, splitted[0], b, splitted[1], c, splitted[2])

pass;
