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

# ---------------------------------------------------
#	Problem:
#
#	Three vertical wells spaced at 1000 m have been cored and analyzed for porosity at regular 4 m intervals. Calculate and plot the horizontal and vertical semivariograms of porosity from the dataset "3welldata.txt".
#
# ----------------------------------------------------

# Loading sample data from file
dict = load_gslib_file("3welldata.txt")

x_coord = dict['X(Easting)(m)']
z_coord = dict['Z(m)']
poro_values = dict['Por']
y_coord = zeros((len(x_coord)), dtype = float32)

# Lets make a PointSet
PointSet = {}
PointSet['X'] = x_coord
PointSet['Y'] = y_coord
PointSet['Z'] = z_coord
PointSet['Property'] = poro_values

IndicatorData = []
IndicatorData.append(poro_values)

Params = {'HardData':IndicatorData}
Function = CalcVariogramFunction

#Suggested Parameters for Vertical Variogram:

#Azimuth = 0 (Azimut)
#Dip = 90 (Dip)
#Lag Distance = 4 m (LagWidth, LagSeparation)
#Horizontal Bandwith = 0.1 m (R2)
#Vertical Bandwith = 0.1 m (R3)
#Number of Lags = 7 (NumLags)

XVariogram, XLagDistance = PointSetScanContStyle(TVVariogramSearchTemplate(
    LagWidth = 4, LagSeparation = 4, TolDistance = 4, NumLags = 8, 
    Ellipsoid = TVEllipsoid(R1 = 1, R2 = 0.1, R3 = 0.1, Azimut = 0, Dip = 90, Rotation = 0)
), PointSet, Function, Params)

Variogram_x = XVariogram[:, 0]
print "Vertical semivariogram:", Variogram_x

#Experimental vertical semivariogram
figure()
plot(XLagDistance, Variogram_x, 'bo')
plot(XLagDistance, Variogram_x, 'b--')
xlabel("Distance")
ylabel("Gamma")
title("Experimental vertical semivariogram")

#Suggested Parameters for Horizontal Variogram:

#Azimuth = 0 (Azimut)
#Dip = 0 (Dip)
#Lag Distance = 1000 m (LagWidth, LagSeparation)
#Horizontal Bandwith = 0.1 m (R2)
#Vertical Bandwith = 4.5 m (R3)
#Number of Lags = 2(NumLags)

XVariogram, XLagDistance = PointSetScanContStyle(TVVariogramSearchTemplate(
    LagWidth = 1000, LagSeparation = 1000, TolDistance = 1000, NumLags = 3, 
    Ellipsoid = TVEllipsoid(R1 = 1, R2 = 0.1, R3 = 4.5, Azimut = 0, Dip = 0, Rotation = 0)
), PointSet, Function, Params)

Variogram_z = XVariogram[:, 0]
print "Horizontal semovariogram:", Variogram_z

#Experimental horizontal semivariogram
figure()
plot(XLagDistance, Variogram_z, 'bo')
plot(XLagDistance, Variogram_z, 'b--')
xlabel("Distance")
ylabel("Gamma")
title("Experimental horizontal semivariogram")
show()
