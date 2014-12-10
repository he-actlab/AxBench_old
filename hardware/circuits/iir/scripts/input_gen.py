#Implements edge detection on an image using Sobel Operator
from PIL import Image, ImageFilter, ImageOps
import sys
import numpy
def img2txt(imageName, outputname):

	out=open(outputname, "w")

	#open image and convert to grayscale
	im = Image.open(imageName)
	im = ImageOps.grayscale(im)
	pixels = im.getdata()

	for x in range(len(pixels)):
		out.write("%d\n"%(pixels[x]))
		
	out.close()

if __name__ == '__main__':
    img2txt(sys.argv[1],sys.argv[2])


	
