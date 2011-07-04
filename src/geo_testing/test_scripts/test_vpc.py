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

if not os.path.exists("results"):
	os.mkdir("results")

if not os.path.exists("results/small"):
	os.mkdir("results/small")
	
if not os.path.exists("results/medium"):
	os.mkdir("results/medium")

def calc_small_vpc():
	grid = SugarboxGrid(55, 52, 1)
	ik_prop = load_ind_property("test_data/NEW_TEST_PROP_01.INC", -99, [0,1])

	means = calc_vpc(ik_prop, grid, [0.8, 0.2])

	write_mean_data(means[0], "results/small/prob_0.inc", "PROB_0");
	write_mean_data(means[1], "results/small/prob_1.inc", "PROB_1");

def calc_medium_vpc():
	grid2 = SugarboxGrid(166, 141, 20)
	ik_prop2 = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99, [0,1])

	means2 = calc_vpc(ik_prop2, grid2, [0.8, 0.2])
	write_mean_data(means2[0], "results/medium/prob_0.inc", "PROB_0");
	write_mean_data(means2[0], "results/medium/prob_1.inc", "PROB_1");

calc_small_vpc()
calc_medium_vpc()
