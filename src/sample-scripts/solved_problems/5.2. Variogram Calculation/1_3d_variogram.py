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
#	Script for lesson 5.2
#	"Variogram Calculation"
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
#	Calculate directional experimental semivariograms using the full 3D dataset "allwelldata.txt". Note that a subset of this same data vas used in Problem 3.3 Declustering is important and is necessary to establish representative univariate statistics; however, there is no systematic widely accepted approach to decluster a variogram. In general, the data pairs are equally weighted in the calculation of the variogram for a particular lag.
#
# ----------------------------------------------------

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
    LagWidth = 550, LagSeparation = 550, TolDistance = 800, NumLags = 12, 
    Ellipsoid = TVEllipsoid(R1 = 1, R2 = 500, R3 = 5, Azimut = 320, Dip = 0, Rotation = 0)
), PointSet, Function, Params)
#print XVariogram

Variogram_hor1 = XVariogram[:, 0]
print "Horizontal Variogram 1:", Variogram_hor1

#Suggested Parameters for Horizontal Variogram 2.
#Azimuth = 230 (Azimut)
#Dip = 0 (Dip)
#Lag Distance = 550 m (LagWidth, LagSeparation)
#Horizontal Bandwith = 500 m (R2)
#Vertical Bandwith = 5 m (R3)
#Number of Lags = 11 (NumLags)

XVariogram, XLagDistance2 = PointSetScanContStyle(TVVariogramSearchTemplate(
    LagWidth = 550, LagSeparation = 550, TolDistance = 800, NumLags = 12, 
    Ellipsoid = TVEllipsoid(R1 = 1, R2 = 500, R3 = 5, Azimut = 230, Dip = 0, Rotation = 0)
), PointSet, Function, Params)
#print XVariogram

Variogram_hor2 = XVariogram[:, 0]
print "Horizontal Variogram 2:", Variogram_hor2

#Experimental horizontal semivariogram 1 and 2
figure()
plot(XLagDistance1, Variogram_hor1, 'bo')
plot(XLagDistance1, Variogram_hor1, 'b-.')
plot(XLagDistance2, Variogram_hor2, 'bo', color = 'green')
plot(XLagDistance2, Variogram_hor2, 'b--', color = 'green')
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
    LagWidth = 4, LagSeparation = 4, TolDistance = 1.5, NumLags = 11, 
    Ellipsoid = TVEllipsoid(R1 = 1, R2 = 0.1, R3 = 10, Azimut = 0, Dip = 90, Rotation = 0)
), PointSet, Function, Params)
#print XVariogram

Variogram_ver = XVariogram[:, 0]
print "Vertical Variogram:", Variogram_ver

#Experimental vertical semivariogram
figure()
plot(XLagDistance, Variogram_ver, 'bo')
plot(XLagDistance, Variogram_ver, 'b--')
xlabel("Distance")
ylabel("Gamma")
title("Experimental vertical semivariogram")
show()
