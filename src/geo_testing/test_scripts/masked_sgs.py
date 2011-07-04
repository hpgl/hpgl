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
write_property(prop, "results/MIRROR_INPUT.INC", "INPUT_PROP", -99);

grid = SugarboxGrid(55, 52, 1)

mask = hpgl.byte_property_array(prop.size(), [0,1])

for i in xrange(prop.size()):
	mask.set_at(i, i%2)

sgs_params = {
	"radiuses": (20, 20, 20),
	"max_neighbours": 12,
	"covariance_type": covariance.exponential,
	"ranges": (10, 10, 10),
	"sill": 0.4}

sgs_result1 = sgs_simulation(prop, grid, seed=3439275, mask=mask, **sgs_params)
write_property(sgs_result1, "results/small/masked_sgs", "masked_SGS1", -99)



