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

from statistics import *
from numpy import *
from geo import *
from grid_3d import *
from gslib import *

# ---------------------------------------------------
#	Problem:
#
#	Examine the spatial configuration of the data and determine whether declustering is required. If so, then perform declustering and determine the representative mean of the 2D well-averaged porosity. Also, calculate the correlation between the average porosity and the collocated seismic data (using declustering weights if required).
#
# ----------------------------------------------------

print "----------------------------------------------------"
print "Loading data & initializing..."

# Loading sample data from file
dx = 10
dy = 10
dz = 1

dict = load_gslib_file("welldata.txt")

print "Done."
print "----------------------------------------------------"

array3 = zeros( (len(dict['X'])), dtype = float)
PointSet = (dict['X'], dict['Y'], array3)

nx = 10 + (max(dict['X']) - min(dict['X']))/dx
ny = 10 + (max(dict['Y']) - min(dict['Y']))/dy
nz = (max(array3) - min(array3))/dz

# Lets define 3D grid with dx*dy*dz cells and nx, ny, nz cells length
array_grid = Grid(min(PointSet[0]), min(PointSet[1]), min(PointSet[2]), dx, dy, dz, nx, ny, nz)

array4 = dict['Por']

#Cell declustering calculation
w_cell = get_weights_cell(array_grid, PointSet)

# Weights standardization
w_cell = stand_weight(w_cell, (len(dict['X'])))

#Calculate porosity mean
por_mean = calc_mean_array(array4)
print "Porosity mean =", por_mean

# Calculate porosity standard deviation
por_quadr_var = calc_quadr_var(array4, por_mean)
#print "Porosity standart deviation =", por_quadr_var

#Calculate porosity mean with cell declustering
por_cell_mean = w_mean(w_cell, array4)
print "Porosity mean with cell declustering =", por_cell_mean

# Calculate porosity variance with cell declustering
por_cell_var = w_var(w_cell, array4, por_cell_mean)
print "Porosity variance with cell declustering =", por_cell_var

print "Difference between means = ", por_mean-por_cell_mean
#print "diff vars = ", por_quadr_var-por_cell_var
