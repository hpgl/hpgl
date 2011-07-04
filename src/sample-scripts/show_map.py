#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

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
		
