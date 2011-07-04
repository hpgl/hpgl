#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from numpy import *
from scipy import *
from sys import *

def load_gslib_file(filename):
	dict = {}
	list_prop = []
	points = []

	f = open(filename)
	name = f.readline()
	num_p = int(f.readline())
	#print num_p

	for i in xrange(num_p):
		list_prop.append(str(f.readline().strip()))
	#print list_prop

	for i in xrange(len(list_prop)):
		dict[ list_prop[i] ] = array([])

	for line in f:
		points = line.split()
		for j in xrange(len(points)):
			dict[ list_prop[j] ] = concatenate( (dict[ list_prop[j] ],  array([float64(points[j])])) )

	return dict