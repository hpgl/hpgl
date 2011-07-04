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
from sys import *
from geo import *
from razn import *
from python_property import *
import os
import time

x=166
y=141
z=20

time_OK = 159.887
time_SK = 170.430
time_IK = 179.585
time_LVM = 158.275
time_SGS = 229.388
time_LVM_SGS = 227.352
time_SIS = 224.521

if not os.path.exists("current/"):
        os.mkdir("current/")

print "loading double property..."
prop = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99, [0,1])
ik_prop = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99,[0,1])
prop_cont = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC", -99)
prop_cdf = load_cont_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99)
print "done"

sec_data = load_cont_property("test_data/BIG_SOFT_DATA_CON_CoK.INC", -99);

was_errors = False

grid = SugarboxGrid(166, 141, 20)

lvm_mean_data = load_mean_data("test_data/mean_0.487_166_141_20.inc")  

primary_data = {'data': prop_cont, 'mean': calc_mean(prop_cont), 'cov_type': covariance.exponential, 'ranges': (10, 10, 10), 'sill': 1, 'nugget': 0, 'angles': None}
secondary_data = {'data': sec_data, 'mean': calc_mean(sec_data), 'cov_type': covariance.exponential, 'ranges': (10, 10, 10), 'sill': 1, 'nugget': 0, 'angles': None}
ik_data =  [    {
                        "cov_type": 1, 
                        "ranges": (10, 10, 10),
                        "sill": 1,
                        "radiuses": (20, 20, 20),
                        "max_neighbours": 12,
                        "marginal_prob": 0.8,
                        "value": 0
                },
                {
                        "cov_type": 1, 
                        "ranges": (10, 10, 10),
                        "sill": 1,
                        "radiuses": (20, 20, 20),
                        "max_neighbours": 12,
                        "marginal_prob": 0.2,
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

variogram1 = {"covariance_type": covariance.exponential, "ranges": (10, 10, 10), "sill": 1}
variogram2 = {"covariance_type": covariance.exponential, "ranges": (10, 10, 10), "sill": 0.4}
neighbourhood1 = {"radiuses": (20, 20, 20), "max_neighbours":12}

test_data = [
		("OK", 
			ordinary_kriging, [variogram1, neighbourhood1, {"prop":prop_cont, "grid": grid }],
			"current/RESULT_OK_BIG.INC",
			"BIG_OK",
			"fixed/RESULT_OK_BIG.INC",
			time_SK), 
		("SK",
			simple_kriging, [variogram1, neighbourhood1, { "prop": prop_cont, "grid": grid, "mean": 1.6}],
			"current/RESULT_SK_BIG.INC", "BIG_SK",
			"fixed/RESULT_SK_BIG.INC",
			time_OK),
		("LVM",
			lvm_kriging, [variogram1, neighbourhood1, {"prop": prop_cont, "grid": grid, "mean_data": lvm_mean_data}],
			 "current/lvm.inc", "LVM_MEDIUM",
			  "fixed/lvm.inc",
			  time_LVM),
		("SGS1",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "kriging_type": "sk"}],
			 "current/SGS1.INC", "SGS1",
			 "fixed/SGS1.INC",
			 time_SGS),
		("PSGS1",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "kriging_type": "sk", "workers_count": 2}],
			 "current/PSGS1.INC", "SGS1",
			 "fixed/SGS1.INC",
			 time_SGS),
		("SGS_CDF_EQUAL",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "kriging_type": "sk", "cdf_data": prop_cont}],
			 "current/SGS1.INC", "SGS1",
			 "fixed/SGS1.INC",
			 time_SGS),
 		("SGS_CDF_NOT_EQUAL",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "kriging_type": "sk", "cdf_data": prop_cdf}],
			 "current/SGS1.INC", "SGS1",
			 "fixed/SGS_CDF_NOT_EQUAL.INC",
			 time_SGS),
		("SGS2",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 24193421, "kriging_type": "sk"}],
			 "current/RESULT_SGS2_BIG.INC", "BIG_SGS2",
			 "fixed/RESULT_SGS2_BIG.INC",
			 time_SGS),
		("SGS_LVM", sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "mean_data": lvm_mean_data}],
			"current/SGS_LVM.INC", "SGS_LVM", 
			"fixed/SGS_LVM.INC", time_LVM_SGS),
		("IK", indicator_kriging, [{"prop": ik_prop, "grid": grid, "data": ik_data}],
			"current/RESULT_IK_BIG_SOFT.INC", "BIG_IK",
			"fixed/RESULT_IK_BIG_SOFT.INC", time_IK),
		("MIK", median_ik, [neighbourhood1, variogram1, {"prop": ik_prop, "grid": grid, "marginal_probs": (0.8, 0.2) }],
			"current/RESULT_MIK_BIG_SOFT.INC", "BIG_MIK",
			"fixed/RESULT_IK_BIG_SOFT.INC", None),
		("SIS", sis_simulation, [{"prop": ik_prop, "grid": grid, "data": ik_data, "seed": 3241347}],
			"current//RESULT_SIS_BIG_SOFT.INC", "SIS_RESULT_BIG_SOFT",
			"fixed/RESULT_SIS_BIG_SOFT.INC", time_SIS),
		("PSIS", sis_simulation, [{"prop": ik_prop, "grid": grid, "data": ik_data, "seed": 3241347, "workers_count": 2}],
			"current//RESULT_PSIS_BIG_SOFT.INC", "PSIS_RESULT_BIG_SOFT",
			"fixed/RESULT_SIS_BIG_SOFT.INC", time_SIS),
		("SCK_MI", simple_cokriging_markI,[variogram1, neighbourhood1, {"prop":prop_cont, "grid":grid, "secondary_data": sec_data, "primary_mean": calc_mean(prop_cont),
			"secondary_mean": calc_mean(sec_data), "secondary_variance": 0.07, "correlation_coef": 0.25}],
			"current//RESULT_SCK_MI.INC", "SCK_MI",
			"fixed/RESULT_SCK_MI.INC", None),
		("SCK_MII", simple_cokriging_markII,[neighbourhood1, {"grid": grid, "primary_data": primary_data, "secondary_data": secondary_data, "correlation_coef": 0.25}],
			"current//RESULT_SCK_MII.INC", "SCK_MII",
			"fixed/RESULT_SCK_MII.INC", None),
		]

def test_algo(algo_name, algo, params, current_file_path, current_prop_name, fixed_file_path, sgems_time):
	print "=================================="
	print "Testing %s..." % (algo_name)
	p = {}
	for pr in params:
		p.update(pr)
	time1 = time.time()
	prop = algo(**p)
	time2 = time.time()
	write_property(prop, current_file_path, current_prop_name, -99)
	del (prop)
	print "Comparing %s..." % (algo_name)
	result_fixed = load_property_python(x,y,z, fixed_file_path, True)
	result_current = load_property_python(x,y,z, current_file_path, True)
	if all(result_fixed == result_current):
		result = True
		print "Ok"
	else:
		result = False
		print "Error"
		_, _, diff, _ = calc_razn(x,y,z,result_fixed, result_current)
		was_errors = True
	print "Time of %s in SGeMS: %s and in Python: %s" % (algo_name, sgems_time, time2 - time1)
	if not sgems_time is None:
		print "Time supremacy: %s." % (sgems_time / (time2 - time1) )
	print "\n\n"
	err_data = (algo_name, diff)
	return result, err_data

def test_all():
	errors = 0
	error_data = []
		for data in test_data:
			success, err_data = test_algo(*data)
			if not success:
				errors += 1
				error_data.append(err_data)
	if errors == 0:
		print "All %s tests succeed." % (len(test_data))
	else:
		for err_data in error_data:
			print "%s: mean diff = %s" % err_data
		print "%s of %s tests failed." % (errors, len(test_data))
	

def test_sis_only():
	errors = 0
	for data in test_data:
		if (data[0] == "SIS"):
			if not test_algo(*data):
				errors += 1

def test_psis_only():
	errors = 0
	for data in test_data:
		if (data[0] == "PSIS"):
			if not test_algo(*data):
				errors += 1
def test_ik_only():
	errors = 0
	for data in test_data:
		if (data[0] == "IK"):
			if not test_algo(*data):
				errors += 1
def test_psgs_only():
	errors = 0
	for data in test_data:
		if (data[0] in ["PSGS1"]):
			if not test_algo(*data):
				errors += 1
def test_sck_mi_only():
	errors = 0
	for data in test_data:
		if (data[0] == "SCK_MI"):
			if not test_algo(*data):
				errors += 1
def test_sck_mii_only():
	errors = 0
	for data in test_data:
		if (data[0] == "SCK_MII"):
			if not test_algo(*data):
				errors += 1
#test_all()
#test_sis_only()
#test_psis_only()
#test_ik_only()
#test_psgs_only()
#test_sck_mi_only()
test_sck_mii_only()
stdin.readline()

