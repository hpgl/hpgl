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
#	Script for lesson 7.3
#	"Gaussian Simulation"
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
#	Upscale the models to a cell size of 1500 * 1500 m using arithmetic, geometric, and harmonic averaging. Note the general reduction in the variance of the upscaled results relative to the point scale gridded data; and the differential variance reduction between the point scale and upscaled kriged and simulated values.
#
# ----------------------------------------------------

# number of cells
i_max = 10
j_max = 10
k_max = 1

# Loading sample data from file
dict = load_gslib_file("DBHdata.txt")

x_coord = dict['X']
y_coord = dict['Y']
z_coord = zeros((len(x_coord)), dtype = 'uint8')

# property
value = "Por"

# Define 3D grid 
grid = SugarboxGrid(i_max, j_max, k_max)

# x, y, z size(m)
nx = 1500
ny = 1500
nz = 1

# Threshold value
threshold_value = 10.5

# Lets define 3D grid 
array_grid = Grid(0, 0, 0, i_max, j_max, k_max, nx, ny, nz)

prop_ijk = zeros((i_max, j_max, k_max))
prop_ijk = require(prop_ijk, dtype=float32, requirements=['F'])

array_defined = zeros((i_max, j_max, k_max))
array_defined = require(array_defined, dtype=uint8, requirements=['F'])

# Get_sum_cell_value with arithmetic averaging
for i in xrange(i_max):
	for j in xrange(j_max):
		for k in xrange(k_max):
			arithmetic_mean = get_sum_cell_value(array_grid, x_coord, y_coord, z_coord, i, j, k, dict[value], type = 'a')
			if (arithmetic_mean > 0):
				prop_ijk[i,j,k] = arithmetic_mean
				array_defined[i,j,k] = 1
			else:
				prop_ijk[i,j,k] = -99
				array_defined[i,j,k] = 0
var_arithmetic = var(prop_ijk.compress((prop_ijk!=-99).flat))
print "Arithmetic variance:", var_arithmetic

# Get_sum_cell_value with geometric averaging
prop_geometric = zeros((i_max, j_max, k_max))
prop_geometric = require(prop_geometric, dtype=float32, requirements=['F'])
for i in xrange(i_max):
	for j in xrange(j_max):
		for k in xrange(k_max):
			geometric_mean = get_sum_cell_value(array_grid, x_coord, y_coord, z_coord, i, j, k, dict[value], type = 'g')
			if (geometric_mean > 0):
				prop_geometric[i,j,k] = geometric_mean
			else:
				prop_geometric[i,j,k] = -99
var_geometric = var(prop_geometric.compress((prop_geometric!=-99).flat))
print "Geometric variance:", var_geometric

# Get_sum_cell_value with harmonic averaging
prop_harmonic = zeros((i_max, j_max, k_max))
prop_harmonic = require(prop_harmonic, dtype=float32, requirements=['F'])
for i in xrange(i_max):
	for j in xrange(j_max):
		for k in xrange(k_max):
			harmonic_mean = get_sum_cell_value(array_grid, x_coord, y_coord, z_coord, i, j, k, dict[value], type = 'h')
			if (harmonic_mean > 0):
				prop_harmonic[i,j,k] = harmonic_mean
			else:
				prop_harmonic[i,j,k] = -99
var_harmonic = var(prop_harmonic.compress((prop_harmonic!=-99).flat))
print "Harmonic variance:", var_harmonic

var_initial = var(dict[value])
print "Point scale variance:", var_initial

initial_data_arithmetic = copy(prop_ijk)

# Transform initial data with cdf_transform function
transformed_data = copy(prop_ijk)
props, values = cdf_transform(transformed_data, -99)

# Generate a property of transformed data for SK and SGS
prop_transformed = (transformed_data, array_defined)

# Generate a property of initial data for SGS
prop_initial_arithmetic = (initial_data_arithmetic, array_defined)
prop_initial_harmonic = (prop_harmonic, array_defined)

# Generate a kriged field by performing SK on transformed data with 315 azimuth direction
variogram1 = CovarianceModel(type=covariance.spherical, ranges = (80, 20, 1), sill = 1, angles = (315, 0, 0))
krigged_transformed_data = simple_kriging(prop = prop_transformed, grid = grid, radiuses = (160, 40, 1), max_neighbours = 20, cov_model = variogram1)

back_krigged_transformed_data = copy(krigged_transformed_data)
# Back transform to original data space
back_cdf_transform(back_krigged_transformed_data[0], props, values, -99)

var_back_krigged_transformed_data = var(back_krigged_transformed_data[0])
print "var_back_krigged_transformed_data:", var_back_krigged_transformed_data
print "The diff_variance between point scale and kriged values:", (var_initial - var_back_krigged_transformed_data)

# Generate a kriged field by performing SK on initial property (arithmetic and harmonic averaging) with 315 azimuth direction
variogram1 = CovarianceModel(type=covariance.spherical, ranges = (80, 20, 1), sill = 1, angles = (315, 0, 0))
krigged_arithmetic_data = simple_kriging(prop = prop_initial_arithmetic, grid = grid, radiuses = (160, 40, 1), max_neighbours = 20, cov_model = variogram1)

krigged_harmonic_data = simple_kriging(prop = prop_initial_harmonic, grid = grid, radiuses = (160, 40, 1), max_neighbours = 20, cov_model = variogram1)

# Generate a simulated field by performing SGS on initial property (arithmetic and harmonic averaging) with 315 azimuth direction
variogram2 = CovarianceModel(type=covariance.spherical, ranges = (80, 20, 1), sill = 1, angles = (315, 0, 0))
sgs_params = {"cov_model": variogram2, "radiuses": (160, 40, 1), "max_neighbours": 20, "force_single_thread": True}

sgs_arithmetic_data = sgs_simulation(prop_initial_arithmetic, grid, seed=5232463, **sgs_params)

sgs_harmonic_data = sgs_simulation(prop_initial_harmonic, grid, seed=5232463, **sgs_params)

var_sgs_arithmetic_data = var(sgs_arithmetic_data[0])
print "Sgs on arithmetic data variance:", var_sgs_arithmetic_data
print "The differential variance between point scale and simulated values:", (var_initial - var_sgs_arithmetic_data)

# Histogram of simulated field from harmonic averaging
figure()
hist(sgs_harmonic_data[0], bins = 20)
title("Simulated field from harmonic averaging")

# Histogram of simulated field from arithmetic averaging
figure()
hist(sgs_arithmetic_data[0], bins = 20)
title("Simulated field from arithmetic averaging")

# Histogram of kriged field from harmonic averaging
figure()
hist(krigged_harmonic_data[0], bins = 20)
title("Kriged field from harmonic averaging")

# Histogram of kriged field from arithmetic averaging
figure()
hist(krigged_arithmetic_data[0], bins = 20)
title("Kriged field from arithmetic averaging")


#---------------------------------------------------
#	Problem:
#
#	Plot the cumulative histograms of the point-scale reference data and gridded models and compare the proportions of the data above the threshold value of 12.0. The differences observed here are commensurate with the changes observed in the variance and shape of the histograms in the previous step.
#
# ----------------------------------------------------

array_threshold_initial = dict[value].compress((dict[value] > threshold_value).flat)
print (float(len(array_threshold_initial)) / (len(dict[value]))) *100, "% of the data lies above the threshold"

array_threshold_krigged = krigged_arithmetic_data[0].compress((krigged_arithmetic_data[0] > threshold_value).flat)
print (float(len(array_threshold_krigged)) / (i_max * j_max * k_max)) *100, "% value of the kriged model falls above the threshold"

array_threshold_simulated = sgs_arithmetic_data[0].compress((sgs_arithmetic_data[0] > threshold_value).flat)
print (float(len(array_threshold_simulated)) / (i_max * j_max * k_max)) *100, "% value of the simulated model falls above the threshold"

figure()
hist(dict[value], bins = 20, cumulative = True)
axvline(threshold_value, linewidth = 2, color = 'k')
title("Cumulative point-scale data")

figure()
hist(krigged_arithmetic_data[0], bins = 20, cumulative = True)
axvline(threshold_value, linewidth = 2, color = 'k')
title("Cumulative kriged field from arithmetic averaging")

figure()
hist(sgs_arithmetic_data[0], bins = 20, cumulative = True)
axvline(threshold_value, linewidth = 2, color = 'k')
title("Cumulative simulated field from arithmetic averaging")

show()