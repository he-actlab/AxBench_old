import Image
import numpy
import sys

if len(sys.argv) != 5:
	print 'Usage: txt2img.py <text file> <pixel_w> <pixel_h> <output_image>'
	exit(1)

data=[]
fn=open(sys.argv[1],'r')

pixel_w=int(sys.argv[2])
pixel_h=int(sys.argv[3])

data=fn.readlines()
img=Image.new("L",(pixel_w, pixel_h))

for i in range(pixel_w):
       for j in range(pixel_h):
		data[i*pixel_h+j]=data[i*pixel_h+j].rstrip()
		img.putpixel((i,j),int(data[i*pixel_h+j]))
img.save(sys.argv[4])
fn.close()

