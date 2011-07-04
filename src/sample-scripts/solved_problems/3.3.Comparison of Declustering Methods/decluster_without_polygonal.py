import sys
sys.path.append(r'../shared')

from grid_3d import *
from geo import *
from pylab import *

# Inverse distance weighting calculation
def w_idw(Grid, PointSet, c, nx, ny, nz):
	widw = zeros( (len(PointSet[0])), order = 'F', dtype = float)
	for i in xrange(Grid.i_max):
		for j in xrange(Grid.j_max):
			for k in xrange(Grid.k_max):
				x_center, y_center, z_center = get_center_points(i, j, k, nx, ny, nz, min(PointSet[0]), min(PointSet[1]), min(PointSet[2]))
				ww_idw = get_weights_idw(Grid, x_center, y_center, z_center, PointSet, c)
				for q in xrange(len(PointSet[0])):
					widw[q] = widw[q] + ww_idw[q]
	return widw

# Kriging weights calculation
def w_kriging(Grid, PointSet):
	n_x = []
	n_y = []
	n_z = []
	for i in xrange(len(PointSet[0])):
		n_x.append(PointSet[0][i])
		n_y.append(PointSet[1][i])
		n_z.append(PointSet[2][i])
	wsk = zeros( (len(PointSet[0])), order = 'F', dtype = float32)
	for i in xrange(Grid.i_max):
		for j in xrange(Grid.j_max):
			for k in xrange(Grid.k_max):
				x_center, y_center, z_center = get_center_points(i, j, k, Grid.nx, Grid.ny, Grid.nz, min(PointSet[0]), min(PointSet[1]), min(PointSet[2]))
				center_point = [ x_center, y_center , z_center ] 
				w_sk = simple_kriging_weights(center_point, n_x, n_y, n_z)
				for q in xrange(len(PointSet[0])):
					wsk[q] = wsk[q] + w_sk[q]
	return wsk

#Drawing bar
def bar_show(w_cell, wsk, widw, x):
	ind = arange(x)
	for i in xrange(len(wsk)):
		p1 = bar(i, widw[i], color = 'y', width = 0.3)
		p2 = bar(i+0.3, w_cell[i], width = 0.3)
		p3 = bar(i+0.6, wsk[i], color = 'r', width = 0.3)
		# p4 = bar(i+0.6, wp[i], color = 'g', width = 0.2)
		bar(i+0.8, 0.0, color = 'w', width = 0.2)
	legend( (p1[0], p2[0], p3[0]), ('IDW', 'Cell', 'Kriging'), 'upper left')
	xlabel("Number of data")
	ylabel("Standardized weights")
	title("Comparison of Declustering Methods")
	xticks(ind+0.4, ('1','2','3','4','5','6','7','8','9','10','11','12','13','14'))
	show()