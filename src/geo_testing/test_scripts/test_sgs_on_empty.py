#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from geo import *
from sys import *

grid = SugarboxGrid(166, 141, 20)

prop = load_cont_property("test_data/BIG_N_EMPTY.INC", -99)

sgs_result = sgs_simulation(prop, grid,
		radiuses = (20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10, 10, 10),
		sill = 0.4,
		seed = 3439275)

stdin.readline()
