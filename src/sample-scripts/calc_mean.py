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

if len(sys.argv) <> 2:
	print "Usage: calc_mean.py <file>"
	exit()

filename = sys.argv[1]

values = []
f = open(filename)
for line in f:
	ss = line.split()
	for s in ss:
		try:
			values += [float(s.strip())]
		except:
			pass

sum = 0
count = 0
for v in values:
	if v <> -99:
		sum += v
		count += 1
print sum
print count
print sum/count	

		
