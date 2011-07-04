#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from gtsimk_const_prob import *
from sys import *
import os

# gtsim for k indicators test

prop = load_cont_property("test_data/BIG_SOFT_DATA_166_141_20.INC", -99)
grid = SugarboxGrid(166, 141, 20)

sk_params = { "radiuses" : (20, 20, 20),
                 "max_neighbours" : 12,
                 "covariance_type" : 1,
                 "ranges" : (10, 10, 10),
                 "sill" : 1}
                   
indicator = 3
gtsim_Kind_const_prop(grid, prop, indicator, sk_params)
stdin.readline()
