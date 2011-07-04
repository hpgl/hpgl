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
#	Script for lesson 7.2
#	"Conditioning by Kriging"
# ------------------------------------------------

import sys
sys.path.append(r'../shared')

from numpy import *
from geo import *
from matplotlib import *
from pylab import *
from scipy import *
from gslib import *
from grid_3d import *
from gaussian_cdf import *

#---------------------------------------------------
#	Problem:
#
#	By using the 1D dataset, transect.txt, generate a conditional simulation using the conditioning by kriging method and plot the results
#
# ----------------------------------------------------

# number of cells
i_max = 20
j_max = 1
k_max = 1

# Loading sample data from file
dict = load_gslib_file("transect.txt")

x_coord = dict['X']
value = dict['val']

# Define 3D grid 
grid = SugarboxGrid(i_max, j_max, k_max)

# x, y, z size(m)
nx = 1 + (max(x_coord) - min(x_coord))/i_max
ny = 1
nz = 1

array_grid = Grid(min(x_coord) - 5, 0, 0, i_max, j_max, k_max, nx, ny, nz)
 
array_val = array(zeros((i_max)), order='F', dtype = 'float32')
prop_val = array(zeros((i_max)), order='F', dtype = 'float32')
array_for_sk = array(zeros((i_max)), order='F', dtype = 'float32')
array_val[0:i_max] = -99
prop_val[0:i_max] = -99
array_for_sk[0:i_max] = -99

array_defined = array(zeros((i_max)), order='F', dtype = 'uint8')
prop_defined = array(zeros((i_max)), order='F', dtype = 'uint8')

x_val = array([])
for i in xrange(i_max):
	x, y, z = get_center_points(i, 0, 0, nx, 1, 1, min(x_coord) - 5, 0, 0)
	x_val = append(x_val, x)

for q in xrange(len(x_coord)):
	i, j, k = array_grid.get_ijk(x_coord[q], 0, 0)
	array_val[i] = value[q]
	array_defined[i] = 1
	x_val[i] = x_coord[q]

#Generate a kriged field of estimates by performing SK on the Gaussian-transformed dataset in transect.txt
prop1 = (float32(array_val), array_defined)
variogram1 = CovarianceModel(type=covariance.exponential, ranges = (5, 1, 1), sill = 1)

prop_result1 = simple_kriging(prop = prop1, grid = grid, radiuses =(5, 1, 1), max_neighbours = 2, cov_model = variogram1)
print "SK result:", prop_result1[0]

#Generate an unconditional Gaussian simulation, retaining the values at the data locations
prop = (float32(prop_val), prop_defined)
variogram2 = CovarianceModel(type=covariance.exponential, ranges = (5, 1, 1), sill = 1)

sgs_params = {"cov_model": variogram2, "cdf_data":  prop1, "force_single_thread": True}
sgs_result = sgs_simulation(prop, grid, radiuses = (2, 1, 1), max_neighbours = 2, seed=3244759, **sgs_params)
print "SGS result:", sgs_result[0]

sgsed_harddata = array([])
for i in xrange(i_max):
	if (array_defined[i] == 1):
		array_for_sk[i] = sgs_result[0][i]
		sgsed_harddata = append(sgsed_harddata, sgs_result[0][i])

#Perform a second simple kriging using these values
prop2 = (float32(array_for_sk), array_defined)

prop_result2 = simple_kriging(prop = prop2, grid = grid, radiuses =(5, 1, 1), max_neighbours = 2, cov_model = variogram1 )
print "Simple Kriging result:", prop_result2[0]

#Take the difference between the fields of SGS and second SK and the result to kriged field from first SK
diff = array([])
final_diff = array([])
diff = prop_result2[0] - sgs_result[0]
final_diff = diff + prop_result1[0]
print "Error Field:", diff
print "Cond.Sim:", final_diff

# SK of the dataset and an unconditional SGS
figure()
plot(x_coord, value,'bo')
p1 = plot(x_val, prop_result1[0])
p2 = plot(x_val, sgs_result[0], '--')
legend( (p1[0], p2[0]), ('z*(u)', 'U.C.Sim'),  'upper left')
xlabel("Distance (m)")
ylabel("Value")
title("SK of the dataset and an unconditional SGS")
axis([0.0, 30, -3.0, 2.0])

# Simulated error field obtained by taking the difference between a SK of the values retained from the unconditional simulation at the locations of the data
figure()
plot(x_coord, sgsed_harddata, 'bo')
p1 = plot(x_val, prop_result2[0])
p2 = plot(x_val, sgs_result[0], '--')
p3 = plot(x_val, diff, '-.')
legend( (p1[0], p2[0], p3[0]), ('zs*(u)', 'U.C.Sim', 'Error Field'),  'upper left')
xlabel("Distance (m)")
ylabel("Value")
axis([0.0, 30, -3.0, 2.0])

# Final resultant conditional simulation obtained by adding the realization of the error field to the original kriging of the data
figure()
plot(x_coord, value,'bo')
p1 = plot(x_val, prop_result1[0])
p2 = plot(x_val, sgs_result[0], '--')
p3 = plot(x_val, diff, '-.')
p4 = plot(x_val, final_diff, 'k')
legend( (p1[0], p2[0], p3[0], p4[0]), ('z*(u)', 'U.C.Sim', 'Error Field', 'Cond.Sim'),  'upper left')
xlabel("Distance (m)")
ylabel("Value")
axis([0.0, 30, -3.0, 2.0])
show()