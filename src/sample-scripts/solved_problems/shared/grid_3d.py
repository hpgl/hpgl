#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from numpy import *
from geo import *
from scipy import *

class Grid:
	def __init__(self, x0, y0, z0, i_max, j_max, k_max, nx, ny, nz):
		self.x0 = x0
		self.y0 = y0
		self.z0 = z0
		self.i_max = i_max
		self.j_max = j_max
		self.k_max = k_max
		self.x_max = i_max * nx + x0
		self.y_max = j_max * ny + y0
		self.z_max = k_max * nz + z0
		self.nx = nx
		self.ny = ny
		self.nz = nz
	
	def get_ijk(self, x, y, z):
		if (x <= self.x_max) and (y <= self.y_max) and (z <= self.z_max):
			i = int((x - self.x0) / self.nx)
			j = int((y - self.y0) / self.ny)
			k = int((z - self.z0) / self.nz)
			return i, j, k
		else:
			return -1, -1, -1
	
	def get_cell_num(self, i, j, k):
		index = 0
		index = j * self.i_max + i + k * self.j_max * self.i_max
		return index
	
	def get_ijk_cell(self, index):
		i = mod(index, self.i_max)
		j = mod(index, self.j_max * self.i_max) / self.i_max
		k = index / (self.j_max * self.i_max)
		return i, j, k
	
def get_point_num(Grid, n, PointSet):
	N = 0
	for i in xrange(len(PointSet[0])):
		i, j, k = Grid.get_ijk(PointSet[0][i], PointSet[1][i], PointSet[2][i])
		index = Grid.get_cell_num(i, j, k)
		if (index == n):
			N = N + 1
	return N
	
def get_weights_cell(Grid, PointSet):
	w = zeros( (len(PointSet[0])), dtype = float)
	n = 0
	n_1 = 0
	for q in xrange(len(PointSet[0])):
		i, j, k = Grid.get_ijk(PointSet[0][q], PointSet[1][q], PointSet[2][q])
		n_1 = Grid.get_cell_num(i, j, k)
		n = get_point_num(Grid, n_1, PointSet)
		w[q] = 1.0 / float(n)
	return w

def get_center_points(i, j, k, nnx, nny, nnz, x_min, y_min, z_min):
	x_center = 0.0
	y_center = 0.0
	z_center = 0.0
	x_center = i * nnx + nnx/2 + x_min
	y_center = j * nny + nny/2 + y_min
	z_center = k * nnz + nnz/2 + z_min
	return x_center, y_center, z_center

def get_weights_idw(Grid, x_center, y_center, z_center, PointSet, c):
	w1 = zeros( (len(PointSet[0])), dtype = float)
	e_h = calc_e_h(x_center, y_center, z_center, c, PointSet)
	for i in xrange(len(PointSet[0])):
		h = calc_h(x_center, y_center, z_center, i, c, PointSet)
		w1[i] = h/e_h
	return w1

def calc_e_h(x_center, y_center, z_center, c, PointSet):
	e_h = 0.0
	for i in xrange(len(PointSet[0])):
		e_h = e_h + (sqrt((x_center-PointSet[0][i])**2 + (y_center-PointSet[1][i])**2 + (z_center-PointSet[2][i])**2))**(-c)
	return e_h
	
def calc_h(x_center, y_center, z_center, j, c, PointSet):
	h = (sqrt((x_center-PointSet[0][j])**2 + (y_center-PointSet[1][j])**2 + (z_center-PointSet[2][j])**2))**(-c)
	return h

def stand_weight(w, n):
	sum = 0.0
	for i in xrange(len(w)):
		sum = sum + w[i]
	for i in xrange(len(w)):
		w[i] = (w[i]*n)/sum
	return w
# n- kol-vo izvestnih tochek

def get_sum_cell_value(array_grid, array_x, array_y, array_z, i, j, k, value, type ='a'):
	if (type == 'a'):
		# arithmetic average
		array_value = 0
		n = 0
		for x in xrange(len(array_x)):
			i_x, i_y, i_z = array_grid.get_ijk(array_x[x], array_y[x], array_z[x])
			if (i_x == i) and (i_y == j) and (i_z == k):
				array_value = array_value + value[x]
				n = n + 1
		if (n > 0):
			return (array_value / n)
		else:
			return 0
	
	if (type == 'g'):
		# geometric average
		array_value = 1.0
		n = 0
		for x in xrange(len(array_x)):
			i_x, i_y, i_z = array_grid.get_ijk(array_x[x], array_y[x], array_z[x])
			if (i_x == i) and (i_y == j) and (i_z == k):
				array_value = array_value * value[x]
				n = n + 1
		if (n > 0):
			return (array_value ** (1./n) )
		else:
			return 0
	
	if (type == 'h'):
		# harmonic average
		array_value = 0.0
		n = 0.0
		for x in xrange(len(array_x)):
			i_x, i_y, i_z = array_grid.get_ijk(array_x[x], array_y[x], array_z[x])
			if (i_x == i) and (i_y == j) and (i_z == k):
				array_value = array_value + 1. / value[x] 
				n = n + 1
		if (n > 0):
			return (float32 (n) / array_value)
		else:
			return 0