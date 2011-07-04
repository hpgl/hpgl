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

prop_cont = load_cont_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99, size_prop)
ik_prop = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99,[0,1], size_prop)

grid = SugarboxGrid(166, 141, 20)

sk_prop = simple_kriging(prop_cont, grid, 
		radiuses = (20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10, 10, 10),
		sill = 1)

ik_data =  [    {
                        "cov_type": 1, 
                        "ranges": (10, 10, 10),
                        "sill": 1,
                        "radiuses": (20, 20, 20),
                        "max_neighbours": 12,
                        "marginal_prob": 0.8,
                        "value": 0
                },
                {
                        "cov_type": 1, 
                        "ranges": (10, 10, 10),
                        "sill": 1,
                        "radiuses": (20, 20, 20),
                        "max_neighbours": 12,
                        "marginal_prob": 0.2,
                        "value": 1
                }]

sum_ar = zeros( (sk_prop[0].shape), dtype = float32)
sum_ar = require(sum_ar, requirements = 'F')
s = []
for k in range(1, n+1):
	#sis_res = sis_simulation (ik_prop, grid, ik_data, seed = (94234523/ k) - (234432 / k))
	#sis_res = sis_simulation (ik_prop, grid, ik_data, seed = (94234523/ k) - (234432 / k), workers_count = 2, force_parallel = True, use_psis_5 = True)
	#sis_res = sis_simulation (ik_prop, grid, ik_data, seed = (94234523/ k) - (234432 / k), workers_count = 2, force_parallel = True)

	#sis_res = sis_simulation (ik_prop, grid, ik_data, seed = (94234523/ k) - (234432 / k), use_corellogram = False)
	#sis_res = sis_simulation (ik_prop, grid, ik_data, seed = (94234523/ k) - (234432 / k), use_corellogram = False, workers_count = 2, force_parallel = True, use_psis_5 = True)
	sis_res = sis_simulation (ik_prop, grid, ik_data, seed = (94234523/ k) - (234432 / k), use_corellogram = False, workers_count = 2, force_parallel = True)

	sum_ar += sis_res[0]
	#print (sum ( (sum_ar / k) - sk_prop[0]) ) / (166*141*20)
	s.append( (sum (abs( (sum_ar / k) - sk_prop[0]) ) ) / (166*141*20) )
print "Summa:"
print s

figure()
plot(s)
show()