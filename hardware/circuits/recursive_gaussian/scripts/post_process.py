from PIL import Image, ImageOps
import sys
import math
from decimal import *
import re

def convert2dec(value, tot_bits, frac_bits):
  matchObj = re.match(r'[0|1]+', value, re.M|re.I)
  if (not matchObj):
    print "you should provide binary inputs"
    exit(1)

  value_str=value
  sign_str=value_str[:1]
  int_str=value_str[1:int(sys.argv[3])-int(sys.argv[4])]
  flt_str=value_str[int(sys.argv[3])-int(sys.argv[4]):]

  #print sign_str
  #print int(int_str,2)
  #print int(flt_str,2)/float(math.pow(2.0,int(sys.argv[2])))

  if(sign_str=='1'):
    return -1.0 * float(int(int_str,2) + int(flt_str,2)/float(math.pow(2.0,int(sys.argv[4]))))
  else:
    return 1.0 * float(int(int_str,2) + int(flt_str,2)/float(math.pow(2.0,int(sys.argv[4]))))

if(len(sys.argv) != 6):
    print "Usage: post_process.py input_file_txt input_image #bits #fraction output_image"
    exit(1)

data=[]
fn=open(sys.argv[1],'r')

data=fn.readlines()
im = Image.open(sys.argv[2])
im = ImageOps.grayscale(im)
size = im.getbbox()
columns = size[2]
rows = size[3]

for i in range(2,columns-2):
	for j in range(2,rows-2):
		curr_data = data[(i-2)*(rows-4)+(j-2)] 
		curr_data = int(255*convert2dec(curr_data.rstrip(),int(sys.argv[3]),int(sys.argv[4])))
		if curr_data > 255: 
			curr_data = 255
		if curr_data < 0:
			curr_data = 0
		im.putpixel((i,j),int(curr_data))
im.save(sys.argv[5])
fn.close()

