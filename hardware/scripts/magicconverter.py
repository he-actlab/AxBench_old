#!/usr/bin/python

# Amir Yazdanbakhsh
# This script provides an easy way to convert "floating" point numbers in decimal to binary "fixed" point representation.
# It provides a flexible way to change the number of bits for integer and fraction parts

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


def convert2dec(value_str, tot_bits, frac_bits):
  sign_str=value_str[:1]
  int_str=value_str[1:tot_bits-frac_bits]
  flt_str=value_str[tot_bits-frac_bits:]

  if(sign_str=='1'):
    return -1.0 * float(int(int_str,2) + int(flt_str,2)/float(math.pow(2.0,frac_bits)))
  else:
    return 1.0 * float(int(int_str,2) + int(flt_str,2)/float(math.pow(2.0,frac_bits)))
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

def usage():
  print bcolors.FAIL + "\nUsage: " + bcolors.ENDC
  print bcolors.FAIL + "       1) converter [b2d|d2b] [number of bits] [number of fractions] [binary value | decimal value]" + bcolors.ENDC
  print bcolors.FAIL + "       2) converter -f [input file] -o [output file]" + bcolors.ENDC
  print bcolors.FAIL + "       3) converter --help\n" + bcolors.ENDC  
  exit(1)
pass;

def main():

  if(len(sys.argv) < 2):
    usage()

  if(sys.argv[1]=="--help"):
    usage()

  # first argument should always be either "d2b" or "b2d"
  if(sys.argv[1] != "d2b" and sys.argv[1] != "b2d" and sys.argv[1] != "-f"):
    usage()

  if(sys.argv[1] == "-f"):
    if(len(sys.argv) < 5):
      usage()
    elif(sys.argv[3] != "-o"):
      usage()
  pass;

  isfile = True if (sys.argv[1] == "-f") else False
  
  if(isfile):
    fh = 0
    outh = 0
    try:
      fh    = open(sys.argv[2], 'r')
      outh  = open(sys.argv[4], 'w')
    except:
      print bcolors.FAIL + "%s does not exist!" %(sys.argv[2]) + bcolors.ENDC
      usage()

    print bcolors.OKBLUE + "Start processing the file..." + bcolors.ENDC
    lines = fh.readlines()
    for line in lines:
      line = line.rstrip()
      splitted = line.split()
      n_bits = 0
      n_frac = 0
      value  = ""
      result = ""
      if(len(splitted) != 4):
        print bcolors.FAIL + "Error in input file!" + bcolors.ENDC
      try:
        n_bits = int(splitted[1])
        n_frac = int(splitted[2])
        value  = splitted[3]
      except:
        print bcolors.FAIL + "Error in input file!" + bcolors.ENDC

      if(splitted[0] == "d2b"):
        result = convert2fixed(float(value), n_bits, n_frac)
        outh.write("%s.%s" % (result[0:(n_bits-n_frac)], result[(n_bits-n_frac):n_bits]))
        if(line != lines[len(lines)-1]):
          outh.write("\n")
      elif(splitted[0] == "b2d"):
        result = convert2dec(value, n_bits, n_frac)
        outh.write(str(result))
        if(line != lines[len(lines)-1]):
          outh.write("\n")
      else:
        usage()
    pass;
    outh.close()
    print bcolors.OKGREEN + "Thanks for using converter!" + bcolors.ENDC
  else:
      if(len(sys.argv) != 5):
        usage()
      n_bits = 0
      n_frac = 0
      value  = ""
      try:
        n_bits = int(sys.argv[2])
        n_frac = int(sys.argv[3])
        value  = sys.argv[4]
      except:
        usage()

      if(sys.argv[1] == "d2b"):
        value = float(value)
        result = convert2fixed(value, n_bits, n_frac)
        print bcolors.OKBLUE + "Result: %s.%s" % (result[0:(n_bits-n_frac)], result[(n_bits-n_frac):n_bits]) + bcolors.ENDC
        print bcolors.OKGREEN + "Thanks for using converter!" + bcolors.ENDC
      else:
        result = convert2dec(value, n_bits, n_frac)
        print bcolors.OKBLUE + "Result: %s" % (result) + bcolors.ENDC
        print bcolors.OKGREEN + "Thanks for using converter!" + bcolors.ENDC
      pass;
  pass;

pass;



if __name__ == "__main__":
  main()