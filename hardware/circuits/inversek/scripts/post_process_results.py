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

def inversek(x, y):
  x_in = float(x)
  y_in = float(y)

  x_2 = pow(x_in, 2.0)
  y_2 = pow(y_in, 2.0)

  num = x_2 + y_2 - l1 * l1 - l2 * l2
  den = 2 * l1 * l2

  theta2 = math.acos(num/den)



  y_part = y_in * (l1 + l2 * math.cos(theta2))
  x_part = x_in * l2 * math.sin(theta2)

  print "x2=%f" % x_2
  print "y2=%f" % y_2
  print "y_part=%f" % y_part
  print "x_part=%f" % x_part
  print "asin_in=%f" %((y_part - x_part) / (x_2 + y_2))

  theta1 = math.asin((y_part - x_part) / (x_2 + y_2))

  return (theta1, theta2)

pass;

input_file 	= open(sys.argv[1])
tot_bits    = int(sys.argv[2])
frac_bits   = int(sys.argv[3])
lines 		= input_file.readlines()

for l in lines:
  l = l.rstrip()
  splitted = l.split()
  x = convert2dec(splitted[0], tot_bits, frac_bits)
  y = convert2dec(splitted[1], tot_bits, frac_bits)
  t1 = convert2dec(splitted[2], tot_bits, frac_bits)
  t2 = convert2dec(splitted[3], tot_bits, frac_bits)
  (theta1, theta2) = inversek(x, y)
  print "%s(%s) %s(%s) %s(%s -- %s) %s(%s -- %s)" % (x, splitted[0], y, splitted[1], t1, splitted[2], str(theta1), t2, splitted[3], str(theta2))

pass;
