from PIL import Image, ImageOps
import sys
import math
from decimal import *
import re

def build5x5(x,y,im,rows,columns):
	pixels = im.getdata()
	one = pixels[((y-2)*columns)+(x-2)] /255.0
	one = convert2fixed(float(one),int(sys.argv[2]), int(sys.argv[3]))
	two = pixels[((y-2)*columns)+(x-1)] /255.0
	two = convert2fixed(float(two),int(sys.argv[2]), int(sys.argv[3]))
	three = pixels[((y-2)*columns)+(x)] /255.0
        three = convert2fixed(float(three),int(sys.argv[2]), int(sys.argv[3]))
	four = pixels[((y-2)*columns)+(x+1)] /255.0
        four = convert2fixed(float(four),int(sys.argv[2]), int(sys.argv[3]))
	five = pixels[((y-2)*columns)+(x+2)] /255.0
        five = convert2fixed(float(five),int(sys.argv[2]), int(sys.argv[3]))
	upperleft = pixels[((y-1)*columns)+(x-1)] /255.0
	seven = convert2fixed(float(upperleft),int(sys.argv[2]), int(sys.argv[3]))
	above = pixels[((y-1)*columns)+x] /255.0
	eight = convert2fixed(float(above),int(sys.argv[2]), int(sys.argv[3]))
	left = pixels[(y*columns)+(x-1)] /255.0
	twelve = convert2fixed(float(left),int(sys.argv[2]), int(sys.argv[3]))
	center = pixels[(y*columns)+x] /255.0
	thirteen = convert2fixed(float(center),int(sys.argv[2]), int(sys.argv[3]))
	lowerleft = pixels[((y+1)*columns)+(x-1)] /255.0
	seventeen = convert2fixed(float(lowerleft),int(sys.argv[2]), int(sys.argv[3]))
	below = pixels[((y+1)*columns)+x] /255.0
	eighteen = convert2fixed(float(below),int(sys.argv[2]), int(sys.argv[3]))
	upperright = pixels[((y-1)*columns)+(x+1)] /255.0
	nine = convert2fixed(float(upperright),int(sys.argv[2]), int(sys.argv[3]))
	right = pixels[(y*columns)+(x+1)] /255.0
	fourteen = convert2fixed(float(right),int(sys.argv[2]), int(sys.argv[3]))
	lowerright = pixels[((y+1)*columns)+(x+1)] /255.0
	nineteen = convert2fixed(float(lowerleft),int(sys.argv[2]), int(sys.argv[3]))
	six = pixels[((y-1)*columns)+(x-2)] /255.0
        six = convert2fixed(float(six),int(sys.argv[2]), int(sys.argv[3]))
	eleven = pixels[((y)*columns)+(x-2)] /255.0
        eleven = convert2fixed(float(eleven),int(sys.argv[2]), int(sys.argv[3]))
	sixteen = pixels[((y+1)*columns)+(x-2)] /255.0
        sixteen = convert2fixed(float(sixteen),int(sys.argv[2]), int(sys.argv[3]))
	ten = pixels[((y-1)*columns)+(x+2)] /255.0
        ten = convert2fixed(float(ten),int(sys.argv[2]), int(sys.argv[3]))
        fifteen = pixels[((y)*columns)+(x+2)] /255.0
        fifteen = convert2fixed(float(fifteen),int(sys.argv[2]), int(sys.argv[3]))
        twenty = pixels[((y+1)*columns)+(x+2)] /255.0
        twenty = convert2fixed(float(twenty),int(sys.argv[2]), int(sys.argv[3]))
	twentyone = pixels[((y+2)*columns)+(x-2)] /255.0
        twentyone = convert2fixed(float(twentyone),int(sys.argv[2]), int(sys.argv[3]))
	twentytwo = pixels[((y+2)*columns)+(x-1)] /255.0
        twentytwo = convert2fixed(float(twentytwo),int(sys.argv[2]), int(sys.argv[3]))
	twentythree = pixels[((y+2)*columns)+(x)] /255.0
        twentythree = convert2fixed(float(twentythree),int(sys.argv[2]), int(sys.argv[3]))
	twentyfour = pixels[((y+2)*columns)+(x+1)] /255.0
        twentyfour = convert2fixed(float(twentyfour),int(sys.argv[2]), int(sys.argv[3]))
	twentyfive = pixels[((y+2)*columns)+(x+2)] /255.0
        twentyfive = convert2fixed(float(twentyfive),int(sys.argv[2]), int(sys.argv[3]))

	block = (one,two,three, four, five, six, seven, eight, nine, ten, eleven, twelve, thirteen, fourteen, fifteen, sixteen, seventeen, eighteen, nineteen, twenty, twentyone, twentytwo, twentythree, twentyfour, twentyfive)
	return block


def convert2fixed(value, tot_bits, frac_bits):
  (frac_value, int_value)  = math.modf(value)
  flag = 0
  if(abs(int_value) >= pow(2.0, tot_bits-frac_bits-1)):
    print "Integer saturation!"
    int_value = pow(2.0, tot_bits-frac_bits-1)-1
  if(abs(frac_value) > ((pow(2.0,frac_bits)-1) / math.pow(2.0,frac_bits))):
    print "Float saturation!"
    frac_value = (pow(2.0,frac_bits)-1) / math.pow(2.0,frac_bits)
    flag = 1
  else:
    frac_value = (frac_value * math.pow(2.0,frac_bits))

  int_value  = int(int_value)
  if (flag ==0):
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



if(len(sys.argv) != 5):
    print "Usage: sobel_input.py input_image #bits #fraction output_text_file"
    exit(1)

im = Image.open(sys.argv[1])
im = ImageOps.grayscale(im)
#Dimensions
size = im.getbbox()
columns = size[2]
rows = size[3]
f = open(sys.argv[4],'w')
for x in range(2,columns-2):
  for y in range(2,rows-2):
    p = build5x5(x,y,im,rows,columns)
    string = '\n'.join(map(str,(p)))
    f.write(string+'\n')

