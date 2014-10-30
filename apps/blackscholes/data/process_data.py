#!/usr/bin/python

import math

import sys

filename = sys.argv[1]
#print("Filename is: " + filename)

fh = open(filename)

price_ref  = []
price_orig = []
price_appx = []

next(fh)
for line in fh:
	#print line 
	#raw_input()
	line = line.replace('\n','')
	if not line:
		print "end"
	linesplit = line.split('\t')
	price_ref.append(float(linesplit[0]))
	price_orig.append(float(linesplit[1]))
	price_appx.append(float(linesplit[2]))

	# print price_orig
	# print price_appx
	#raw_input()

#print "Size:" + str(len(price_orig))



price_threshold = []

for item in price_appx:
	if(item < 0.6):
		price_threshold.append(0)
	else:
		price_threshold.append(item)


absError = 0
count = 0
countOne = 0
countOverThreshold = 0
threshold = 0.001

for i in range(len(price_orig)):
	if(price_orig[i] != 0.0):
		#mse = ((price_appx[i] - price_orig[i]) * (price_appx[i] - price_orig[i]))
		#print "# MSE: " + str(mse)
		if( ((price_appx[i] - price_orig[i]) * (price_appx[i] - price_orig[i])) > threshold):
			countOverThreshold = countOverThreshold + 1
		relativeError = abs(price_orig[i]-price_appx[i]) / abs(price_orig[i])
		absError += relativeError
		count=count+1
#print "----------- Original Prices -------------------"
#print "# Total item:             " + str(len(price_orig))
#print "# Total Relative Error:   " + str(absError)
print "Error: " + (str(round(absError/count*100,2))) + "%"




