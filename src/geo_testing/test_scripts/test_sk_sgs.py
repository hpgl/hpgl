#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from sys import *
from geo import *
from geo_routines import *
from variogram_routines import *
import os
import time
from pylab import *

size_prop = [166, 141, 20]

n = 100

prop_cont = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC", -99, size_prop)

grid = SugarboxGrid(166, 141, 20)

sk_prop = simple_kriging(prop_cont, grid, 
		radiuses = (10, 10, 10),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (5, 5, 5),
		sill = 1)

sgs_params = {
		"radiuses": (10, 10, 10),
		"max_neighbours": 12,
		"covariance_type": covariance.exponential,
		"ranges": (5, 5, 5),
		"sill": 1,
		"kriging_type": "sk"}

sum_ar = zeros( (sk_prop[0].shape), dtype = float32)
sum_ar = require(sum_ar, requirements = 'F')
s = []

#rg_sz = ( 40, 40, 20 )
for i in xrange(1):
	rg_sz = ( 20 + i*10, 20 + i*10, 20 ) 
	for k in range(1, n+1):
		#sgs_res = sgs_simulation (prop_cont, grid, seed = (94234523/ k) - (234432 / k), **sgs_params)
		sgs_res = sgs_simulation (prop_cont, grid, seed = (94234523/ k) - (234432 / k), workers_count = 2, min_neighbours = 12, region_size = rg_sz, force_parallel = True, use_new_psgs = True, **sgs_params)
		# min_neighbours = 12
		#sgs_res = sgs_simulation (prop_cont, grid, seed = (94234523/ k) - (234432 / k), workers_count = 2, force_parallel = True, **sgs_params)
		sum_ar += sgs_res[0]
		#print (sum ( (sum_ar / k) - sk_prop[0]) ) / (166*141*20)
		s.append( (sum (abs( (sum_ar / k) - sk_prop[0]) ) ) / (166*141*20) )
	figure()
	plot(s)
	show()
	s = []
	sum_ar = zeros( (sk_prop[0].shape), dtype = float32)
	sum_ar = require(sum_ar, requirements = 'F')
#print "Summa:"
#print s


#print "press key"
#stdin.readline()