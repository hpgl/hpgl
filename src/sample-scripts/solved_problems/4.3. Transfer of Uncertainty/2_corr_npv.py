#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

#
#	Solved Problems in Geostatistics
#
# ------------------------------------------------
#	Script for lesson 4.3
#	"Transfer of Uncertainty"
# ------------------------------------------------

import sys
sys.path.append(r'../shared')

from numpy import *
from geo import *
from statistics import *
from matplotlib import *
from pylab import *
from scipy import *
from scipy.interpolate import *

# ---------------------------------------------------
#	Problem:
#
#	Repeat the above evaluation of NPV assuming a correlation coefficient of 0.80 exists between revenue and costs and that the bivariate distribution between these two variables is Gaussian. You should write your own program for these problems.
#
# ----------------------------------------------------

# Mean and variance of annual revenue, R.
mean_R = 10
var_R = 2

# Mean and variance of annual costs, C.
mean_C = 8
var_C = 2

# Constant discount factor, r.
r = 0.1

# Number of years.
n = 5

# Number of realizations.
l = 1000

# Correlation coefficient
p = 0.8

# A formula for calculating NPV:
def npv_calculate(R, C, r, n):
	npv = 0.0
	for i in xrange(n):
		npv = npv + ((R[i] - C[i]) / (1 + r)**i)
	return npv

def mean_var_calculate(p, C, mean_R, var_R, mean_C, var_C, n):
	R = zeros( (n), dtype = float)
	for i in xrange(n):
		var = var_R * (1 - p**2)
		mean = mean_R + (p * sqrt(var_R) * (C[i] - mean_C)) / (sqrt(var_C))
		R[i] = random.normal(mean, sqrt(var), 1)
	return R
npv_array = []

# Repeat l times to assemble the distribution for NPV.
for i in xrange(l):
	#Simulate random C
	C = random.normal(mean_C, sqrt(var_C), n)
	#print C
	
	#Calculate R
	R = mean_var_calculate(p, C, mean_R, var_R, mean_C, var_C, n)
	
	npv_array.append(npv_calculate(R, C, r, n))
print "NPV", npv_array

[array_bins, array_hist] = histogram(npv_array, l)

for i in xrange(l-1):
	array_bins[i+1] = array_bins[i+1] + array_bins[i]

array_bin = zeros( (l), dtype = float)

for i in xrange(l):
	array_bin[i] =  float(array_bins[i]) / l

#print array_bin
array_hist = delete(array_hist, [0])
z = []

# Interpolation
int_obj = interp1d(array_bin, array_hist, kind = 'linear')
z.append(int_obj(0.1))
z.append(int_obj(0.5))
z.append(int_obj(0.9))
print "P10 = ", z[0]
print "P50 = ", z[1]
print "P90 = ", z[2]

# Calculate the probability of a negative NPV.

negative_npv = 0.0
for i in xrange(l):
	if (npv_array[i] < 0):
		negative_npv = negative_npv + 1.0

prob_npv = negative_npv / l
print "Probability of negative NPV = ", prob_npv

# Histogram (1) and cumulative histogram (2) of NPV with correlated revenue and cost inputs.

figure()
hist(npv_array, 50, normed = True)
xlabel("NPV")
ylabel("Frequency")
title("Histogram of NPV")

figure()
plot(array_hist, array_bin)
xlabel("NPV")
ylabel("Cumulative Frequency")
title("Cumulative histogram of NPV")
show()