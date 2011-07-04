import sys
sys.path.append(r'../shared')

from decl_grid import *
from numpy import *
from geo import *
from statistics import *

# x = 14
# y = 3
# d_x = 10
# d_y = 10

def optimal_dx_dy(array1, array2, array3, d_x, d_y, min_max, x):
	w_m = []
	dx_ar = []
	dy_ar = []
	for dx in range(1, d_x):
		for dy in range(1, d_y):
			l1 = (min_max[2] - min_max[0])/dx
			l2 = (min_max[3] - min_max[1])/dy

			array_grid = Grid(min_max[0], min_max[1], dx, dy, l1, l2)

			for i in xrange(x):
				array_grid.add_point(array1[i], array2[i])

#cell_declustering

			w_cell = array_grid.get_weights_cell()
		#print "Cell_declustering"
			w_cell = stand_weight(w_cell, x)
		#print w_cell
			w_m.append(w_mean(w_cell, array3))
			dx_ar.append(dx)
			dy_ar.append(dy)

	#print w_m
	w_min = min(w_m)
	for i in xrange(len(w_m)):
		if (w_m[i] == w_min):
			i_min = i
	#print i_min
	#print dx_ar[i_min], dy_ar[i_min]
	return dx_ar[i_min], dy_ar[i_min]