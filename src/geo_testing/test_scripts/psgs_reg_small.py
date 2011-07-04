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
from geo import *
from sys import *
import os

if not os.path.exists("results/"):
	os.mkdir("results/")
if not os.path.exists("results/small/"):
	os.mkdir("results/small")


print "Loading continuous property..."
prop = load_cont_property("test_data/NEW_TEST_PROP.INC", -99)
print "Done"
grid = SugarboxGrid(55, 52, 1)

sgs_params = {
	"radiuses": (10, 10, 10),
	"max_neighbours": 12,
	"covariance_type": covariance.exponential,
	"ranges": (10, 10, 10),
	"sill": 0.4}

result=sgs_simulation(prop, grid, seed=3439275, use_new_psgs=True, workers_count=2, ** sgs_params)

write_property(result, "results/small/psgs_reg.inc", "psgs_reg_small", -99)
