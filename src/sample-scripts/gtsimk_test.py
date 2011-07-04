#
#
#   Copyright 2009 HPGL Team
#
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.
#
#   HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.
#
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
