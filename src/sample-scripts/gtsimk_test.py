from gtsimk import *
from sys import *
import os

# gtsim for k indicators test

prop = load_cont_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99)
grid = SugarboxGrid(166, 141, 20)

sk_params = { "radiuses" : (20, 20, 20),
                 "max_neighbours" : 12,
                 "covariance_type" : 1,
                 "ranges" : (10, 10, 10),
                 "sill" : 1,
                 "mean" : 1.6}
                   
indicator = 3
gtsim_Kind(grid, prop, indicator, sk_params)
stdin.readline()
