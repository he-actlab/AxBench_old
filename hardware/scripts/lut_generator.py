#!/usr/bin/python

# Amir Yazdanbakhsh
# a.yazdanbakhsh@gatech.edu
# Dec. 19 - 2014

import sys
import math
import magicconverter # user defined function

func_name_list = ["exp", "log", "sqrt"]

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'


def powTwoBit(number):
	return (number & (number-1) == 0) and (number != 0)
	pass

def printUsage():
	print "Usage: python lut_generatory.py [exp|log|sqrt] [min] [max] [# of bits] [# fractional bits] [# enteries] [# outputfile]"
	exit(1)
	pass

def printInfo(func_name, min_range, max_range, tot_bits, frac_bits, num_entr):
	print bcolors.OKBLUE + ">>> Generating LUT for %s with %d enteries in the range [%f, %f] <<< " % (func_name, num_entr, min_range, max_range) + bcolors.ENDC
	print bcolors.OKBLUE + ">>>  Fixed-Point Precision: Q%d.%d <<<" %(tot_bits-frac_bits-1, frac_bits) + bcolors.ENDC
	pass

def check_func(func_name):
	if(func_name not in func_name_list):
		print ""
		print bcolors.FAIL + "The following functions are supported:"
		for i in range(len(func_name_list)):
			print "	%d) %s" %(i+1, func_name_list[i])
		print bcolors.ENDC,
		exit(1)
	pass

def check_min_max(min_range, max_range):
	if(min_range >= max_range):
		print ""
		print bcolors.FAIL + "Min range should be greater less than max range!" + bcolors.ENDC
		print ""
		exit(1)
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

def main(argv):
	if(len(argv) != 8):
		printUsage()
	pass

	func_name = argv[1]
	min_range = float(argv[2])
	max_range = float(argv[3])
	tot_bits  = int(argv[4])
	frac_bits = int(argv[5])
	num_entr  = int(argv[6])
	output_name = argv[7]

	# open Verilog file for writing
	verilog_file = open(output_name, "w")

	# check if the function is supported by the tool
	check_func(func_name)

	# check max be greater than min
	check_min_max(min_range, max_range)

	# check if the number of entries is power of two
	if((not powTwoBit(num_entr)) or (not powTwoBit(int(abs(max_range-min_range))))):
		print bcolors.FAIL + "We only support number of entries (ranges) which are power of two!" + bcolors.ENDC
		exit(1)

	# print some information about the function
	printInfo(func_name, min_range, max_range, tot_bits, frac_bits, num_entr)

	# find the index of the function in the func list
	func_index = func_name_list.index(func_name)
	func_name_method = func_name + "_func"

	# find the step
	step = abs(max_range - min_range) / float(num_entr)
	print "-----------------------------"
	print bcolors.OKBLUE + " Step: %f" % (step) + bcolors.ENDC
	print "-----------------------------"

	# file header
	verilog_file.write("// Developed by: 	Amir Yazdanbakhsh\n")
	verilog_file.write("// Email: 			a.yazdanbakhsh@gatech.edu\n\n")
	verilog_file.write("`timescale 1ns/1ps\n")
	verilog_file.write("module %s_lut(d_in, d_out);\n" %(func_name))
	verilog_file.write("\tinput\t\t[%d:0] d_in;\n" % (tot_bits-1))
	verilog_file.write("\toutput\treg\t[%d:0] d_out;\n\n" %(tot_bits-1))
	verilog_file.write("\twire\t[%d:0] addr;\n\n" % (int(math.log(num_entr,2))-1))
	verilog_file.write("\talways @(addr)\n")
	verilog_file.write("\t\tbegin:lut_rom\n")
	verilog_file.write("\t\t\tcase(addr)\n")

	# generate the case values
	if((min_range >= 0)): # both min and max are positive
		curr_value = min_range
		for i in range(num_entr):
			curr_output = globals()[func_name_method](curr_value)
			curr_binary = magicconverter.convert2fixed(curr_output, tot_bits, frac_bits)
			verilog_file.write("\t\t\t\t%d'd%d: d_out <= %d'b%s; // d_in = %f, d_out = %f\n" %(int(math.log(num_entr,2)), i, tot_bits, curr_binary, curr_value, curr_output))
			curr_value +=  step
		pass
	elif((max_range <= 0)): # both min and max are negative
		curr_value = max_range
		for i in range(num_entr):
			curr_output = globals()[func_name_method](curr_value)
			curr_binary = magicconverter.convert2fixed(curr_output, tot_bits, frac_bits)
			verilog_file.write("\t\t\t\t%d'd%d: d_out <= %d'b%s; // d_in = %f, d_out = %f\n" %(int(math.log(num_entr,2)), i, tot_bits, curr_binary, curr_value, curr_output))
			curr_value -=  step
		pass
	else: # min is negative and max is positive
		curr_value = 0.0
		num_pos_entries = int(max_range / step)
		num_neg_entries = num_entr - num_pos_entries
		# first positive numbers
		for i in range(num_pos_entries):
			curr_output = globals()[func_name_method](curr_value)
			curr_binary = magicconverter.convert2fixed(curr_output, tot_bits, frac_bits)
			verilog_file.write("\t\t\t\t%d'd%d: d_out <= %d'b%s; // d_in = %f, d_out = %f\n" %(int(math.log(num_entr,2)), i, tot_bits, curr_binary, curr_value, curr_output))
			curr_value +=  step
		curr_value = 0.0

		curr_index = num_pos_entries
		for i in range(num_neg_entries):
			curr_output = globals()[func_name_method](curr_value)
			curr_binary = magicconverter.convert2fixed(curr_output, tot_bits, frac_bits)
			verilog_file.write("\t\t\t\t%d'd%d: d_out <= %d'b%s; // d_in = %f, d_out = %f\n" %(int(math.log(num_entr,2)), curr_index, tot_bits, curr_binary, curr_value, curr_output))
			curr_value -=  step
			curr_index += 1
		rem_bits = int(math.log(num_entr,2)) - math.log(num_neg_entries,2)

	
	

	verilog_file.write("\t\t\tendcase\n")
	verilog_file.write("\t\tend\n")
	verilog_file.write("\taddress_gen u_address_gen(d_in, addr);\n")
	verilog_file.write("endmodule\n")

	# address generator module
	verilog_file.write("module address_gen(d_in, addr);\n")
	verilog_file.write("\tinput  [%d:0] d_in;\n"%(tot_bits-1))
	verilog_file.write("\toutput [%d:0] addr;\n\n"%(int(math.log(num_entr,2))-1))
	

	# generate the address translation
	# find the first one in the 
	first_one_index = tot_bits - magicconverter.convert2fixed(step, tot_bits, frac_bits).index("1")
	if(min_range >= 0):
		frac_range = frac_bits - (first_one_index - 1) # number of bits which is in the fraction part
		rem_int_bits = int(math.log(num_entr,2)) - frac_range
		verilog_file.write("\twire   [%d:0] diff;\n" % (rem_int_bits-1))
		verilog_file.write("\tassign diff = d_in[%d:%d] - %d'd%d;\n" % (frac_bits + math.log(max_range-1,2) , frac_bits, math.log(max_range-1,2)+1, min_range))
		verilog_file.write("\tassign addr = {diff, d_in[%d:%d]};\n" % (frac_bits-1, first_one_index - 1))
		pass
	elif(max_range < 0):
		verilog_file.write("\tassign addr = d_in[%d:%d];\n\n"%(first_one_index+int(math.log(num_entr,2))-2, first_one_index-1))
		pass
	else:
		ones_str = int(rem_bits) * "1"
		verilog_file.write("\tassign addr = d_in[%d] ? {%d'b%s,d_in[%d:%d]} : {d_in[%d:%d]};\n\n"%(tot_bits-1, rem_bits, ones_str, first_one_index+int(math.log(num_entr,2))-2-rem_bits, first_one_index-1, first_one_index+int(math.log(num_entr,2))-2, first_one_index-1))
		pass
	verilog_file.write("endmodule\n")
	
	verilog_file.close()

if __name__ == "__main__":
	main(sys.argv) 