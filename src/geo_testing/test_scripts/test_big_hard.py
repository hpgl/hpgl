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
prop_cont = load_cont_property("test_data/BIG_HARD_DATA.INC", -99)
print "done"

grid = SugarboxGrid(166, 141, 225)


if (not "nook" in argv):
#	while True:
	time1 = time.time()
	prop2 = ordinary_kriging(prop_cont, grid,
		radiuses = (20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10, 10, 10),
		sill = 1)
	time2 = time.time()
	time_OK_p = time2 - time1
	write_property(prop2, "current/RESULT_OK_BIG.INC", "BIG_OK", -99)
	del(prop2)

if (not "nosk" in argv):
	time1 = time.time()
	prop3 = simple_kriging(prop_cont, grid, radiuses = (20, 20, 20),
		max_neighbours = 12,
		covariance_type = covariance.exponential,
		ranges = (10, 10, 10),
		sill = 1,
		mean = 1.6)
	time2 = time.time()
	time_SK_p = time2 - time1
	write_property(prop3, "current/RESULT_SK_BIG.INC", "BIG_SK", -99)
	del(prop3)

if (not "nolvm" in argv):
	mean_data = load_mean_data("test_data/mean_0.5_166_141_225.INC")
	time1 = time.time()
	prop_lvm = lvm_kriging(prop_cont, grid, mean_data,
			radiuses = (20, 20, 20),
                        max_neighbours = 12,
                        covariance_type = covariance.exponential,
                        ranges = (10, 10, 10),
                        sill = 1)
	time2 = time.time()
	time_LVM_p = time2 - time1
	write_property(prop_lvm, "current/lvm.inc", "lvm_medium", -99)
	del(mean_data)
	del(prop_lvm)

if (not "nosgs" in argv):
	
	sgs_params = {
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"covariance_type": covariance.exponential,
		"ranges": (10, 10, 10),
		"sill": 0.4,
		"kriging_type": "sk"}
	mean_data = load_mean_data("test_data/mean_0.5_166_141_225.INC")
	lvm_sgs_params = {
		"radiuses": (20, 20, 20),
		"max_neighbours": 12,
		"covariance_type": covariance.exponential,
		"ranges": (10, 10, 10),
		"sill": 0.4,
		"mean_data": mean_data}

	
	time1 = time.time()	
	sgs_result1 = sgs_simulation(prop_cont, grid, seed=3439275, **sgs_params)
	time2 = time.time()
	time_SGS_p = time2 - time1
	write_property(sgs_result1, "current/SGS1.INC", "SGS1", -99)

	# has bug for now, excluded
	#time1 = time.time()
	#sgs_lvm = sgs_simulation(prop_cont, grid, seed=3439275, **lvm_sgs_params)
	#time2 = time.time()
	#time_LVM_SGS_p = time2 - time1
	#write_property(sgs_lvm, "current/SGS_LVM.INC", "SGS_LVM", -99)

	del(sgs_result1)
	del(sgs_lvm)
	
ik_prop = load_ind_property("test_data/BIG_HARD_DATA.INC", -99,[0,1])
print 'Property data loaded.'


ik_data =  [	{
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

if (not "noik" in argv):
	time1 = time.time()
	ik_result = indicator_kriging(ik_prop, grid, ik_data)
	time2 = time.time()
	time_IK_p = time2 - time1
	write_property(ik_result, "current/RESULT_IK_BIG_SOFT.INC", "BIG_IK", -99)

	
if (not "nosis" in argv):
	time1 = time.time()
	sis_result = sis_simulation(ik_prop, grid, ik_data, seed=3241347)
	time2 = time.time()
	time_SIS_p = time2 - time1
	write_property(sis_result, "current/RESULT_SIS_BIG_SOFT.INC", "SIS_RESULT_BIG_SOFT", -99)


print "time SK(GOCAD, Python, /) ", time_SK, time_SK_p, time_SK/time_SK_p
	
print "time OK(GOCAD, Python, /) ", time_OK, time_OK_p, time_OK/time_OK_p
   
print "time IK(GOCAD, Python, /) ", time_IK, time_IK_p, time_IK/time_IK_p

print "time LVM(GOCAD, Python, /) ", time_LVM, time_LVM_p, time_LVM/time_LVM_p

print "time SGS(GOCAD, Python, /) ", time_SGS, time_SGS_p, time_SGS/time_SGS_p

print "time SGS_LVM(GOCAD, Python, /) ", time_LVM_SGS, time_LVM_SGS_p, time_LVM_SGS/time_LVM_SGS_p

print "time SIS(GOCAD, Python, /) ", time_SIS, time_SIS_p, time_SIS/time_SIS_p

stdin.readline()
