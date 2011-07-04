#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from sys import *
from geo import *
from razn import *
from python_property import *
import os
import time

x=166
y=141
z=225

time_OK = 290.2
time_SK = 294.1
time_IK = 644.5
time_LVM = 283.1
time_SGS = 19.8
time_LVM_SGS = 21.2
time_SIS = 23.1

if not os.path.exists("current/"):
        os.mkdir("current/")

print "loading double property..."
prop = load_ind_property("test_data/BIG_HARD_DATA.INC", -99, [0,1])
ik_prop = load_ind_property("test_data/BIG_HARD_DATA.INC", -99,[0,1])
prop_cont = load_cont_property("test_data/BIG_HARD_DATA.INC", -99)
print "done"

was_errors = False

grid = SugarboxGrid(166, 141, 225)

lvm_mean_data = load_mean_data("test_data/mean_0.5_166_141_225.INC")  

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
			time_OK), 
		("SK",
			simple_kriging, [variogram1, neighbourhood1, { "prop": prop_cont, "grid": grid, "mean": 1.6}],
			time_SK),
		("LVM",
			lvm_kriging, [variogram1, neighbourhood1, {"prop": prop_cont, "grid": grid, "mean_data": lvm_mean_data}], time_LVM),
		("SGS1",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "kriging_type": "sk"}],
                        time_SGS),
		("SGS2",
			sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 24193421, "kriging_type": "sk"}],
			time_SGS),
		("SGS_LVM", sgs_simulation, [variogram2, neighbourhood1, {"prop": prop_cont, "grid": grid, "seed": 3439275, "mean_data": lvm_mean_data}],
			time_LVM_SGS),
		("IK", indicator_kriging, [{"prop": ik_prop, "grid": grid, "data": ik_data}],
			time_IK),
		("SIS", sis_simulation, [{"prop": ik_prop, "grid": grid, "data": ik_data, "seed": 3241347}],
			time_SIS)
		]

def test_algo(algo_name, algo, params, gocad_time):
	print "=================================="
	print "Testing %s..." % (algo_name)
	p = {}
	for pr in params:
		p.update(pr)
	time1 = time.time()
	prop = algo(**p)
	time2 = time.time()
	del (prop)
	print "Time of %s in GoCAD: %s and in Python: %s" % (algo_name, gocad_time, time2 - time1)
	if not gocad_time is None:
		print "Time supremacy: %s." % (gocad_time / (time2 - time1) )	
	print "\n\n"

def test_all():
	for data in test_data:
		test_algo(*data)
		print "test sucseed."

def test_sis_only():
	errors = 0
	for data in test_data:
		if (data[0] == "SIS"):
			if not test_algo(*data):
				errors += 1


#if (not "nook" in argv):
#       while True:
#        time1 = time.time()
#        prop2 = ordinary_kriging(prop_cont, grid,
#                                 radiuses = (20, 20, 20),
#                                 max_neighbours = 12,
#                                 covariance_type = covariance.exponential,
#                                 ranges = (10, 10, 10),
#                                 sill = 1)
#        time2 = time.time()
#        time_OK_p = time2 - time1
#        write_property(prop2, "current/RESULT_OK_BIG.INC", "BIG_OK", -99)
#        del(prop2)

#if (not "nosk" in argv):
#        time1 = time.time()
#        prop3 = simple_kriging(prop_cont, grid,
#                radiuses = (20, 20, 20),
#                max_neighbours = 12,
#                covariance_type = covariance.exponential,
#                ranges = (10, 10, 10),
#                sill = 1,
#                mean = 1.6)
#        time2 = time.time()
#        time_SK_p = time2 - time1
#        write_property(prop3, "current/RESULT_SK_BIG.INC", "BIG_SK", -99)
#        del(prop3)


#if (not "nolvm" in argv):
#        mean_data = load_mean_data("test_data/mean_0.487_166_141_20.inc")       
#        time1 = time.time()
#        prop_lvm = lvm_kriging(prop_cont, grid, mean_data,
#                        radiuses = (20, 20, 20),
#                max_neighbours = 12,
#                covariance_type = covariance.exponential,
#                ranges = (10, 10, 10),
#                sill = 1)
#        time2 = time.time()
#        time_LVM_p = time2 - time1
#        write_property(prop_lvm, "current/lvm.inc", "lvm_medium", -99)
#        del(mean_data)
#        del(prop_lvm)

#if (not "nosgs" in argv):
#        
#        sgs_params = {
#                "radiuses": (20, 20, 20),
#                "max_neighbours": 12,
#                "covariance_type": covariance.exponential,
#                "ranges": (10, 10, 10),
#                "sill": 0.4,
#                "kriging_type": "sk"}
                
#        mean_data = load_mean_data("test_data/mean_0.487_166_141_20.inc")
#        lvm_sgs_params = {
#               "radiuses": (20, 20, 20),
#                "max_neighbours": 12,
#                "covariance_type": covariance.exponential,
#                "ranges": (10, 10, 10),
#                "sill": 0.4,
#                "mean_data": mean_data}

#        time1 = time.time()     
#        sgs_result1 = sgs_simulation(prop_cont, grid, seed=3439275, **sgs_params)
#       time2 = time.time()
#        time_SGS_p = time2 - time1
#        write_property(sgs_result1, "current/SGS1.INC", "SGS1", -99)
#
#       sgs_result2 = sgs_simulation(prop_cont, grid, seed=24193421, **sgs_params)
#        write_property(sgs_result2, "current/SGS2.INC", "SGS2", -99)

#        time1 = time.time()
#        sgs_lvm = sgs_simulation(prop_cont, grid, seed=3439275, **lvm_sgs_params)
#        time2 = time.time()
#        time_LVM_SGS_p = time2 - time1
#        write_property(sgs_lvm, "current/SGS_LVM.INC", "SGS_LVM", -99)

#        write_property(sgs_result2, "current/RESULT_SGS2_BIG.INC", "BIG_SGS2", -99)
#        del(sgs_result1)
#        del(sgs_result2)
#        del(sgs_lvm)

#if (not "noik" in argv):
#        time1 = time.time()
#        ik_result = indicator_kriging(ik_prop, grid, ik_data)
#        time2 = time.time()
#        time_IK_p = time2 - time1
#        write_property(ik_result, "current/RESULT_IK_BIG_SOFT.INC", "BIG_IK", -99)

#        ik_result = indicator_kriging(ik_prop, grid, ik_data)
#        write_property(ik_result, "current/RESULT_IK_VPC_BIG_SOFT.INC", "BIG_IK", -99)

#	ik_result = median_ik(ik_prop, grid, values=(0,1), marginal_probs = (0.8, 0.2), radiuses = (20, 20, 20), max_neighbours = 12, covariance_type=1, ranges=(10, 10, 10), sill = 1)
#	write_property(ik_result, "current/RESULT_MIK_BIG_SOFT.INC", "BIG_MIK", -99)

#if (not "nosis" in argv):
#        time1 = time.time()
#        sis_result = sis_simulation(ik_prop, grid, ik_data, seed=3241347)
#        time2 = time.time()
#        time_SIS_p = time2 - time1
#        write_property(sis_result, "current/RESULT_SIS_BIG_SOFT.INC", "SIS_RESULT_BIG_SOFT", -99)



#print "Comparing SK..."
#result_sk_big = load_property_python(x,y,z,"fixed/RESULT_SK_BIG.INC",True)
#result1_sk_big = load_property_python(x,y,z,"current/RESULT_SK_BIG.INC",True)
#if all(result_sk_big==result1_sk_big):
#    print "ok"
#else:
#    print "error"
#    calc_razn(x,y,z,result_sk_big,result1_sk_big)

#print "Comparing OK..."
#result_ok_big = load_property_python(x,y,z,"fixed/RESULT_OK_BIG.INC",True)
#result1_ok_big = load_property_python(x,y,z,"current/RESULT_OK_BIG.INC",True)
#if all(result_ok_big==result1_ok_big):
#    print "ok"
#else:
#    print "error"
#    calc_razn(x,y,z,result_ok_big,result1_ok_big)

#print "Comparing SGS2..."
#result_sgs2_big = load_property_python(x,y,z,"fixed/RESULT_SGS2_BIG.INC",True)
#result1_sgs2_big = load_property_python(x,y,z,"current/RESULT_SGS2_BIG.INC",True)
#if all(result_sgs2_big==result1_sgs2_big):
#    print "ok"
#else:
#    print "error"
#    calc_razn(x,y,z,result_sgs2_big,result1_sgs2_big)
    
#print "Comparing SIS..."
#result_sis_big = load_property_python(x,y,z,"fixed/RESULT_SIS_BIG_SOFT.INC")
#result1_sis_big = load_property_python(x,y,z,"current/RESULT_SIS_BIG_SOFT.INC")
#if all(result_sis_big==result1_sis_big):
#    print "ok"
#else:
#    print "error"
#    calc_razn(x,y,z,result_sis_big,result1_sis_big)

#print "Comparing SGS_LVM..."
#result_sgs_lvm = load_property_python(x,y,z,"fixed/SGS_LVM.INC",True)
#result1_sgs_lvm = load_property_python(x,y,z,"current/SGS_LVM.INC",True)
#if all(result_sgs_lvm==result1_sgs_lvm):
#    print "ok"
#else:
#    print "error"
#    calc_razn(x,y,z,result_sgs_lvm,result1_sgs_lvm)

#print "Comparing SGS1..."
#result_sgs1 = load_property_python(x,y,z,"fixed/SGS1.INC",True)
#result1_sgs1 = load_property_python(x,y,z,"current/SGS1.INC",True)
#if all(result_sgs1==result1_sgs1):
#    print "ok"
#else:
#    print "error"
#    calc_razn(x,y,z,result_sgs1,result1_sgs1)

#print "Comparing SGS2..."
#result_sgs2 = load_property_python(x,y,z,"fixed/SGS2.INC",True)
#result1_sgs2 = load_property_python(x,y,z,"current/SGS2.INC",True)
#if all(result_sgs2==result1_sgs2):
#    print "ok"
#else:
#    print "error"
#    calc_razn(x,y,z,result_sgs2,result1_sgs2)

#result1_ik_big = load_property_python(x,y,z,"current/RESULT_MIK_BIG_SOFT.INC")
#if all(result_ik_big==result1_ik_big):
#    print "ok"
#else:
#    print "error"
#    calc_razn(x,y,z,result_ik_big,result1_ik_big)

#print "Comparing LVM..."
#result_lvm = load_property_python(x,y,z,"fixed/LVM.INC",True)
#result1_lvm = load_property_python(x,y,z,"current/LVM.INC",True)
#if all(result_lvm==result1_lvm):
#    print "ok"
#else:
#    print "error"
#    calc_razn(x,y,z,result_lvm,result1_lvm)


test_all()
#test_sis_only()
stdin.readline()
