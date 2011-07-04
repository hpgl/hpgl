#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from sys import *
from geo import *
from razn import *
import os
import time
from pylab import *


x = 166
y = 141
z = 20
size_prop = [166, 141, 20]

if not os.path.exists("current/"):
        os.mkdir("current/")
if not os.path.exists("current/images"):
        os.mkdir("current/images")
		
print "loading properties..."
prop = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99, [0,1], size_prop)
ik_prop = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99,[0,1], size_prop)
mask_prop = load_ind_property("test_data/MASK_PROPERTY.INC", -99,[0,1], size_prop)[0]
prop_cont = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC", -99, size_prop)
prop_cdf = load_cont_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99, size_prop)
sec_data = load_cont_property("test_data/BIG_SOFT_DATA_CON_CoK.INC", -99, size_prop);
mean_prop = load_cont_property("test_data/mean_0.487_166_141_20.inc", -99, size_prop)[0];
print "done"

print "GSLIB support test..."
print "Reading..."
gslib_prop = load_gslib_file("test_data/samples.gslib")
print gslib_prop

print "Writing..."
write_gslib_file(gslib_prop, "results/samples_writed.gslib", "test property")
writed_gslib_prop = load_gslib_file("results/samples_writed.gslib")
print writed_gslib_prop
extracted_gslib_prop = get_gslib_property(gslib_prop, "Variable", 10)
print extracted_gslib_prop

was_errors = False

grid = SugarboxGrid(166, 141, 20)

lvm_mean_data = load_cont_property("test_data/mean_0.487_166_141_20.inc", -99, size_prop)[0] 

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
variogram_nugget = {"covariance_type": covariance.exponential, "ranges": (10, 10, 10), "sill": 1, "nugget": 0.5}
variogram2 = {"covariance_type": covariance.exponential, "ranges": (10, 10, 10), "sill": 0.4}
variogram3 = {"covariance_type": covariance.gaussian, "ranges": (10, 10, 10), "sill": 1}
variogram4 = {"covariance_type": covariance.spherical, "ranges": (10, 10, 10), "sill": 1}
neighbourhood1 = {"radiuses": (20, 20, 20), "max_neighbours":12}
sck_primary = { 'data': prop_cont, 'mean': calc_mean(prop_cont), 'cov_type': covariance.exponential, 'ranges': (10, 10, 10), 'sill': 1, 'nugget': 0, 'angles': None }
sck_secondary = { 'data': sec_data, 'mean': calc_mean(sec_data), 'cov_type': covariance.exponential, 'ranges': (10, 10, 10), 'sill': 1, 'angles': None }

test_data = [
		("OK", 
			ordinary_kriging, [variogram1, neighbourhood1, {"prop":prop_cont, "grid": grid }],
			"current/RESULT_OK_BIG.INC",
			"BIG_OK",
			"fixed/RESULT_OK_BIG.INC"), 
		("SK",
			simple_kriging, [variogram1, neighbourhood1, { "prop": prop_cont, "grid": grid, "mean": 1.6}],
			"current/RESULT_SK_BIG.INC", "BIG_SK",
			"fixed/RESULT_SK_BIG.INC"),
		("SK_nugget",
			simple_kriging, [variogram_nugget, neighbourhood1, { "prop": prop_cont, "grid": grid, "mean": 1.6}],
			"current/RESULT_SK_BIG_NUGGET.INC", "BIG_SK_NUGGET",
			"fixed/RESULT_SK_BIG_NUGGET.INC"),
		("SK_var_gaussian",
			simple_kriging, [variogram1, neighbourhood1, { "prop": prop_cont, "grid": grid, "mean": 1.6}],
			"current/RESULT_SK_BIG_VAR_GAUSS.INC", "BIG_SK_VAR_GAUSS",
			"fixed/RESULT_SK_BIG_VAR_GAUSS.INC"),
		("SK_var_spherical",
			simple_kriging, [variogram1, neighbourhood1, { "prop": prop_cont, "grid": grid, "mean": 1.6}],
			"current/RESULT_SK_BIG_VAR_SPHERICAL.INC", "BIG_SK_VAR_SPHERICAL",
			"fixed/RESULT_SK_BIG_VAR_SPHERICAL.INC"),
		("LVM",
			lvm_kriging, [variogram1, neighbourhood1, {"prop": prop_cont, "grid": grid, "mean_data": lvm_mean_data}],
			 "current/lvm.inc", "LVM_MEDIUM",
			  "fixed/lvm.inc"),
		("SGS1",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "kriging_type": "sk"}],
			 "current/SGS1.INC", "SGS1",
			 "fixed/SGS1.INC"),
		("SGS1_masked",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "kriging_type": "sk", "mask": mask_prop}],
			 "current/SGS1_MASKED.INC", "SGS1_MASKED",
			 "fixed/SGS1_MASKED.INC"),
		("SGS_CDF_EQUAL",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "kriging_type": "sk", "cdf_data": prop_cont}],
			 "current/SGS1.INC", "SGS1",
			 "fixed/SGS1.INC"),
 		("SGS_CDF_NOT_EQUAL",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "kriging_type": "sk", "cdf_data": prop_cdf}],
			 "current/SGS1.INC", "SGS1",
			 "fixed/SGS_CDF_NOT_EQUAL.INC"),
		("SGS2",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 24193421, "kriging_type": "sk"}],
			 "current/RESULT_SGS2_BIG.INC", "BIG_SGS2",
			 "fixed/RESULT_SGS2_BIG.INC"),
		("SGS_LVM", sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "mean_data": lvm_mean_data}],
			"current/SGS_LVM.INC", "SGS_LVM", 
			"fixed/SGS_LVM.INC"),
		("IK", indicator_kriging, [{"prop": ik_prop, "grid": grid, "data": ik_data}],
			"current/RESULT_IK_BIG_SOFT.INC", "BIG_IK",
			"fixed/RESULT_IK_BIG_SOFT.INC"),
		("MIK", median_ik, [neighbourhood1, variogram1, {"prop": ik_prop, "grid": grid, "marginal_probs": (0.8, 0.2) }],
			"current/RESULT_MIK_BIG_SOFT.INC", "BIG_MIK",
			"fixed/RESULT_IK_BIG_SOFT.INC"),
		("SIS", sis_simulation, [{"prop": ik_prop, "grid": grid, "data": ik_data, "seed": 3241347}],
			"current//RESULT_SIS_BIG_SOFT.INC", "SIS_RESULT_BIG_SOFT",
			"fixed/RESULT_SIS_BIG_SOFT.INC"),
		("SIS_MASKED", sis_simulation, [{"prop": ik_prop, "grid": grid, "data": ik_data, "seed": 3241347, "mask": mask_prop}],
			"current//RESULT_SIS_BIG_SOFT_MASKED.INC", "SIS_RESULT_BIG_SOFT_MASKED",
			"fixed/RESULT_SIS_BIG_SOFT_MASKED.INC"),
		("Simple_CoK_M1", simple_cokriging_markI, [neighbourhood1, variogram1, { "prop": prop_cont, "grid": grid, "secondary_data": sec_data, "primary_mean": calc_mean(prop_cont),	"secondary_mean": calc_mean(sec_data), "secondary_variance": 5.9 , "correlation_coef": 0.97}],
			"current//RESULT_SCK_MARK_1.INC", "SCK_MARK_1",
			"fixed/RESULT_SCK_MARK_1.INC"),
		("Simple_CoK_M2", simple_cokriging_markII, [neighbourhood1, { "primary_data": sck_primary, "secondary_data": sck_secondary, "grid": grid, "correlation_coef": 0.97}],
			"current//RESULT_SCK_MARK_2.INC", "SCK_MARK_2",
			"fixed/RESULT_SCK_MARK_2.INC"),
		("SIS_LVM", sis_simulation, [{"prop": ik_prop, "grid": grid, "data": ik_data, "seed": 3241347, "mean_data": [1 - mean_prop, mean_prop]}],
			"current//RESULT_SIS_BIG_SOFT_LVM.INC", "SIS_RESULT_BIG_SOFT_LVM",
			"fixed/RESULT_SIS_BIG_SOFT_LVM.INC"),
]

def test_algo(algo_name, algo, params, current_file_path, current_prop_name, fixed_file_path):
	print "=================================="
	print "Testing %s..." % (algo_name)
	p = {}
	for pr in params:
		p.update(pr)
	time1 = time.time()
	result_current = algo(**p)
	time2 = time.time()
	
	# uncomment to make new fixed properties
	#print "Writing property..."
	#write_property(result_current, fixed_file_path, algo_name, -99)
	#print "Done"
	
	print "Comparing %s..." % (algo_name)
	result_fixed = load_cont_property(fixed_file_path, -99, size_prop)
	
	# saving image
	print "Saving image..."
	imshow(result_current[0][:,:,0], vmin = 0, vmax = 5)
	savefig("current/images/" + algo_name)
	imshow(result_fixed[0][:,:,0], vmin = 0, vmax = 5)
	savefig("current/images/" + algo_name + "_fixed")
	print "Done."
	
	diff_res = result_fixed[0] - result_current[0];
	
	if all(diff_res < 0.01):
	#if all(diff_res.mean() < 0.01):
		result = True
		diff = 0
		print "Ok"
	else:
		result = False
		print "Error"
		_, _, diff, _ = calc_razn(x,y,z,result_fixed[0], result_current[0])
		was_errors = True
	print "Time of %s in Python: %s" % (algo_name, time2 - time1)
	print "\n\n"
	err_data = (algo_name, diff, time2 - time1)
	return result, err_data

def print_summary(errors, error_data, test_count):
	if errors == 0:
		print "All %s tests succeed." % (test_count)
	else:
		for err_data in error_data:
			print "%s:\t mean diff = %s,\t time = %s" % err_data
		print "%s of %s tests failed." % (errors, test_count)


def test_all():
	errors = 0
	error_data = []
	for data in test_data:
		success, err_data = test_algo(*data)
		if not success:
			errors += 1
			error_data.append(err_data)
	print_summary(errors, error_data, len(test_data))

def test_only(tests):
	errors = 0
	error_data = []
	for data in test_data:
		if (data[0] in tests):
			success, err_data = test_algo(*data)
			if not success:
				errors += 1
				error_data.append(err_data)
	print_summary(errors, error_data, len(tests))

test_all()
#test_only(["OK"])
#stdin.readline()
