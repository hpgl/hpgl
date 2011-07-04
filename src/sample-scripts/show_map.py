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

if len(sys.argv) < 4:
	print "Usage: show_map.py <x> <y> <file> <min_value>"
	exit()

x = int(sys.argv[1])
y = int(sys.argv[2])
filename = sys.argv[3]

values = []
f = open(filename)
for line in f:
	ss = line.split()
	for s in ss:
		try:
			values += [float(s.strip())]
		except:
			pass

print "Values: ", len(values)

values = array(values).reshape(x,y)

#imshow(values, vmin=float(sys.argv[4]))
imshow(values)

show()
		
