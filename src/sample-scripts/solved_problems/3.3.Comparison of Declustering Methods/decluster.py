#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

import sys
sys.path.append(r'../shared')

from grid_3d import *
from geo import *
from matplotlib import *
from pylab import *

def w_idw(Grid, PointSet, c, nx, ny, nz):
	widw = zeros( (len(PointSet[0])), dtype = float)
	for i in xrange(Grid.i_max):
		for j in xrange(Grid.j_max):
			for k in xrange(Grid.k_max):
				x_center, y_center, z_center = get_center_points(i, j, k, nx, ny, nz, min(PointSet[0]), min(PointSet[1]), min(PointSet[2]))
				ww_idw = get_weights_idw(Grid, x_center, y_center, z_center, PointSet, c)
				for q in xrange(len(PointSet[0])):
					widw[q] = widw[q] + ww_idw[q]
	return widw


# Inverse distance weighting calculation
# def w_idw(array_grid, x, dx, dy, c):
	# array_grid.get_center_points()
	# widw = zeros( (x), dtype = float)
	# for i in xrange(dx*dy):
		# x_center, y_center = array_grid.get_center_num(i)
		# w_idw = array_grid.get_weights_idw(x_center, y_center, c)
		# for j in xrange(x):
			# widw[j] = widw[j] + w_idw[j]
	# return widw

#Kriging weights calculation
def w_kriging(Grid, PointSet):
	n_x = []
	n_y = []
	n_z = []
	for i in xrange(len(PointSet[0])):
		n_x.append(PointSet[0][i])
		n_y.append(PointSet[1][i])
		n_z.append(PointSet[2][i])
	wsk = zeros( (len(PointSet[0])), dtype = float)
	for i in xrange(Grid.i_max):
		for j in xrange(Grid.j_max):
			for k in xrange(Grid.k_max):
				x_center, y_center, z_center = get_center_points(i, j, k, Grid.nx, Grid.ny, Grid.nz, min(PointSet[0]), min(PointSet[1]), min(PointSet[2]))
				center_point = [ x_center, y_center , z_center ] 
				w_sk = simple_kriging_weights(center_point, n_x, n_y, n_z)
				for q in xrange(len(PointSet[0])):
					wsk[q] = wsk[q] + w_sk[q]
	return wsk

#Polygonal declustering calculation
# def w_poly(array_grid, x, array1, array2, n_p):
	# s = 0.0
	# s_poly = []
	# wp = []

	# for j in xrange(x):
		# x_middle = []
		# y_middle = []
		# h = array_grid.hgt_calc(array_grid.data1[j],array_grid.data2[j], n_p)
		# for i in xrange(x):
			# for k in xrange(len(h)):
				# if (i==h[k]):
					# x_middle.append(array_grid.get_segment_x_middle(array1[j], array1[i]))
					# y_middle.append(array_grid.get_segment_y_middle(array2[j], array2[i]))
					# break
		# s_poly.append(s_polygon(x_middle, y_middle))

	# for i in xrange(len(s_poly)):
		# s = s+s_poly[i]

	# for i in xrange(len(s_poly)):
		# wp.append(s_poly[i]/s)
	# return wp

#Drawing bar
# def bar_show(wp, w_cell, wsk, widw, x):
	# ind = arange(x)
	# for i in xrange(len(wp)):
		# p1 = bar(i,wp[i], color = 'y', width = 0.2)
		# p2 = bar(i+0.2,w_cell[i], width = 0.2)
		# p3 = bar(i+0.4,wsk[i],color = 'r', width = 0.2)
		# p4 = bar(i+0.6,widw[i],color = 'g', width = 0.2)
		# bar(i+0.8,0.0, color='w',width = 0.2)
	# legend( (p1[0], p2[0], p3[0], p4[0]), ('Polygonal', 'Cell', 'Kriging', 'Idw'))
	# xlabel("Number of data")
	# ylabel("Standardized weights")
	# title("Comparison of Declustering Methods")
	# xticks(ind+0.4, ('1','2','3','4','5','6','7','8','9','10','11','12','13','14'))
	# show()