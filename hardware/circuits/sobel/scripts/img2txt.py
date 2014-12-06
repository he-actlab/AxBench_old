#Implements edge detection on an image using Sobel Operator
from PIL import Image, ImageFilter, ImageOps
import sys
import numpy
def img2txt(imageName, outputname):

	out=open(outputname, "w")
	#open image and convert to grayscale
	im = Image.open(imageName)
	im = ImageOps.grayscale(im)

	#Dimensions
	size = im.getbbox()
	columns = size[2]
	rows = size[3]

	for x in range(0,columns):
		for y in range(0,rows):
			p = build3x3(x,y,im,rows,columns)
			out.write("%d %d %d %d %d %d %d %d %d\n"%(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]))
	out.close()

#Returns array of 3x3 block where center entry is input pixel
def build3x3(x,y,im,rows,columns):
	#Creates 1d array of all pixels by iterating through rows
	pixels = im.getdata()

	upperleft, above, upperright, left, center, right, lowerleft, below, lowerright = 0,0,0,0,0,0,0,0,0

	#Uses row major order to take neighboring pixels out of 1d array
	if x > 0:
		if y > 0:
			upperleft = pixels[((y-1)*columns)+(x-1)]
			above = pixels[((y-1)*columns)+x]
		left = pixels[(y*columns)+(x-1)]
		center = pixels[(y*columns)+x]
		if y < rows-1:
			lowerleft = pixels[((y+1)*columns)+(x-1)]
			below = pixels[((y+1)*columns)+x]

	if x < columns-1:
		if y > 0:
			upperright = pixels[((y-1)*columns)+(x+1)]
		right = pixels[(y*columns)+(x+1)]
		if  y < rows-1:
			lowerright = pixels[((y+1)*columns)+(x+1)]

	block = (upperleft,above,upperright,left,center,right,lowerleft,below,lowerright)
	return block

if __name__ == '__main__':
    img2txt(sys.argv[1],sys.argv[2])


	
