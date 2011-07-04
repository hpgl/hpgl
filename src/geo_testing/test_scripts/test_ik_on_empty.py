#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from geo import *
from sys import *

grid = SugarboxGrid(166, 141, 20)

ik_prop = load_ind_property("test_data/BIG_N_EMPTY.INC", -99, [0,1])
sgs_prop = load_cont_property("test_data/BIG_N_EMPTY.INC", -99)
ik_data =  [	{
			"cov_type": 0, 
			"ranges": (10, 10, 10),
			'sill': 0.4,
			"radiuses": (10, 10, 10),
			"max_neighbours": 12,
			"marginal_prob": 0.9,
			"value": 0
		},
		{
			"cov_type": 0, 
			"ranges": (10, 10, 10),
			"sill": 0.4,
			"radiuses": (10, 10, 10),
			"max_neighbours": 12,
			"marginal_prob": 0.1,
			"value": 1
		}]
		
multi_ik_data = [
	{
		"cov_type": 0,
		"ranges": (4, 4, 4),
		"sill": 0.25,
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"marginal_prob": 0.24,
		"value": 0
	},
	{
		"cov_type": 0,
		"ranges": (6, 6, 6),
		"sill": 0.25,
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"marginal_prob": 0.235,
		"value": 1
	},
	{
		"cov_type": 0,
		"ranges": (2, 2, 2),
		"sill": 0.25,
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"marginal_prob": 0.34,
		"value": 2
	},
	{
		"cov_type": 0,
		"ranges": (10, 10, 10),
		"sill": 0.25,
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"marginal_prob": 0.18,
		"value": 3
	}]		

#print "Doing IK..."
#ik_result = indicator_kriging(ik_prop, grid, ik_data, False)
#write_property(ik_result, "results/RESULT_IK_ON_EMPTY.INC", "IK", -99)
#print "Done"

print "Doing SIS..."
sis_result = sis_simulation(ik_prop, grid, ik_data, seed=3241347)
write_property(sis_result, "results/RESULT_SIS_ON_EMPTY.INC", "SIS_RESULT", -99)
print "Done"
print "Doing SGS..."
sgs_params = {
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"covariance_type": covariance.exponential,
		"ranges": (10, 10, 10),
		"sill": 0.4}
sgs_result = sgs_simulation(sgs_prop, grid, seed=3439275, **sgs_params)
write_property(sgs_result, "results/RESULT_SGS_ON_EMPTY.INC", "SGS_RESULT", -99)
print "Done"


stdin.readline()
