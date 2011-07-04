#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from geo import *
from sys import *
import time
import os

if not os.path.exists("results/"):
	os.mkdir("results/")
if not os.path.exists("results/medium/"):
	os.mkdir("results/medium/")

print "loading double property..."

prop = load_cont_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99)
prop_cont = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC", -99)
print "done"

grid = SugarboxGrid(166, 141, 20)


if (not "nook" in argv):
#	while True:
	prop2 = ordinary_kriging(prop_cont, grid,
		radiuses = (20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10, 10, 10),
		sill = 1)
	
	write_property(prop2, "results/RESULT_OK_BIG.INC", "BIG_OK", -99)
	del(prop2)

if (not "nosk" in argv):

	prop3 = simple_kriging(prop_cont, grid,
		radiuses = (20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10, 10, 10),
		sill = 1,
		mean = 1.6)
	
	write_property(prop3, "results/RESULT_SK_BIG.INC", "BIG_SK", -99)
	del(prop3)


if (not "nolvm" in argv):
	mean_data = load_mean_data("test_data/mean_0.487_166_141_20.inc")	
	prop_lvm = lvm_kriging(prop_cont, grid, mean_data,
			radiuses = (20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10, 10, 10),
		sill = 1)
	
	write_property(prop_lvm, "results/medium/lvm.inc", "lvm_medium", -99)
	del(mean_data)
	del(prop_lvm)

if (not "nosgs" in argv):
	
	sgs_params = {
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"covariance_type": covariance.exponential,
		"ranges": (10, 10, 10),
		"sill": 0.4,
		"kriging_type": "sk"}
		
	mean_data = load_mean_data("test_data/mean_0.487_166_141_20.inc")
	lvm_sgs_params = {
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"covariance_type": covariance.exponential,
		"ranges": (10, 10, 10),
		"sill": 0.4,
		"mean_data": mean_data}
		
	sgs_result1 = sgs_simulation(prop_cont, grid, seed=3439275, **sgs_params)
	write_property(sgs_result1, "results/medium/SGS1.INC", "SGS1", -99)

	sgs_result2 = sgs_simulation(prop_cont, grid, seed=24193421, **sgs_params)
	write_property(sgs_result2, "results/medium/SGS2.INC", "SGS2", -99)

	sgs_lvm = sgs_simulation(prop_cont, grid, seed=3439275, **lvm_sgs_params)
	write_property(sgs_lvm, "results/medium/SGS_LVM.INC", "SGS_LVM", -99)

	write_property(sgs_result2, "results/RESULt_SGS2_BIG.INC", "BIG_SGS2", -99)
	del(sgs_result1)
	del(sgs_result2)
	del(sgs_lvm)

ik_prop = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99, [0,1])
print 'Property data loaded.'


ik_data =  [	{
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

if (not "noik" in argv):
	t1 = time.time()
	ik_result = indicator_kriging(ik_prop, grid, ik_data)
	t2 = time.time()
	print "Time: %s s." % (t2 - t1)
	write_property(ik_result, "results/RESULT_IK_BIG_SOFT.INC", "BIG_IK", -99)

	t1 = time.time()
	ik_result = median_ik(ik_prop, grid, values=(0,1), marginal_probs = (0.8, 0.2), radiuses = (20, 20, 20), max_neighbours = 12, covariance_type=1, ranges=(10, 10, 10), sill = 1)
	t2 = time.time()
	print "Time: %s s." % (t2 - t1)
	write_property(ik_result, "results/RESULT_MIK_BIG_SOFT.INC", "BIG_MIK", -99)

if (not "nosis" in argv):
	sis_result = sis_simulation(ik_prop, grid, ik_data, seed=3241347)
	write_property(sis_result, "results/RESULT_SIS_BIG_SOFT.INC", "SIS_RESULT_BIG_SOFT", -99)

stdin.readline()
