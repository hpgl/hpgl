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
#	Script for lesson 4.2
#	"Bootstrap & Spatial Bootstrap"
# ------------------------------------------------

import sys
sys.path.append(r'../shared')

from numpy import *
from geo import *
from grid_3d import *
from statistics import *
from matplotlib import *
from pylab import *
from gslib import *

# ---------------------------------------------------
#	Problem:
#
#	Perform declustering and determine the representative mean of the 2D well-averaged porosity. Also, calculate the correlation between the average porosity and the collocated seismic data (using declustering weights if required). ...
#
# ----------------------------------------------------

print "----------------------------------------------------"
print "Loading data & initializing..."

# Loading sample data from file
n = 62
l = 1000
dx = 7
dy = 8
dz = 1

dict = load_gslib_file("welldata.txt")

poro_values = dict['Por']
seismic_values = dict['Seis']

print "Done."
print "----------------------------------------------------"

array3 = zeros( (len(dict['X'])), dtype = float)
PointSet = (dict['X'], dict['Y'], array3)

nx = 10 + (max(dict['X']) - min(dict['X']))/dx
ny = 10 + (max(dict['Y']) - min(dict['Y']))/dy
nz = (max(array3) - min(array3))/dz

# Lets define 3D grid with dx*dy*dz cells and nx, ny, nz cells length
array_grid = Grid(min(PointSet[0]), min(PointSet[1]), min(PointSet[2]), dx, dy, dz, nx, ny, nz)

# Correlation coefficient calculation
coef = corr_coef(poro_values, seismic_values)
print "Correlation between porosity and seismic: ", coef

#Cell declustering calculation
w_cell = get_weights_cell(array_grid, PointSet)

# Weights standardization
w_cell = stand_weight(w_cell, (len(dict['X'])))
# ----------------------------------------

# Weighted correlation coefficient calculation
w_coef = w_corr_coef(poro_values, seismic_values, w_cell)
print "Weighted correlation between porosity and seismic: ", w_coef
print "----------------------------------------------------"

# ---------------------------------------------------
#	Problem:
#
#	... Perform the bootstrap to evaluate the uncertainty in these summary statistics. Perform the bootstrap for the average and the correlation coefficient, respectively. 
#
# ----------------------------------------------------

print "Doing boostrap for poro mean and correlation with seismic data... "
print "Number of random realizations: ", l

# Doing bootstrap for mean and correlation
mean_poro = []

coef_rand = []
w_coef_rand = []

mean_seis = w_mean(w_cell, seismic_values)
mean_poro_value = w_mean(w_cell, poro_values)

for i in xrange(l):
	# Sampling arrays _pairwise_ for correlation bootstrap estimation
	[poro_rand, seismic_rand]  = rand_arrays(poro_values, seismic_values, n)
	
	mean_poro_value = poro_rand.mean()
	mean_poro.append(mean_poro_value)
	
	# Correlation
	coef_rand.append(corr_coef(poro_rand, seismic_rand))
	# Weighted correlation
	w_coef_rand.append(w_corr_coef(poro_rand, seismic_rand, w_cell))

#std_err, conf_interval = bootstrap_correlation(poro_values, seismic_values)
#print std_err, conf_interval

print "Bootstrap calculation completed."
print "----------------------------------------------------"

print "Drawing histograms..."

#Draw histogram "Distribution of bootstrapped mean for porosity"
figure()
hist(mean_poro, 50, normed=True)
xlabel("Porosity (%)")
ylabel("Frequency")
title("Distribution of bootstrapped mean for porosity")

#Draw histogram "Bootstrapped correlation between weighted porosity and collocated seismic"
figure()
hist(w_coef_rand, 50, normed=True)
xlabel("Correlation coefficient")
ylabel("Frequency")
title("Bootstrapped correlation between porosity and collocated seismic")

#Draw histogram "Bootstrapped correlation between porosity and collocated seismic"
figure()
hist(coef_rand, 50, normed=True)
xlabel("Correlation coefficient")
ylabel("Frequency")
title("Bootstrapped correlation between weighted porosity and collocated seismic")

#Draw cross plot with seismic data
figure()
plot(poro_values, seismic_values, 'bo')
xlabel("Porosity (%)")
ylabel("Seismic")
title("Cross plot porosity vs seismic")
show()
