#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from numpy import *


class Grid:
	def __init__(self, x_min, y_min, dx, dy, l1, l2):
		self.x_min = x_min
		self.y_min = y_min
		self.dx = dx
		self.dy = dy
		self.l1 = l1
		self.l2 = l2
		self.J = 0
		self.data1 = []
		self.data2 = []
		self.x_cell = zeros( (dx+1), dtype = float)
		self.y_cell = zeros( (dy+1), dtype = float)
		for i in xrange(dx+1):
			self.x_cell[i] = self.x_min+l1*i
		for i in xrange(dy+1):
			self.y_cell[i] = self.y_min+l2*i
	
	def get_center_point(self):
		x_centr = (self.x_cell[self.dx]+self.x_min)/2
		y_centr = (self.y_cell[self.dy]+self.y_min)/2
		return x_centr,y_centr
		
	def get_center_points(self):
		self.x_center = zeros( (self.dx), dtype = float)
		self.y_center = zeros( (self.dy), dtype = float)
		self.x_center[0] = self.x_min+self.l1/2
		self.y_center[0] = self.y_min+self.l2/2
		for i in xrange(self.dx):
			self.x_center[i] = self.x_center[0]+self.l1*i
		for i in xrange(self.dy):
			self.y_center[i] = self.y_center[0]+self.l2*i
	
	def get_center_num(self,n):
		for j in xrange(self.dy):
			for i in xrange(self.dx):
				s = self.get_cell_num(self.x_center[i], self.y_center[j])
				if (s == n):
					#print self.x_center[i], self.y_center[j]
					return self.x_center[i], self.y_center[j]
					break
	
	def add_point(self, x, y):
		self.data1.append(x)
		self.data2.append(y)
		self.get_j(x, y)
	
	def get_cell_num(self, x, y):
		i_x = 0
		i_y = 0
		N = 0
		for i in xrange(self.dx):
			if (x>self.x_cell[i]):
				i_x = i
		for i in xrange(self.dy):
			if (y>self.y_cell[i]):
				i_y = i
		N = i_y*self.dx + i_x
		return N
	
	def get_point_num(self, n):
		N = 0
		for i in xrange(len(self.data1)):
			s = self.get_cell_num(self.data1[i], self.data2[i])
			if (s == n):
				N = N + 1
		return N
	
	def get_j(self, x, y):
		n_1 = self.get_cell_num(x,y)
		n = self.get_point_num(n_1)
		if (n == 1):
			self.J = self.J + 1

	def get_weights_cell(self):
		w = zeros( (len(self.data1)), dtype = float)
		n = 0
		n_1 = 0
		#print "J =", self.J
		for i in xrange(len(self.data1)):
			n_1 = self.get_cell_num(self.data1[i],self.data2[i])
			n = self.get_point_num(n_1)
			w[i] = 1./(n*self.J)
		return w
	
	def calc_e_h(self, x_center, y_center, c):
		e_h = 0.0
		for i in xrange(len(self.data1)):
			e_h = e_h + (sqrt((x_center-self.data1[i])**2+(y_center-self.data2[i])**2))**(-c)
		return e_h
	
	def calc_h(self, x_center, y_center, j, c):
		h = (sqrt((x_center-self.data1[j])**2+(y_center-self.data2[j])**2))**(-c)
		return h
	
	def calc_hgt(self, x1, y1, x2, y2):
		h = sqrt((x2-x1)**2 + (y2-y1)**2)
		return h
	
	def hgt_calc(self, x0, y0, n_p):
		h = zeros( (len(self.data1)), dtype = float)
		j = 0
		for i in xrange(len(self.data1)):
			h[j] = self.calc_hgt(x0, y0, self.data1[i], self.data2[i])
			j = j+1
		h = argsort(h)
		h_n = []
		for i in xrange(n_p):
			h_n.append(h[i+1])
		h_n.sort()
		return h_n
	
	def get_weights_idw(self, x_center, y_center, c):
		w1 = zeros( (len(self.data1)), dtype = float)
		e_h = self.calc_e_h(x_center, y_center, c)
		for i in xrange(len(self.data1)):
			h = self.calc_h(x_center, y_center, i, c)
			w1[i] = h/e_h
		return w1
	
	def get_segment_x_middle(self, x1, x2):
		x = 0.0
		x = (x1 + x2)/2
		return x
	
	def get_segment_y_middle(self, y1, y2):
		y = 0.0
		y = (y1 + y2)/2
		return y
	
def get_rect(array_x, array_y):
	min_max = zeros( (4), dtype = float)
	min_max[0] = min(array_x) #x(min)
	min_max[1] = min(array_y) #y(min)
	min_max[2] = max(array_x) #x(max)
	min_max[3] = max(array_y) #y(max)
	
	for i in xrange(2):
		min_max[i] = min_max[i] - 10
		min_max[i+2] = min_max[i+2] + 10
	
	#print min_max
	
	# if ((min_max[2]-min_max[0])>(min_max[3]-min_max[1])):
		# min_max[3] = min_max[2] - min_max[0] + min_max[1]
	# elif ((min_max[2]-min_max[0])<(min_max[3]-min_max[1])):
		# min_max[2] = min_max[3] - min_max[1] + min_max[0]
	return min_max
	
	
def stand_weight(w, n):
	sum = 0.0
	for i in xrange(len(w)):
		sum = sum + w[i]
	for i in xrange(len(w)):
		w[i] = (w[i]*n)/sum
	return w
# n- kol-vo izvestnih tochek


def s_polygon(x, y):
	s = 0.0
	n = len(x)
	for i in xrange(n):
		if (i == 0):
			s = s + x[i]*(y[n-1] - y[i+1])
		elif (i == n-1):
			s = s + x[i]*(y[i-1] - y[0])
		else:
			s = s + x[i]*(y[i-1] - y[i+1])
	s = 0.5 * abs(s)
	return s

	
def s_polygon_1(x, y):
	s = 0.0
	n = len(x)
	for i in xrange(n):
		if (i == n-1):
			s = s + (x[i]*y[0])-(y[i]*x[0])
		else:
			s = s + (x[i]*y[i+1])-(y[i]*x[i+1])
	s = 0.5 * abs(s)
	return s
	
