#!/usr/bin/python

import sys
import math
from decimal import *

l1 = 0.5
l2 = 0.5

# theta2 = acos(((x * x) + (y * y) - (l1 * l1) - (l2 * l2))/(2 * l1 * l2)) ;
# theta1 = asin((y * (l1 + l2 * cos(theta2)) - x * l2 * sin(theta2))/(x * x + y * y)) ;

def convert2dec(value_str, tot_bits, frac_bits):
  sign_str=value_str[:1]
  int_str=value_str[1:tot_bits-frac_bits]
  flt_str=value_str[tot_bits-frac_bits:]

  if(sign_str=='1'):
    return -1.0 * float(int(int_str,2) + int(flt_str,2)/float(math.pow(2.0,frac_bits)))
  else:
    return 1.0 * float(int(int_str,2) + int(flt_str,2)/float(math.pow(2.0,frac_bits)))
pass;

def forwardk(theta1, theta2):
  theta1_in = float(theta1)
  theta2_in = float(theta2)

  x_part = l1 * math.cos(theta1_in) + l2 * math.cos(theta1_in + theta2_in)
  y_part = l1 * math.sin(theta1_in) + l2 * math.sin(theta1_in + theta2_in)

  print "theta1   = %f" % theta1_in
  print "theta2   = %f" % theta2_in
  print "x        = %f" % x_part
  print "y        = %f" % y_part

  return (x_part, y_part)

pass;

input_file 	= open(sys.argv[1])
tot_bits    = int(sys.argv[2])
frac_bits   = int(sys.argv[3])
lines 		= input_file.readlines()

for l in lines:
  l = l.rstrip()
  splitted = l.split()
  t1 = convert2dec(splitted[0], tot_bits, frac_bits)
  t2 = convert2dec(splitted[1], tot_bits, frac_bits)
  x = convert2dec(splitted[2], tot_bits, frac_bits)
  y = convert2dec(splitted[3], tot_bits, frac_bits)
  (x_orig, y_orig) = forwardk(t1, t2)
  print "%s(%s) %s(%s) %s(%s -- %s) %s(%s -- %s)" % (t1, splitted[0], t2, splitted[1], x, splitted[2], str(x_orig), y, splitted[3], str(y_orig))

pass;
