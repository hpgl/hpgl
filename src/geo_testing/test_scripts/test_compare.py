#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from pylab import  *
from geo import *
from geo.routines import *
from razn import *
import os
import time


x = 166
y = 141
z = 20
size_prop = [166, 141, 20]
multi_indicators = [0,1,2,3]

if not os.path.exists("current/"):
        os.mkdir("current/")
if not os.path.exists("current/images"):
        os.mkdir("current/images")
		
print "loading properties..."
prop = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99, [0,1], size_prop)
ik_prop = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99,[0,1], size_prop)
multi_ik_prop = load_ind_property("test_data/MULTI_IND.INC", -99,[0,1,2,3], size_prop)
mask_prop = load_ind_property("test_data/MASK_PROPERTY.INC", -99,[0,1], size_prop)[0]
prop_cont = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC", -99, size_prop)
prop_cdf = load_cont_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99, size_prop)
sec_data = load_cont_property("test_data/BIG_SOFT_DATA_CON_CoK.INC", -99, size_prop);

mean_prop = load_cont_property("test_data/mean_0.487_166_141_20.inc", -99, size_prop)[0];

MarginalProbs = CalcMarginalProbsIndicator(multi_ik_prop[0], multi_ik_prop[1], multi_indicators)
VPCs = CalcVPCsIndicator(multi_ik_prop[0], multi_ik_prop[1], multi_indicators, MarginalProbs)
mean_props_multi_ik = CubesFromVPCs(VPCs, size_prop[0], size_prop[1])

print "done"

#print "GSLIB support test..."
#print "Reading..."
#gslib_prop = load_gslib_file("test_data/samples.gslib")
#print gslib_prop

#print "Writing..."
#write_gslib_file(gslib_prop, "results/samples_writed.gslib", "test property")
#writed_gslib_prop = load_gslib_file("results/samples_writed.gslib")
#print writed_gslib_prop
#extracted_gslib_prop = get_gslib_property(gslib_prop, "Variable", 10)
#print extracted_gslib_prop

was_errors = False

grid = SugarboxGrid(166, 141, 20)

lvm_mean_data = load_cont_property("test_data/mean_0.487_166_141_20.inc", -99, size_prop)[0]

cov1 = CovarianceModel(type=1, ranges=(10,10,10), sill=1) 
cov2 = CovarianceModel(type=0, ranges=(4,4,4), sill=0.25)
cov3 = CovarianceModel(type=0, ranges=(6,6,6), sill=0.25) 
cov4 = CovarianceModel(type=0, ranges=(2,2,2), sill=0.25)
cov5 = CovarianceModel(type=0, ranges=(10,10,10), sill=0.25)

ik_data =  [    {
			"cov_model": cov1,
                        "radiuses": (20, 20, 20),
                        "max_neighbours": 12,
                },
                {
			"cov_model": cov1,
                        "radiuses": (20, 20, 20),
                        "max_neighbours": 12,
                }]
                
multi_ik_data = [
        {
		"cov_model": cov2,
                "radiuses": (20, 20, 20),
                "max_neighbours": 12,
        },
        {
		"cov_model": cov3,
                "radiuses": (20, 20, 20),
                "max_neighbours": 12,
        },
        {
		"cov_model": cov4,
                "radiuses": (20, 20, 20),
                "max_neighbours": 12,
        },
        {
		"cov_model": cov5,
                "radiuses": (20, 20, 20),
                "max_neighbours": 12,
        }]          

variogram1 =       CovarianceModel(type=covariance.exponential, ranges=(10, 10, 10), sill=1,   nugget=0)
variogram_nugget = CovarianceModel(type=covariance.exponential, ranges=(10, 10, 10), sill=1,   nugget=0.5)
variogram2 =       CovarianceModel(type=covariance.exponential, ranges=(10, 10, 10), sill=0.4, nugget=0)
variogram3 =       CovarianceModel(type=covariance.gaussian,    ranges=(10, 10, 10), sill=1,   nugget=0)
variogram4 =       CovarianceModel(type=covariance.spherical,   ranges=(10, 10, 10), sill=1,   nugget=0)

sck_var_primary =  CovarianceModel(type=covariance.exponential, ranges=(10, 10, 10), sill=1, nugget=0)
sck_var_secondary =CovarianceModel(type=covariance.exponential, ranges=(10, 10, 10), sill=1, nugget=0)

neighbourhood1 = {"radiuses": (20, 20, 20), "max_neighbours":12}
sck_primary =   {'data': prop_cont, 'mean': calc_mean(prop_cont), 'cov_model': sck_var_primary}
sck_secondary = {'data': sec_data, 'mean': calc_mean(sec_data), 'cov_model': sck_var_secondary}

test_data = [
		("OK", 
			ordinary_kriging, [neighbourhood1, {"prop":prop_cont, "grid": grid, "cov_model": variogram1}],
			"current/RESULT_OK_BIG.INC",
			"BIG_OK",
			"fixed/RESULT_OK_BIG.INC"), 
		("SK",
			simple_kriging, [neighbourhood1, { "prop": prop_cont, "grid": grid, "cov_model": variogram1, "mean": 1.6}],
			"current/RESULT_SK_BIG.INC", "BIG_SK",
			"fixed/RESULT_SK_BIG.INC"),
		("SK_nugget",
			simple_kriging, [neighbourhood1, { "prop": prop_cont, "grid": grid, "cov_model": variogram_nugget, "mean": 1.6}],
			"current/RESULT_SK_BIG_NUGGET.INC", "BIG_SK_NUGGET",
			"fixed/RESULT_SK_BIG_NUGGET.INC"),
		("SK_var_gaussian",
			simple_kriging, [neighbourhood1, { "prop": prop_cont, "grid": grid, "cov_model": variogram1, "mean": 1.6}],
			"current/RESULT_SK_BIG_VAR_GAUSS.INC", "BIG_SK_VAR_GAUSS",
			"fixed/RESULT_SK_BIG_VAR_GAUSS.INC"),
		("SK_var_spherical",
			simple_kriging, [neighbourhood1, { "prop": prop_cont, "grid": grid, "cov_model": variogram1, "mean": 1.6}],
			"current/RESULT_SK_BIG_VAR_SPHERICAL.INC", "BIG_SK_VAR_SPHERICAL",
			"fixed/RESULT_SK_BIG_VAR_SPHERICAL.INC"),
		("LVM",
			lvm_kriging, [neighbourhood1, {"prop": prop_cont, "grid": grid, "cov_model": variogram1, "mean_data": lvm_mean_data}],
			 "current/lvm.inc", "LVM_MEDIUM",
			  "fixed/lvm.inc"),
		("SGS1",
			sgs_simulation, [neighbourhood1, {"prop": prop_cont, "grid": grid, "cov_model": variogram2, "seed": 3439275, "kriging_type": "sk", "force_single_thread": True}],
			 "current/SGS1.INC", "SGS1",
			 "fixed/SGS1.INC"),
		("SGS1_masked",
			sgs_simulation, [neighbourhood1, {"prop": prop_cont, "grid": grid, "cov_model": variogram2, "seed": 3439275, "kriging_type": "sk", "mask": mask_prop, "force_single_thread": True}],
			 "current/SGS1_MASKED.INC", "SGS1_MASKED",
			 "fixed/SGS1_MASKED.INC"),
		("SGS_CDF_EQUAL",
			sgs_simulation, [neighbourhood1, {"prop": prop_cont, "grid": grid, "cov_model": variogram2, "seed": 3439275, "kriging_type": "sk", "cdf_data": prop_cont,"force_single_thread": True}],
			 "current/SGS1.INC", "SGS1",
			 "fixed/SGS1.INC"),
 		("SGS_CDF_NOT_EQUAL",
			sgs_simulation, [neighbourhood1, {"prop": prop_cont, "grid": grid, "cov_model": variogram2, "seed": 3439275, "kriging_type": "sk", "cdf_data": prop_cdf, "force_single_thread": True}],
			 "current/SGS1.INC", "SGS1",
			 "fixed/SGS_CDF_NOT_EQUAL.INC"),
		("SGS2",
			sgs_simulation, [neighbourhood1, {"prop": prop_cont, "grid": grid, "cov_model": variogram2, "seed": 24193421, "kriging_type": "sk", "force_single_thread": True}],
			 "current/RESULT_SGS2_BIG.INC", "BIG_SGS2",
			 "fixed/RESULT_SGS2_BIG.INC"),
		("SGS_LVM", sgs_simulation, [neighbourhood1, {"prop": prop_cont, "grid": grid, "cov_model":variogram2, "seed": 3439275, "mean": lvm_mean_data, "force_single_thread": True}],
			"current/SGS_LVM.INC", "SGS_LVM", 
			"fixed/SGS_LVM.INC"),
		("IK", indicator_kriging, [{"prop": ik_prop, "grid": grid, "data": ik_data, "marginal_probs": (0.8, 0.2)}],
			"current/RESULT_IK_BIG_SOFT.INC", "BIG_IK",
			"fixed/RESULT_IK_BIG_SOFT.INC"),
		("IK_Multi_Facies", indicator_kriging, [{"prop": multi_ik_prop, "grid": grid, "data": multi_ik_data, "marginal_probs":[0.24,0.235,0.34,0.18]}],
			"current/RESULT_IK_MULTI.INC", "MULTI_IK",
			"fixed/RESULT_IK_MULTI.INC"),
		("MIK", median_ik, [neighbourhood1, {"prop": ik_prop, "grid": grid, "cov_model": variogram1, "marginal_probs": (0.8, 0.2) }],
			"current/RESULT_MIK_BIG_SOFT.INC", "BIG_MIK",
			"fixed/RESULT_IK_BIG_SOFT.INC"),
		("SIS", sis_simulation, [{"prop": ik_prop, "grid": grid, "data": ik_data, "marginal_probs":(0.8,0.2), "seed": 3241347, "force_single_thread": True}],
			"current//RESULT_SIS_BIG_SOFT.INC", "SIS_RESULT_BIG_SOFT",
			"fixed/RESULT_SIS_BIG_SOFT.INC"),
		("SIS_Multi_Facies", sis_simulation, [{"prop": multi_ik_prop, "grid": grid, "data": multi_ik_data, "marginal_probs":[0.24,0.235,0.34,0.18], "seed": 3241347, "force_single_thread": True}],
			"current//RESULT_SIS_MULTI.INC", "SIS_RESULT_MULTI",
			"fixed/RESULT_SIS_MULTI.INC"),
		("SIS_MASKED", sis_simulation, [{"prop": ik_prop, "grid": grid, "data": ik_data, "marginal_probs": (0.8, 0.2), "seed": 3241347, "mask": mask_prop, "force_single_thread": True}],
			"current//RESULT_SIS_BIG_SOFT_MASKED.INC", "SIS_RESULT_BIG_SOFT_MASKED",
			"fixed/RESULT_SIS_BIG_SOFT_MASKED.INC"),
		("Simple_CoK_M1", simple_cokriging_markI, [neighbourhood1, { "prop": prop_cont, "grid": grid, "cov_model": variogram1, "secondary_data": sec_data, "primary_mean": calc_mean(prop_cont),	"secondary_mean": calc_mean(sec_data), "secondary_variance": 5.9 , "correlation_coef": 0.97}],
			"current//RESULT_SCK_MARK_1.INC", "SCK_MARK_1",
			"fixed/RESULT_SCK_MARK_1.INC"),
		("Simple_CoK_M2", simple_cokriging_markII, [neighbourhood1, { "primary_data": sck_primary, "secondary_data": sck_secondary, "grid": grid, "correlation_coef": 0.97}],
			"current//RESULT_SCK_MARK_2.INC", "SCK_MARK_2",
			"fixed/RESULT_SCK_MARK_2.INC"),
		("SIS_LVM", sis_simulation, [{"prop": multi_ik_prop, "grid": grid, "data": multi_ik_data, "seed": 3241347, "marginal_probs": mean_props_multi_ik, "force_single_thread": True}],
			"current//RESULT_SIS_BIG_SOFT_LVM.INC", "SIS_RESULT_BIG_SOFT_LVM",
			"fixed/RESULT_SIS_BIG_SOFT_LVM.INC"),
		("SIS_LVM_NO_CORELL", sis_simulation, [{"prop": multi_ik_prop, "grid": grid, "data": multi_ik_data, "seed": 3241347, "marginal_probs": mean_props_multi_ik, "use_correlogram": False, "force_single_thread": True}],
			"current//RESULT_SIS_BIG_SOFT_LVM_NO_CORELL.INC", "SIS_RESULT_BIG_SOFT_LVM_NO_CORELL",
			"fixed/RESULT_SIS_BIG_SOFT_LVM_NO_CORELL.INC"),



]


def test_vpc_lvm(prop, result):
	MarginalProbs = CalcMarginalProbsIndicator(prop[0], prop[1], multi_indicators)

	VPCs = CalcVPCsIndicator(prop[0], prop[1], multi_indicators, MarginalProbs)
	VPCsCubes = CubesFromVPCs(VPCs, size_prop[0], size_prop[1])

	VPCs_Result = CalcVPCsIndicator(result[0], result[1], multi_indicators, MarginalProbs)

	diff = 0.0
	for k in xrange(len(multi_indicators)):
		diff += sum(VPCs[k] - VPCs_Result[k])

	return diff


times = []

def test_algo(algo_name, algo, params, current_file_path, current_prop_name, fixed_file_path):
	print "=================================="
	print "Testing %s..." % (algo_name)
	p = {}
	for pr in params:
		p.update(pr)

	time1 = time.time()
	result_current = algo(**p)
	time2 = time.time()

	times.append((algo_name, time2 - time1))
	
	# uncomment to make new fixed properties
#	print "Writing property..."
#	write_property(result_current, fixed_file_path, algo_name, -99)
#	print "Done"
	
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

	# VPC test
	diff_vpc = 0.0
	if p.has_key("mean_data"):
		if p["mean_data"] is mean_props_multi_ik:
			diff_vpc = test_vpc_lvm(p["prop"], result_current) 
			print "VPC diff is: ", diff_vpc
	
	if all(diff_res < 0.01) & (diff_vpc < 0.01):
	#if all(diff_res.mean() < 0.01):
		result = True
		diff = 0
		print "Ok"
	else:
		result = False
		print "Error"
		_, _, diff, _ = calc_razn(x,y,z,result_fixed[0], result_current[0])
		was_errors = True
	

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

set_thread_num(2)
test_all()
for a, t in times:
	print "%s:\t%s s." % (a,t)
#test_only(["SIS_LVM"])
stdin.readline()
