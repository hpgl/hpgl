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
from decl_grid import *
from gslib import *
from opt_x_y import *

# ---------------------------------------------------
#	Problem:
#
#	Examine the spatial configuration of the data and determine whether declustering is required. If so, then perform declustering and determine the representative mean of the 2D well-averaged porosity. Also, calculate the correlation between the average porosity and the collocated seismic data (using declustering weights if required).
#
# ----------------------------------------------------

print "----------------------------------------------------"
print "Loading data & initializing..."

# Loading sample data from file
x = 62
y = 6
d_x = 10
d_y = 10

dict = load_gslib_file("welldata.txt")

print "Done."
print "----------------------------------------------------"

array1 = dict['X']
array2 = dict['Y']
array3 = dict['Por']

min_max = get_rect(array1, array2)

# Lets find optimal dx & dy values (property mean must be minimal)
dx, dy = optimal_dx_dy(array1, array2, array3, d_x, d_y, min_max, x)

print "Optimal dx = ", dx
print "Optimal dy = ", dy

l1 = (min_max[2] - min_max[0])/dx
l2 = (min_max[3] - min_max[1])/dy

# Lets define 2D grid with dx*dy cells and l1/l2 cells length
array_grid = Grid(min_max[0], min_max[1], dx, dy, l1, l2)

# Add points to 2D grid
for i in xrange(x):
	array_grid.add_point(array1[i], array2[i])

#Cell declustering calculation
w_cell = array_grid.get_weights_cell()

# Weights standardization
w_cell = stand_weight(w_cell, x)

#Calculate porosity mean
por_mean = calc_mean_array(array3)
print "Porosity mean =", por_mean

# Calculate porosity standard deviation
por_quadr_var = calc_quadr_var(array3, por_mean)
#print "Porosity standart deviation =", por_quadr_var

#Calculate porosity mean with cell declustering
por_cell_mean = w_mean(w_cell, array3)
print "Porosity mean with cell declustering =", por_cell_mean

# Calculate porosity variance with cell declustering
por_cell_var = w_var(w_cell, array3, por_cell_mean)
print "Porosity variance with cell declustering =", por_cell_var

print "Difference between means = ", por_mean-por_cell_mean
#print "diff vars = ", por_quadr_var-por_cell_var
