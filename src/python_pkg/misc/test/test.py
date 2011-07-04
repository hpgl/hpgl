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

enabled_algos = ['ok', 'sk', 'lvm', 'sgs', 'mik' , 'ik', 'sis']
#enabled_algos = ['sis']

if 'ok' in enabled_algos:
	prop2 = ordinary_kriging(prop, grid,
		radiuses = (20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10, 10, 10),
		sill = 1)

	write_property(prop2, "results/RESULT_OK.INC", "OK", -99)
	del(prop2)

if 'sk' in enabled_algos:
	prop3 = simple_kriging(prop, grid,
		radiuses = (20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10, 10, 10),
		sill = 1,
		mean = 0.487)

	write_property(prop3, "results/RESULT_SK.INC", "SK", -99)

mean_data = load_mean_data("test_data/mp_0_8_52_55_1.inc");

if 'lvm' in enabled_algos:
	prop_lvm = lvm_kriging(prop, grid, mean_data,
		radiuses = (20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10, 10, 10),
		sill = 1)

	write_property(prop_lvm, "results/small/lvm.inc", "LVM_SMALL", -99);

sgs_params = {
	"radiuses": (20, 20, 20),
	"max_neighbours": 12,
	"covariance_type": covariance.exponential,
	"ranges": (10, 10, 10),
	"sill": 0.4}

if ('sgs' in enabled_algos):
	sgs_result1 = sgs_simulation(prop, grid, seed=3439275, **sgs_params)
	write_property(sgs_result1, "results/RESULT_SGS1.INC", "SGS1", -99)

	sgs_result2 = sgs_simulation(prop, grid, seed=24193421, **sgs_params)
	write_property(sgs_result2, "results/RESULt_SGS2.INC", "SGS2", -99)

	sgs_lvm_result = sgs_simulation(prop, grid, mean_data=mean_data, seed=3439275, **sgs_params)
	write_property(sgs_lvm_result, "results/small/sgs_lvm.inc", "SGS_LVM", -99)

ik_prop = load_ind_property("test_data/NEW_TEST_PROP_01.INC", -99, [0,1])
write_property(ik_prop, "results/MIRROR_IK_INPUT.INC", "IK_INPUT", -99);

if ('mik' in enabled_algos):
	mik_prop = median_ik(ik_prop, grid, 
			values=(0,1), 
			marginal_probs=(0.5, 0.5), 
			covariance_type = covariance.spherical,
			ranges=(10, 10, 10),
			sill=0.4,
			radiuses=(10, 10, 10),
			max_neighbours=12)

	write_property(mik_prop, "results/small/MIK.INC", "MIK", -99);


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
		
multi_ik_data = [
	{
		"cov_type": 0,
		"ranges": (4, 4, 4),
		"sill": 0.25,
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"marginal_prob": 0.24,
		"value": 0
	},
	{
		"cov_type": 0,
		"ranges": (6, 6, 6),
		"sill": 0.25,
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"marginal_prob": 0.235,
		"value": 1
	},
	{
		"cov_type": 0,
		"ranges": (2, 2, 2),
		"sill": 0.25,
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"marginal_prob": 0.34,
		"value": 2
	},
	{
		"cov_type": 0,
		"ranges": (10, 10, 10),
		"sill": 0.25,
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"marginal_prob": 0.18,
		"value": 3
	}]		
if 'ik' in enabled_algos:
	ik_result = indicator_kriging(ik_prop, grid, ik_data)
	write_property(ik_result, "results/RESULT_IK.INC", "IK", -99)

if 'sis' in enabled_algos:
	sis_result = sis_simulation(ik_prop, grid, ik_data, seed=3241347)
	write_property(sis_result, "results/RESULT_SIS.INC", "SIS_RESULT", -99)

	psis_result = parallel_sis_simulation(ik_prop, grid, ik_data, seed=3241347)
	write_property(psis_result, "results/small/psis.inc", "psis_result", -99)

stdin.readline()
