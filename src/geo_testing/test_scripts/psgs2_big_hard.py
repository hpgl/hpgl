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
import time

if not os.path.exists("results/"):
	os.mkdir("results/")
if not os.path.exists("results/big/"):
	os.mkdir("results/big/")


grid = SugarboxGrid(166, 141, 225)
prop = load_cont_property("test_data/BIG_HARD_DATA.INC", -99)

sgs_params = {
	"prop": prop,
	"grid": grid,
	"seed": 3439275,
	"kriging_type": "sk",
	"radiuses": (20, 20, 20),
	"max_neighbours": 12,
	"covariance_type": covariance.exponential,
	"ranges": (10, 10, 10),
	"sill": 0.4
}

for x in xrange(8):
	time1 = time.time()
	psgs_result = sgs_simulation(workers_count = x+1, use_new_psgs=True, **sgs_params)
	time2 = time.time()
	print "Workers: %s" % (x+1)
	print "Time: %s" % (time2 - time1)
	if x == 1:
		write_property(psgs_result, "results/big/psgs2.inc", "psgs2_big", -99)



