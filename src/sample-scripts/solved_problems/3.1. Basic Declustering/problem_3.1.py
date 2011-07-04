#
#	Solved problems in Geostatistics
#
# ------------------------------------------------
#	Script for lesson 3.1
#	"Basic Declustering"
# ------------------------------------------------

from numpy import *
from scipy import *

import sys
sys.path.append(r'../shared')

from statistics import *
from gslib import *

# -----------------------------------------------
#	Problem:
#
#	(2) Consider a gold-rich deposit with a lease limit that measures 1200x1200 m. For preliminary exploration purposes, the lease has been sampled at 15 locations. Perform declustering on the sample data using the distance to the nearest data criterion, considering only the three closest data for this weighting (i.e. n = 3). Calculate the corresponding declustered mean grade and standat deviation. Also, calculate and compare the equal weighted mean and variance to this declustered mean and variance.
# -----------------------------------------------

# Weight calculation function
def calculate_weights(points, x, n):
	weights = zeros( (x), dtype = float)
	for i in xrange(x):
		for j in xrange(n):
			weights[i] = weights[i]+points[i,j]
	return weights

print "---------------------------------------------------"
print "Loading data and initializing..."

# Loading data set
x = 15
y = 3

dict = load_gslib_file("dataset.txt")
dataset_east = dict["East"]
dataset_north = dict["North"]
# Number of closest data to take in account
n = 4 

# Vector with values of the dataset

print "Done."
print "---------------------------------------------------"

# Creating values vector

values = dict["Grade"]

# Original mean and variance
original_mean = values.mean()
original_var = calc_quadr_var(values, original_mean)

print "Original data mean is: ", original_mean
print "Original data variance is: ", original_var
print "---------------------------------------------------"

# Calculating distances between points
distances_v = zeros( (x,x), dtype = float)

for i in xrange(x):
	for j in xrange(x):
		x1 = dataset_east[i]
		y1 = dataset_north[i]

		x2 = dataset_east[j]
		y2 = dataset_north[j]

		distances_v[i,j] = calc_distance(x1, y1, x2, y2)

# Sort distances
distances_v.sort()

# Calculate weights using n closest neighbourhoods
weights = calculate_weights(distances_v, x, n)

print "Weights: \n"
print weights
print "---------------------------------------------------"

# Weighted mean and variance
weighted_mean = w_mean(weights, values)
weighted_var = w_var(weights,values, weighted_mean)

print "Weighted data mean is: ", weighted_mean
print "Weighted data variance is: ", weighted_var
print "---------------------------------------------------"

