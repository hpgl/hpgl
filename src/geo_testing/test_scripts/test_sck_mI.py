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

prop_cont = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC", -99)
print "done"

sec_data = load_cont_property("test_data/BIG_SOFT_DATA_CON_CoK.INC", -99);

grid = SugarboxGrid(166, 141, 20)

prop2 = simple_cokriging_markI(
		prop_cont, 
		grid = grid, 
		secondary_data = sec_data, 
		primary_mean = calc_mean(prop_cont),
		secondary_mean = calc_mean(sec_data),
		secondary_variance = 5.9 ,
		correlation_coef = 0.97,
		radiuses=(20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10,10,10),
		sill = 1.0)

write_property(prop2, "results/medium/sck_mI.inc", "sck_mI", -99)


