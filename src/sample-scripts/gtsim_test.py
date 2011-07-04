#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from gtsim import *
import time
from sys import *
import os

if not os.path.exists("results/"):
	os.mkdir("results/")

# gtsim test
time1 = time.time()
prop = load_cont_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99)
grid = SugarboxGrid(166, 141, 20)

sk_params = { "radiuses" : (20, 20, 20),
                 "max_neighbours" : 12,
                 "covariance_type" : 1,
                 "ranges" : (10, 10, 10),
                 "sill" : 1}
sgs_params = { "radiuses" : (20, 20, 20),
                 "max_neighbours" : 12,
                 "covariance_type" : 1,
                 "ranges" : (10, 10, 10),
                 "sill" : 1}
                    
#time3 = time.time()
#gtsim_2ind(grid = grid, prop = prop, sgs_params = sgs_params)
result = gtsim_2ind(grid, prop, sk_params, sgs_params)
time2 = time.time()
print "Time: ", (time2-time1), "s."
#prop2 = load_cont_property("SK.INC",-99)
#gtsim_2ind(grid, prop, sk_params, sgs_params, pk_prop=prop2)
#time4 = time.time()
#print "Time: ", (time4-time2+time3-time1), "s."
write_property(result, "results\GTSIM_BIG_SOFT_RESULT.INC", "GTSIM", -99)

print "Checking result..."
print "Hard data saving test..."
prop_init = load_cont_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99)

errors_hard = 0
all_points = 0
for i in xrange(prop_init.size()):
	if prop_init.is_informed(i) == True:
		all_points = all_points + 1
		if(prop_init.get_at(i) <> result.get_at(i)):
			errors_hard = errors_hard + 1
			#print "Error! Point ", all_points, " is not equal"
			#print "Hard value: ", prop_init.get_at(i)
			#print "Simulated value: ", result.get_at(i)

print "Number of points: ", all_points
print "Error points: ", errors_hard
print "Simulated property mean is: ", calc_mean(result)
stdin.readline()
