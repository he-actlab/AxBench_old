#!/usr/bin/python

import sys
import math
from decimal import *

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

def fir(input_arr):
  return ((int(input_arr[0])/32) + (int(input_arr[1])/16) + (int(input_arr[2])/8) + (int(input_arr[3])/4) + (int(input_arr[4])/2))
pass;

input_file 	= open(sys.argv[1])
lines 		= input_file.readlines()

print "x[n]\tx[n-1]\tx[n-2]\tx[n-3]\tx[n-4]\ty[n]\ty[n](sw)"

for l in lines:
  l = l.rstrip()
  splitted = l.split()
  fir_res = fir(splitted)
  print "%s\t%s\t%s\t%s\t%s\t%s\t%d" % (splitted[0], splitted[1], splitted[2], splitted[3], splitted[4], splitted[5], fir_res)

  if(fir_res != int(splitted[5])):
    print bcolors.FAIL + "Error! [%s %s %s %s %s] = %s != %s (sw)" % (splitted[0], splitted[1], splitted[2], splitted[3], splitted[4], splitted[5], str(fir_res)) + bcolors.ENDC
    exit(1)
pass;
