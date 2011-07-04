#!/usr/bin/python

from numpy import *
from scipy import *
from pylab import *
from geo import *

if len(sys.argv) < 5:
	print "Usage: show_map.py <x> <y> <file> <min_value>"
	exit()

x = int(sys.argv[1])
y = int(sys.argv[2])
z = int(sys.argv[3])
filename = sys.argv[4]

prop = load_cont_property(filename, -99, (x,y,z))

#imshow(values, vmin=float(sys.argv[4]))
imshow(prop[0][:,:,0])

show()
		
