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
#	Script for lesson 2.3
#	"Standartization & probability intervals"
# ------------------------------------------------

from numpy import *
from scipy import *

import sys
sys.path.append(r'../shared')

from gaussian_cdf import *
from statistics import *
from gslib import *

# ---------------------------------------------------
#	Problem:
#
#	Consider 10 data that are available over 2D area of interest, that is nominally 50 x 50 distance units square. Assume, that the data is known to be a normal distribution with mean of 12.0 and a variance of 16.0
#
# ----------------------------------------------------

print "----------------------------------------------------"
print "Loading data & initializing..."

# Loading sample data from file
x = 10
y = 4
dict = load_gslib_file("samples.txt")
	
# Mean and variance
mean = 12.0
var = 16.0

# Standart deviation
st_dev = sqrt(var)

print "Done."
print "----------------------------------------------------"

data_points_variable = dict["Variable"]
data_points_easting = dict["Easting"]
data_points_northing = dict["Northing"]
# ----------------------------------------------------
#	(1) Calculate 95% centered probability interval for the mean, that is, the 0.025 quantile and the 0.975 quantile of the theoretical distribution for the mean.
# ----------------------------------------------------

# Probability interval
p = 0.95

# Calculating quantiles
quantiles = zeros( (2), dtype = float)
# 1: 0.025 quantile
quantiles[0] = (1-p)/2
# 2: 0.975 quantile
quantiles[1] = (1+p)/2

# Retrieving probability intervals
probs = zeros( (2), dtype = float)
probs[0] = inverse_normal_score(quantiles[0], mean, var)
probs[1] = inverse_normal_score(quantiles[1], mean, var)

# Printing result on screen
print "Probability interval is: [", probs[0], ",", probs[1], "]"
print "----------------------------------------------------"

# ----------------------------------------------------
#	(2) Transform the data to be standart normal and recalculate the 95% probability interval.
# ----------------------------------------------------


# Transforming data to standart normal form
for i in xrange(x):
	data_points_variable = stand(data_points_variable, mean, st_dev)
	
# Retrieving probability intervals of transformed data
probs_trans = zeros( (2), dtype = float)
probs_trans[0] = inverse_normal_score(quantiles[0])
probs_trans[1] = inverse_normal_score(quantiles[1])

# Printing result on screen
print "Probability interval for standartized normal data is: [", probs_trans[0], ",", probs_trans[1], "]"
print "----------------------------------------------------"

# ----------------------------------------------------
#	(3) Consider now that the data are spatially dependent, and this dependece is characterized by a correlation function specified as C(h) = exp(-0.2h) where h is calculated as the distance between the samples. Note that this function applies to the standart variable, that is, C(0) = 1.0 (the variance of a standart variable). These values would have to be multiplied by 16 to establish the actual covariance values. Calculate variance of the mean in this case and calculate 95% centered probability interval.
# ----------------------------------------------------

# Function for calculating covariance with model C(h) = exp(-0.2h)
def calc_cov(h):
	cov_h = exp(-0.2*h)
	return cov_h
	
# Calculating covariance matrix
cov_h = zeros( (x,x), dtype = float)

for i in xrange(x):
	for j in xrange(x):
		
		# First point coordinates
		x1 = data_points_easting[i]
		y1 = data_points_northing[i]
		
		# Second point coordinates
		x2 = data_points_easting[j]
		y2 = data_points_northing[j]
		
		# Calculating distance
		distance = calc_distance(x1,y1,x2,y2)
		
		# Calculating covariance
		cov_h [i,j] = calc_cov(distance)
	
# 1. Standartized variable 

# Calculating variance of the mean
var_cov = cov_h.mean()
print "Variance of standartized data in presense of corellation is: ", var_cov

# Retrieving probability intervals for corellated data
probs_cov = zeros( (2), dtype = float)
probs_cov[0] = inverse_normal_score(quantiles[0], var_cov, var)
probs_cov[1] = inverse_normal_score(quantiles[1], var_cov, var)

print "Probability interval for standartized data in presense of corellation is: [", probs_cov[0], ",", probs_cov[1], "]"
print "----------------------------------------------------"

# 2. Original variable 

# Calculating variance of the mean
var_cov_orig = cov_h.mean() * 16
print "Variance of data in presense of corellation is: ", var_cov_orig

# Retrieving probability intervals for corellated data
probs_cov = zeros( (2), dtype = float)
probs_cov[0] = inverse_normal_score(quantiles[0], var_cov_orig, var)
probs_cov[1] = inverse_normal_score(quantiles[1], var_cov_orig, var)

print "Probability interval for data in presense of corellation is: [", probs_cov[0], ",", probs_cov[1], "]"

# Print covariance matrix
# print "Covariance matrix: \n", cov_h






