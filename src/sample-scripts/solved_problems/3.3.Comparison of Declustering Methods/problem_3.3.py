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
#	Script for lesson 3.3
#	"Comparison of Declustering Methods"
# ------------------------------------------------

import sys
sys.path.append(r'../shared')

from grid_3d import *
from numpy import *
from geo import *
from matplotlib import *
from scipy import *
from gslib import *
from decluster import *

# ---------------------------------------------------
#	Problem:
#
#	Compute declustering weights using the four methods: Polygonal declustering, Cell declustering, Kriging declustering, Inverse distance weighting. Compare the weights and the declustered univariate statistics between the four methods, rationalize the differences, and select a preferred alternative.

#
# ----------------------------------------------------

# Loading sample data from file

#x = 14
#y = 3
dx = 1
dy = 1
dz = 1
c = 2
n_p = 7

dict = load_gslib_file("decluster.txt")

array3 = zeros( (len(dict['Northing'])), dtype = float)
PointSet = (dict['Northing'], dict['Elev.'], array3)

nx = 10 + (max(dict['Northing']) - min(dict['Northing']))/dx
ny = 10 + (max(dict['Elev.']) - min(dict['Elev.']))/dy
nz = (max(array3) - min(array3))/dz

print nx, ny, nz
# Lets define 3D grid with dx*dy*dz cells and nx, ny, nz cells length
array_grid = Grid(min(PointSet[0]), min(PointSet[1]), min(PointSet[2]), dx, dy, dz, nx, ny, nz)

#Cell declustering calculation

w_cell = get_weights_cell(array_grid, PointSet)
w_cell = stand_weight(w_cell, len(PointSet[0]))
print "Cell declustering"
print w_cell

#Inverse distance weighting calculation

widw = w_idw(array_grid, PointSet, c, nx, ny, nz)
widw = stand_weight(widw, len(PointSet[0]))
print "Inverse Distance Weighting"
print widw

#Kriging weights calculation

wsk = w_kriging(array_grid, PointSet)
wsk = stand_weight(wsk, len(PointSet[0]))
print "Kriging Weights"
print wsk

#Polygonal declustering calculation

# wp = w_poly(array_grid, x, array1, array2, n_p)
# wp = stand_weight(wp, x)
# print "Stand polygonal weigths"
# print wp

#Drawing bar
#bar_show(w_cell, wsk, widw, len(PointSet[0]))
