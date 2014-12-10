#!/usr/bin/python

import sys
import math
from decimal import *

# y[n] =   x[n]  + 2*x[n-1] + 4*x[n-2] + 8*x[n-3] + 16*x[n-4] + 8 *x[n-5] + 4*x[n-6] + 2*x[n-7] +   x[n-8] +
#            y[n-1] + 2*y[n-2] + 4*y[n-3] + 8 *y[n-4] + 16*y[n-5] + 8*y[n-6] + 4*y[n-7] + 2*y[n-8]

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

def iir(input_arr, output_arr):
  feedforward = int(input_arr[0]) + 2*int(input_arr[1]) + 4*int(input_arr[2]) + 8*int(input_arr[3]) + 16*int(input_arr[4]) + 8*int(input_arr[5]) + 4*int(input_arr[6]) + 2*int(input_arr[7]) + int(input_arr[8]) 

  feedback    = int(output_arr[7]) + 2*int(output_arr[6]) + 4*int(output_arr[5]) + 8*int(output_arr[4]) + 16*int(output_arr[3]) + 8*int(output_arr[2]) + 4*int(output_arr[1]) + 2*int(output_arr[0])

  return (feedforward + feedback, feedforward, feedback)
pass;

input_file 	= open(sys.argv[1])
lines 		= input_file.readlines()

print "x[n]\tx[n-1]\tx[n-2]\tx[n-3]\tx[n-4]\tx[n-5]\tx[n-6]\tx[n-7]\tx[n-8]\ty[n]\tff\tfb\ty_sw[n]\tff_sw\tfb_sw"

output_arr = [0,0,0,0,0,0,0,0,0] # 0 -> 7
for l in lines:
  l = l.rstrip()
  splitted = l.split()
  (iir_res, feedforward, feedback) = iir(splitted, output_arr)
  for i in range(0,12):
    print "%s\t" % (splitted[i]),
  print "%s\t%s\t%s" % (str(iir_res), str(feedforward), str(feedback))


  for i in range(0,8):
    output_arr[i] = output_arr[i+1]
  output_arr[8] = iir_res >> 9;

  if(iir_res != int(splitted[9])):
    print bcolors.FAIL + "Error! iir[%s,%s,%s,%s,%s,%s,%s,%s,%s] = %s != %s (sw)" % (splitted[0], splitted[1], splitted[2], splitted[3], splitted[4], splitted[5], splitted[6], splitted[7], splitted[8], splitted[9], str(iir_res)) + bcolors.ENDC
    exit(1)
pass;
