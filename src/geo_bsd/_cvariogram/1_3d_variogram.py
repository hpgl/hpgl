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
from geo.variogram import *

import time
import variogram_utils

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

# Lets make a PointSet 
PointSet = {}
PointSet['X'] = require(x_coord, 'float32')
PointSet['Y'] = require(y_coord, 'float32')
PointSet['Z'] = require(z_coord, 'float32')
PointSet['Property'] = require(poro_values, 'float32')

IndicatorData = []
IndicatorData.append(poro_values)

Params = {'HardData':IndicatorData}
Function = CalcVariogramFunction

#Suggested Parameters for Horizontal Variogram 1:

#Azimuth = 320 (Azimut)
#Dip = 0 (Dip)
#Lag Distance = 550 m (LagWidth, LagSeparation)
#Horizontal Bandwith = 500 m (R2)
#Vertical Bandwith = 5 m (R3)
#Number of Lags = 11 (NumLags)

t1 = time.time()
XVariogram, XLagDistance1 = PointSetScanContStyle(
    TVVariogramSearchTemplate(
        LagWidth = 550, 
        LagSeparation = 550, 
        TolDistance = 800, 
        NumLags = 12, 
        Ellipsoid = TVEllipsoid(
            R1 = 1, 
            R2 = 500, 
            R3 = 5, 
            Azimut = 320, 
            Dip = 0, 
            Rotation = 0)
        ), 
    PointSet, 
    Function, 
    Params)

t2 = time.time()

Variogram_hor1 = XVariogram[:, 0]
print "Horizontal Variogram 1:", Variogram_hor1

vhor1 = array(range(12), dtype='float32')

print "Time: %s s." % (t2 - t1)

t1 = time.time()
variogram_utils.CalcVariogramsFromPointSet(
    variogram_utils.VariogramSearchTemplate(
        550.0, 550.0, 800.0, 12, 0,
        variogram_utils.Ellipsoid(1, 500, 5, 320, 0, 0)
        ),
    PointSet,
    vhor1)
t2 = time.time()

print "OR"
print vhor1
print "Time: %s s." % (t2 - t1)

#Suggested Parameters for Horizontal Variogram 2:

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

Variogram_hor2 = XVariogram[:, 0]
print "Horizontal Variogram 2:", Variogram_hor2

vhor2 = array(range(12), dtype='float32')

variogram_utils.CalcVariogramsFromPointSet(
    variogram_utils.VariogramSearchTemplate(
        550.0, 550.0, 800.0, 12, 0,
        variogram_utils.Ellipsoid(1, 500, 5, 230, 0, 0)
        ),
    PointSet,
    vhor2)

print "OR"
print vhor2

#Experimental horizontal semivariogram 1 and 2
figure()
plot(XLagDistance1, Variogram_hor1, 'bo')
plot(XLagDistance1, Variogram_hor1, 'b-.')
plot(XLagDistance2, Variogram_hor2, 'bo', color = 'green')
plot(XLagDistance2, Variogram_hor2, 'b--', color = 'green')
xlabel("Distance")
ylabel("Gamma")
title("Experimental horizontal semivariogram")

#Suggested Parameters for Vertical Variogram:

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

Variogram_ver = XVariogram[:, 0]
print "Vertical Variogram:", Variogram_ver

vver = array(range(11), dtype='float32')
variogram_utils.CalcVariogramsFromPointSet(
    variogram_utils.VariogramSearchTemplate(
        4, 4, 1.5, 11, 0,
        variogram_utils.Ellipsoid(1, 0.1, 10, 0, 90, 0)
        ),
    PointSet,
    vver)

print "OR"
print vver

#Experimental vertical semivariogram
figure()
plot(XLagDistance, Variogram_ver, 'bo')
plot(XLagDistance, Variogram_ver, 'b--')
xlabel("Distance")
ylabel("Gamma")
title("Experimental vertical semivariogram")
show()
