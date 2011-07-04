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

from decl_grid import *
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

x = 14
y = 3
dx = 3
dy = 2
c = 2
n_p = 7

dict = load_gslib_file("decluster.txt")

array1 = dict['Northing']
array2 = dict['Elev.']
print array1
print array2

min_max = get_rect(array1, array2)
l1 = (min_max[2] - min_max[0])/dx
l2 = (min_max[3] - min_max[1])/dy

# Lets define 2D grid with dx*dy cells and l1/l2 cells length
array_grid = Grid(min_max[0], min_max[1], dx, dy, l1, l2)

# Add points to 2D grid
for i in xrange(x):
	array_grid.add_point(array1[i], array2[i])

#Cell declustering calculation

w_cell = array_grid.get_weights_cell()
w_cell = stand_weight(w_cell, x)
print "Cell declustering"
print w_cell

#Inverse distance weighting calculation

widw = w_idw(array_grid, x, dx, dy, c)
widw = stand_weight(widw, x)
print "Inverse Distance Weighting"
print widw

#Kriging weights calculation

wsk = w_kriging(array_grid, x, dx, dy, array1, array2)
wsk = stand_weight(wsk, x)
print "Kriging Weights"
print wsk

#Polygonal declustering calculation

wp = w_poly(array_grid, x, array1, array2, n_p)
wp = stand_weight(wp, x)
print "Stand polygonal weigths"
print wp

#Drawing bar
bar_show(wp, w_cell, wsk, widw, x)
