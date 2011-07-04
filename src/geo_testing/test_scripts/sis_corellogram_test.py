#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from geo import *
from sys import *
from numpy import *
from scipy import *
from python_property import *
from save_property import *
import os

if not os.path.exists("results/"):
	os.mkdir("results/")
if not os.path.exists("results/medium/"):
	os.mkdir("results/medium/")

grid = SugarboxGrid(100, 100, 200)

# 1. Test #1
# Mean realization (summary of n stochastic realizations) without hard data but with mean cube loaded must be ~= mean cube.

# number of realizations
n = 30

ik_prop = load_ind_property("fixed/SIS_TESTING_EMPTY_100_100_200.INC", -99, [0,1])
print 'Property data loaded.'
mean_data = load_indicator_mean_data(["fixed/SIS_TESTING_MEAN_CUBE_100_100_200.INC","fixed/SIS_TESTING_MEAN_CUBE_2_100_100_200.INC"])
#mean_data = load_mean_data("fixed/SIS_TESTING_MEAN_CUBE_100_100_200.INC");
#mean_2_data = load_mean_data("fixed/SIS_TESTING_MEAN_CUBE_2_100_100_200.INC");
#mean_data_list = [ mean_data, mean_2_data ]
print 'Mean data loaded.'
print 'Number of realizations:'
print n

print 'Running Test #1:'

ik_data =  [	{
			"cov_type": 1, 
		        "ranges": (10, 10, 10),
			"sill": 1,
			"radiuses": (20, 20, 20),
			"max_neighbours": 12,
			"marginal_prob": 0.7,
			"value": 0
		},
		{
			"cov_type": 1, 
			"ranges": (10, 10, 10),
			"sill": 1,
			"radiuses": (20, 20, 20),
			"max_neighbours": 12,
			"marginal_prob": 0.3,
			"value": 1
		}]

overrall_prop = zeros( (100,100,200), dtype=float )
		
for k in xrange(n):
	print '\nrealization:'
	print k
	#sis_result = sis_simulation(ik_prop, grid, ik_data, seed=13*k, mean_data=mean_data_list)
	sis_result = sis_simulation(ik_prop, grid, ik_data, seed=13*k, mean_data = mean_data, use_corellogram = True)
	write_property(sis_result, "results/SIS_CORELL_TEST_TEMP.INC", "SIS_CORELL_TEST_TEMP.INC", -99)
	current_prop = load_property_python(100,100,200,"results/SIS_CORELL_TEST_TEMP.INC")
	overrall_prop = overrall_prop + current_prop

overrall_prop = overrall_prop / float(n)
save_property(overrall_prop,100,100,200,"results/test_1_overral_corellogram_sis_result_100_100_200.inc","OVERRAL_RES")
