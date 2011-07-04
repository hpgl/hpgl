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
#	Script for lesson 5.3
#	"Variogram Modeling and Volume Variance"
# ------------------------------------------------

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

#---------------------------------------------------
#	Problem:
#
#	Using classical volume variance relations, determine a reasonable blosk size for geostatistical modeling of this field for a fixed number of 1000 blocks.
#
# ----------------------------------------------------

# Loading sample data from file
dict = load_gslib_file("allwelldata.txt")

# x, y, z size(m)
nx = 454
ny = 454
nz = 40

# property
value = "Por"

# number of cells
i_max = 22
j_max = 22
k_max = 2

# Lets define 3D grid 
array_grid = Grid(min(dict['X']), min(dict['Y']), min(dict['Z']), i_max, j_max, k_max, nx, ny, nz)

prop_ijk = array([])
i_coord = array([])
j_coord = array([])
k_coord = array([])

for i in xrange(i_max):
	for j in xrange(j_max):
		for k in xrange(k_max):
			arithmetic_mean = get_sum_cell_value(array_grid, dict['X'], dict['Y'], dict['Z'], i, j, k, dict[value])
			if (n > 0):
				i_coord = append(i_coord, i)
				j_coord = append(j_coord, j)
				k_coord = append(k_coord, k)
				prop_ijk = append(prop_ijk, arithmetic_mean)

PointSet = {}
PointSet['X'] = i_coord
PointSet['Y'] = j_coord
PointSet['Z'] = k_coord
PointSet['Property'] = prop_ijk

IndicatorData = []
IndicatorData.append(prop_ijk)

Params = {'HardData':IndicatorData}
Function = CalcVariogramFunction

#Suggested Parameters for Variogram
#Azimuth = 0 (Azimut)
#Dip = 0 (Dip)
#Lag Distance = 2 (LagWidth, LagSeparation)
#Horizontal Bandwith = 10 (R2)
#Vertical Bandwith = 3 (R3)
#Number of Lags = 5 (NumLags)

XVariogram, XLagDistance = PointSetScanContStyle(TVVariogramSearchTemplate(
    LagWidth = 2, LagSeparation = 2, TolDistance = 4, NumLags = 5, 
    Ellipsoid = TVEllipsoid(R1 = 10, R2 = 10, R3 = 2, Azimut = 0, Dip = 0, Rotation = 0)
), PointSet, Function, Params)

Variogram_ver = XVariogram[:, 0]
print "XVariogram:"
print Variogram_ver

#Calculate Gammabar
gammab = 0
for i in xrange(len(Variogram_ver)):
	gammab = gammab + Variogram_ver[i]
print "Gammab: ", (gammab / ((nx * ny * nz)**2))

#Variogram modeling results for the vertical direction
figure()
plot(XLagDistance, Variogram_ver, 'bo')
xlabel("Distance")
ylabel("Gamma")
title("Variogram modeling results for the vertical direction")
show()
