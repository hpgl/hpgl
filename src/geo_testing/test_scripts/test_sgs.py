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
if not os.path.exists("results/small/psgs/"):
	os.mkdir("results/small/psgs/")	
if not os.path.exists("results/medium/"):
	os.mkdir("results/medium")
if not os.path.exists("results/medium/psgs/"):
	os.mkdir("results/medium/psgs/")	
	
if not os.path.exists("results/big/"):
	os.mkdir("results/medium")
if not os.path.exists("results/big/psgs/"):
	os.mkdir("results/big/psgs/")
	
def test_small():
	prop = load_cont_property("test_data/NEW_TEST_PROP.INC", -99)
	grid = SugarboxGrid(55, 52, 1)
	sgs_params = {
	"radiuses": (20, 20, 20),
	"max_neighbours": 12,
	"covariance_type": covariance.exponential,
	"ranges": (10, 10, 10),
	"sill": 0.4}
	
	for i in xrange(5):
		sgs_result1 = sgs_simulation(prop, grid, seed=3439275, workers_count=i+1, **sgs_params)
		write_property(sgs_result1, "results/small/psgs/sgs_%s.inc" % (i+1), "SGS_%s" % (i+1), -99)

def test_medium():
	prop_con = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC", -99)
	grid = SugarboxGrid(166, 141, 20)
	sgs_params = {
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"covariance_type": covariance.exponential,
		"ranges": (10, 10, 10),
		"sill": 0.4,
		"kriging_type": "sk"}
	for i in xrange(5):
		sgs_result1 = sgs_simulation(prop_con, grid, seed=3439275, workers_count=i+1, **sgs_params)
		write_property(sgs_result1, "results/medium/psgs/sgs_%s.inc" % (i+1), "SGS_%s" % (i+1), -99)

test_small();
test_medium();

		