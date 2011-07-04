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

prop_con = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC", -99)

grid = SugarboxGrid(166, 141, 20)

sgs_params = {
	"radiuses": (20, 20, 20),
	"max_neighbours": 12,
	"covariance_type": covariance.exponential,
	"ranges": (10, 10, 10),
	"sill": 0.4,
	"kriging_type": "sk"}

sgs_result1 = sgs_simulation(prop_con, grid, seed=3439275, workers_count=2, use_new_psgs = True, region_size = (20,20,20), **sgs_params)

		
