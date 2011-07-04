#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

#
#	Solved problems in Geostatistics
#
# ------------------------------------------------
#	Script for lesson 4.1
#	"Impact of the central limit theorem"
# ------------------------------------------------

from numpy import *
from geo import *
from matplotlib import *
from scipy import *
from pylab import *

import sys
sys.path.append(r'../shared')
from statistics import *
import matplotlib.pyplot

# ------------------------------------------------
#	Problem:
#	
#	(1) Verify that the sum of independent random variables tends toward a normal distribuition as predicted by the CLT. For this problem, consider 10 random variables, Xi, i = 1, ..., n, with a uniform probability distrubution between 0 and 1, and create a RV, S, that is the sum of these 10 uniform RVs.
#	Generate several realizations of S and calculate its mean and variance
# ------------------------------------------------

# Function to generate random uniform values
def get_random_uniform_values(n):
	x = zeros( (n), dtype = float)
	for i in xrange(n):
		x[i] = uniform(0,1)
	return x

# Since RVs are independent we can calculate variance simplier
def calc_var_sum(std_dev, n):
	return std_dev**2 / n


# Number of random variables
n = 10
# Number of S realizations
S_num = 100

# Generate one realization of random variables and check mean/var
random_variables = get_random_uniform_values(n)

print "---------------------------------------------------"
rand_mean = random_variables.mean()
rand_var = random_variables.var()

print "One random realization mean is: ", rand_mean
print "One random realization var is: ", rand_var
print "---------------------------------------------------"

# Calculating summary mean/var for S realizations

# Vector with S realizations
summary_vec = zeros( (S_num), dtype = float)

for j in xrange(S_num):
	random_variables = get_random_uniform_values(n)
	for i in xrange(n):
		summary_vec[j] = summary_vec[j] + random_variables[i]

sum_mean = summary_vec.mean() 
sum_var = calc_var_sum(calc_quadr_var(summary_vec, sum_mean),n)

print "Summary mean is: ", sum_mean
print "Summary variance is:", sum_var
print "---------------------------------------------------"

print "plotting histograms..."
# Histogram of random variables
figure()
hist(random_variables)
xlabel("Random variables")
ylabel("Number")
title("Histogram of random variables")

# Histogram of summary random variables statistics
figure()
hist(summary_vec)
xlabel("Summary random variables")
ylabel("Number")
title("Histogram of summary random variables statistics")
show()
