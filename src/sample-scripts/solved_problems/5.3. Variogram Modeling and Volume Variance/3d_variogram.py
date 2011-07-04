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

#---------------------------------------------------
#	Problem:
#
#	Model the experimental semivariograms from Part 2 of the previous Problem 5.2 using maximum of two nested structures. All directions must be modeled using the same structures and variance contributions for each structure, but each structure may have different range parameters.
#
# ----------------------------------------------------

nugget = 0

sill_hor1 = 15
var_range_hor1 = 4000

sill_hor2 = 20
var_range_hor2 = 5000

sill_ver = 11
var_range_ver = 35

def exp_var(sill, nugget, var_range, h_vect):
	Gamma = zeros((len(h_vect)), dtype = float32)
	for i in xrange(len(h_vect)):
		Gamma[i] = (sill - nugget) * (1 - exp(float(-h_vect[i])*3/(var_range)))
	return Gamma

# Loading sample data from file
dict = load_gslib_file("allwelldata.txt")

x_coord = dict['X']
y_coord = dict['Y']
z_coord = dict['Z']
poro_values = dict['Por']

PointSet = {}
PointSet['X'] = x_coord
PointSet['Y'] = y_coord
PointSet['Z'] = z_coord
PointSet['Property'] = poro_values

IndicatorData = []
IndicatorData.append(poro_values)

Params = {'HardData':IndicatorData}
Function = CalcVariogramFunction

#Suggested Parameters for Horizontal Variogram 1.
#Azimuth = 320 (Azimut)
#Dip = 0 (Dip)
#Lag Distance = 550 m (LagWidth, LagSeparation)
#Horizontal Bandwith = 500 m (R2)
#Vertical Bandwith = 5 m (R3)
#Number of Lags = 11 (NumLags)

XVariogram, XLagDistance1 = PointSetScanContStyle(TVVariogramSearchTemplate(
    LagWidth = 550, LagSeparation = 550, TolDistance = 450, NumLags = 12, 
    Ellipsoid = TVEllipsoid(R1 = 1, R2 = 500, R3 = 5, Azimut = 320, Dip = 0, Rotation = 0)
), PointSet, Function, Params)

Variogram_hor1 = XVariogram[:, 0]
print "Horizontal XVariogram 1:"
print Variogram_hor1

#Suggested Parameters for Horizontal Variogram 2.
#Azimuth = 230 (Azimut)
#Dip = 0 (Dip)
#Lag Distance = 550 m (LagWidth, LagSeparation)
#Horizontal Bandwith = 500 m (R2)
#Vertical Bandwith = 5 m (R3)
#Number of Lags = 11 (NumLags)

XVariogram, XLagDistance2 = PointSetScanContStyle(TVVariogramSearchTemplate(
    LagWidth = 550, LagSeparation = 550, TolDistance = 450, NumLags = 12, 
    Ellipsoid = TVEllipsoid(R1 = 1, R2 = 500, R3 = 5, Azimut = 230, Dip = 0, Rotation = 0)
), PointSet, Function, Params)

Variogram_hor2 = XVariogram[:, 0]
print "Horizontal XVariogram 2:"
print Variogram_hor2

#Calculate Gamma for horizontal semivariogram 1 and 2
Gamma1 = exp_var(sill_hor1, nugget, var_range_hor1, range(min(XLagDistance1), max(XLagDistance1), 1))
print "Gamma for horizontal semivariogram 1: ", Gamma1

Gamma2 = exp_var(sill_hor2, nugget, var_range_hor2, range(min(XLagDistance2), max(XLagDistance2), 1))
print "Gamma for horizontal semivariogram 2: ", Gamma2

#Experimental horizontal semivariogram 1 and 2
figure()
plot(XLagDistance1, Variogram_hor1, 'bo', color = 'blue')
plot(range(min(XLagDistance1), max(XLagDistance1), 1), Gamma1, color = 'blue')
plot(XLagDistance2, Variogram_hor2, 'bo', color = 'green')
plot(range(min(XLagDistance2), max(XLagDistance2), 1), Gamma2, color = 'green')
xlabel("Distance")
ylabel("Gamma")
title("Experimental horizontal semivariogram")

#Suggested Parameters for Vertical Variogram.
#Azimuth = 0 (Azimut)
#Dip = 90 (Dip)
#Lag Distance = 4 m (LagWidth, LagSeparation)
#Horizontal Bandwith = 0.0 m (R2)
#Vertical Bandwith = 10 m (R3)
#Number of Lags = 10 (NumLags)

XVariogram, XLagDistance = PointSetScanContStyle(TVVariogramSearchTemplate(
    LagWidth = 4, LagSeparation = 4, TolDistance = 4, NumLags = 11, 
    Ellipsoid = TVEllipsoid(R1 = 1, R2 = 0.1, R3 = 10, Azimut = 0, Dip = 90, Rotation = 0)
), PointSet, Function, Params)

Variogram_ver = XVariogram[:, 0]
print "Vertical Variogram:"
print Variogram_ver
print "XLagDistance: ", XLagDistance

#Calculate Gamma for vertical semivariogram
Gamma = exp_var(sill_ver, nugget, var_range_ver, range(min(XLagDistance), max(XLagDistance), 1))
print "Gamma for vartical semivariogram: ", Gamma

#Variogram modeling results for the vertical direction
figure()
plot(XLagDistance, Variogram_ver, 'bo')
plot(range(min(XLagDistance), max(XLagDistance), 1), Gamma)
xlabel("Distance")
ylabel("Gamma")
title("Variogram modeling results for the vertical direction")
show()
