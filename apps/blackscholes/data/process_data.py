#!/usr/bin/python

import math

import sys

filename = sys.argv[1]
print("Filename is: " + filename)

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

print "Size:" + str(len(price_orig))



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
print "----------- Original Prices -------------------"
print "# Total item:             " + str(len(price_orig))
print "# Total Relative Error:   " + str(absError)
print "# Average Relative Error: " + str(absError/count)
print "# Threshold:              " + str(countOverThreshold/count)



# absError = 0
# count = 0
# countOne = 0

# for i in range(len(price_threshold)):
# 	if(price_orig[i] != 0):
# 		relativeError = abs(price_orig[i]-price_threshold[i]) / abs(price_orig[i])
# 		absError += relativeError
# 		if(relativeError >= 1):
# 			countOne = countOne + 1
# 		count=count+1

# print "----------- After Thereshold Prices -------------------"
# print "# Total item:             " + str(len(price_threshold))
# print "# Total Relative Error:   " + str(absError)
# print "# Average Relative Error: " + str(absError/count)
# print "# Error GEQ One:          " + str(countOne)



# absError = 0
# count = 0
# excludedItems = 0
# pricelessthanOne = 0

# threshold=10e-1
# countMoreThereshold=0

# relativeErrorArray = []


# for i in range(len(price_threshold)):
# 	if(price_orig[i] != 0):
# 		relativeError = abs(price_orig[i]-price_threshold[i]) / abs(price_orig[i])

# 		if(relativeError < 1):
# 			count = count + 1
# 			absError += relativeError
# 			relativeErrorArray.append(relativeError)
# 			if(abs(price_orig[i]-price_threshold[i]) > threshold):
# 				countMoreThereshold = countMoreThereshold + 1
# 		else:
# 			#print price_threshold[i]
# 			if(price_orig[i] < 2):
# 				pricelessthanOne = pricelessthanOne + 1
# 			excludedItems = excludedItems + 1

# averageError = sum(relativeErrorArray)/float(len(relativeErrorArray))
# deviation = 0
# for i in relativeErrorArray:
# 	deviation += (i - averageError) * (i - averageError)




# print "----------- Exclude High Errors -------------------"
# print "# Total item:             " + str(len(price_threshold))
# print "# Total Relative Error:   " + str(absError)
# print "# Average Relative Error: " + str(absError/count)
# print "# Excluded Errors:        " + str(excludedItems)
# print "# Exclude Percentage:     " + str(excludedItems/float(len(price_threshold)))
# print "# Price less than one:    " + str(pricelessthanOne)
# print "# More than threshold:    " + str(countMoreThereshold)
# print "# Error deviation:        " + str(math.sqrt(deviation/float(len(relativeErrorArray))))


