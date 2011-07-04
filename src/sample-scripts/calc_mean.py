#
#
#   Copyright 2009 HPGL Team
#
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.
#
#   HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.
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

		
