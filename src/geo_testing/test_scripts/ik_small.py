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

grid = SugarboxGrid(55, 52, 1)
ik_prop = load_ind_property("test_data/NEW_TEST_PROP_01.INC", -99, [0,1])

ik_data =  [	{
			"cov_type": 0, 
			"ranges": (10, 10, 10),
			'sill': 0.4,
			"radiuses": (10, 10, 10),
			"max_neighbours": 12,
			"marginal_prob": 0.5,
			"value": 0
		},
		{
			"cov_type": 0, 
			"ranges": (10, 10, 10),
			"sill": 0.4,
			"radiuses": (10, 10, 10),
			"max_neighbours": 12,
			"marginal_prob": 0.5,
			"value": 1
		}]

ik_result = indicator_kriging(ik_prop, grid, ik_data)
write_property(ik_result, "results/small/RESULT_IK.INC", "IK", -99)		
