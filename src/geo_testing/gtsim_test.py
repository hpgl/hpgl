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
from gtsim import *
import time

# gtsim test
time1 = time.time()
prop = load_cont_property("test_data/BIG_HARD_DATA.INC", -99)
grid = SugarboxGrid(166, 141, 225)
sk_params = { "radiuses" : (20, 20, 20),
                 "max_neighbours" : 12,
                 "covariance_type" : 1,
                 "ranges" : (10, 10, 10),
                 "sill" : 1,
                 "mean" : 1.6}
sgs_params = { "radiuses" : (20, 20, 20),
                 "max_neighbours" : 12,
                 "covariance_type" : 1,
                 "ranges" : (10, 10, 10),
                 "sill" : 1,
                 "kriging_type": "sk",
                 "mean" : 1.6}
                    
gtsim_2ind(grid, prop, sk_params, sgs_params)
time2 = time.time()

print "Time " (time2 - time1)
stdin.readline()
