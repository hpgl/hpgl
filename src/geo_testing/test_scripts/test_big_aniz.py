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

prop = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC", -99)

grid = SugarboxGrid(166, 141, 20)


prop3 = simple_kriging(prop, grid,
radiuses = (100, 100, 100),
max_neighbours = 12,
covariance_type = covariance.exponential,
ranges = (20, 10, 5),
sill = 1,
mean = 1.7,
angles = (40, 50, 90))

write_property(prop3, "results/RESULT_SK_ANIZ_BIG.INC", "BIG_SK", -99)
del(prop3)
