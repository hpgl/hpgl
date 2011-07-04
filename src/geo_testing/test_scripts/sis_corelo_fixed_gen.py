#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from sys import *
from geo import *

import os

if not os.path.exists("results/"):
	os.mkdir("results/")
if not os.path.exists("results/medium/"):
	os.mkdir("results/medium/")

grid = SugarboxGrid(166, 141, 20)
prop =  load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99,[0,1])
mean_data = load_mean_data("test_data/mean_0.487_166_141_20.inc")

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

out_prop = sis_simulation(prop, grid, ik_data, seed=3241347, mean_data=mean_data)

write_property(out_prop, "results/medium/SIS_CORELO_FIXED.INC", "SIS_CORELO_FIXED", -99)
