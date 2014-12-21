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
  pass

def exp_func(x):
  return math.exp(x)
  pass

def log_func(x):
  return math.log(x)
  pass

def sqrt_func(x):
  return math.sqrt(x)
  pass

input_file 	= open(sys.argv[1])
tot_bits    = int(sys.argv[2])
frac_bits   = int(sys.argv[3])
func_name   = sys.argv[4]
lines 		= input_file.readlines()


func_name_method = func_name + "_func"

for l in lines:
  l = l.rstrip()
  splitted = l.split()
  d_in  = convert2dec(splitted[0], tot_bits, frac_bits)
  d_out = convert2dec(splitted[1], tot_bits, frac_bits)
  d_out_sw = globals()[func_name_method](d_in)
  print "%s(%f(%s)) = %f(%s) --- SW: %s(%f) = %f\n" % (func_name, d_in, splitted[0], d_out, splitted[1], func_name, d_in, d_out_sw)
pass