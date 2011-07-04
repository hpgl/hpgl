#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

import sys
sys.path.append(r'../shared')

from numpy import *
from geo import *
from matplotlib import *
from pylab import *
from scipy import *
from gslib import *
from variogram_routines import *
from grid_3d import *

# Loading sample data from file
dict = load_gslib_file("allwelldata.txt")

# x, y, z size(m)
nx = [454, 555, 909, 1000, 1666]
ny = [454, 555, 909, 1000, 1666]
nz = [40, 26, 10, 8, 3]

# property
value = "Por"

# number of cells
i_max = [22, 18, 11, 10, 6]
j_max = [22, 18, 11, 10, 6]
k_max = [2, 3, 8, 10, 27]

for q in xrange(5):
	# Lets define 3D grid 
	array_grid = Grid(min(dict['X']), min(dict['Y']), min(dict['Z']), i_max[q], j_max[q], k_max[q], nx[q], ny[q], nz[q])
	print "X,Y size =", nx[q], "Z size =", nz[q]
	print "nx, ny =", i_max[q], "nz =", k_max[q]
	prop_ijk = array([])
	i_coord = array([])
	j_coord = array([])
	k_coord = array([])

	for i in xrange(i_max[q]):
		for j in xrange(j_max[q]):
			for k in xrange(k_max[q]):
				arithmetic_mean = get_sum_cell_value(array_grid, dict['X'], dict['Y'], dict['Z'], i, j, k, dict[value])
				if (arithmetic_mean > 0):
					i_coord = append(i_coord, i)
					j_coord = append(j_coord, j)
					k_coord = append(k_coord, k)
					prop_ijk = append(prop_ijk, arithmetic_mean)
	
	IndicatorData = []
	IndicatorData.append(prop_ijk)
	
	PointSet = {}
	PointSet['X'] = i_coord
	PointSet['Y'] = j_coord
	PointSet['Z'] = k_coord
	PointSet['Property'] = prop_ijk
	
	Params = {'HardData':IndicatorData}
	Function = CalcVariogramFunction
	
	#Suggested Parameters for Variogram
	#Azimuth = 0 (Azimut)
	#Dip = 0 (Dip)
	#Lag Distance = (i_max[q]/2) m (LagWidth, LagSeparation)
	#Horizontal Bandwith = (i_max[q]/2) m (R2)
	#Vertical Bandwith = (k_max[q]/2) m (R3)
	#Number of Lags = 6 (NumLags)
	
	XVariogram, XLagDistance = PointSetScanContStyle(TVVariogramSearchTemplate(
		LagWidth = (i_max[q]/2), LagSeparation = (i_max[q]/2), TolDistance = 10, NumLags = 6, 
		Ellipsoid = TVEllipsoid(R1 = 1, R2 = (i_max[q]/2), R3 = (k_max[q]/2), Azimut = 0, Dip = 0, Rotation = 0)
	), PointSet, Function, Params)

	Variogram_ver = XVariogram[:, 0]
	print "Variogram:"
	print Variogram_ver
	
	#Calculate Gammabar
	gammab = 0
	for i in xrange(len(Variogram_ver)):
		gammab = gammab + Variogram_ver[i]
	#print "Gammab = ", (gammab / ((nx[q] * ny[q] * nz[q])**2))
	print "Gammab = ", gammab

	#Variogram modeling results for the vertical direction
	# figure()
	# plot(XLagDistance, Variogram_ver, 'bo')
	# xlabel("Distance")
	# ylabel("Gamma")
	# title("Variogram modeling results for the vertical direction")
	# show()
