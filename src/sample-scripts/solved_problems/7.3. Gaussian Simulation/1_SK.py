#
#	Solved Problems in Geostatistics
#
# ------------------------------------------------
#	Script for lesson 7.3
#	"Gaussian Simulation"
# ------------------------------------------------

import sys
sys.path.append(r'../shared')

from numpy import *
from geo import *
from matplotlib import *
from pylab import *
from scipy import *
from gslib import *
from grid_3d import *
from gaussian_cdf import *
from variogram_routines import *
from geo_routines import *

#---------------------------------------------------
#	Problem:
#
#	Perform SK of the data over a domain of size 15,000 * 15,000 m using a discretization of 50 * 50 m blocks using the supplied variogram model. Ensure that the search radius used in the kriging is commensurate with the variogram ranges; a good rule of thumb is to use a search radius that is at least the maximum range of the variogram model. Note that the variogram model given is the variogram of the normal score transformed data, so the kriging should be performed on the transformed data. Back transform the kriged estimates in order to compare the histograms, but retain the normal scores kriged estimates to compare the variograms.
#
# ----------------------------------------------------

# number of cells
i_max = 200
j_max = 200
k_max = 1

# Loading sample data from file
dict = load_gslib_file("DBHdata.txt")

x_coord = dict['X']
y_coord = dict['Y']
z_coord = zeros((len(x_coord)), order = 'F', dtype = 'uint8')

# property
value = "Por"

# Define 3D grid 
grid = SugarboxGrid(i_max, j_max, k_max)

# x, y, z size(m)
nx = 50
ny = 50
nz = 1

# Lets define 3D grid 
array_grid = Grid(0, 0, 0, i_max, j_max, k_max, nx, ny, nz)

prop_ijk = zeros((i_max, j_max, k_max))
prop_ijk = require(prop_ijk, dtype=float32, requirements=['F'])

array_defined = zeros((i_max, j_max, k_max))
array_defined = require(array_defined, dtype=uint8, requirements=['F'])

for i in xrange(i_max):
	for j in xrange(j_max):
		for k in xrange(k_max):
			arithmetic_mean = get_sum_cell_value(array_grid, x_coord, y_coord, z_coord, i, j, k, dict[value])
			if (arithmetic_mean > 0):
				prop_ijk[i,j,k] = arithmetic_mean
				array_defined[i,j,k] = 1
			else:
				prop_ijk[i,j,k] = -99
				array_defined[i,j,k] = 0

initial_data = copy(prop_ijk)

# Transform initial data with cdf_transform function
props, values = cdf_transform(prop_ijk, -99)
transformed_data = prop_ijk

# Generate a property of transformed data for SK and SGS
prop_transformed = (transformed_data, array_defined)

# Generate a property of initial data for SGS
prop_initial = (initial_data, array_defined)

# Show location map of the data
figure()
imshow(prop_transformed[0][:,:,0], vmin = -3, vmax = 3, extent = [0,50,0,50])
title("Location map of the data")

# Historgam of sample data from the data set
figure()
hist(initial_data.compress((initial_data!=-99).flat))
title("Historgam of sample data from the data set")
xlabel("Sampled value")

# Generate a kriged field by performing SK with 315 azimuth direction
variogram1 = CovarianceModel(type=covariance.exponential, ranges = (80, 20, 11), sill = 1, angles = (315, 0, 0))

krigged_transformed_data = simple_kriging(prop = prop_transformed, grid = grid, radiuses = (160, 40, 1), max_neighbours = 20, cov_model = variogram1)
print "SK_transformed_data result:", krigged_transformed_data[0]

#---------------------------------------------------
#	Problem:
#
#	Compute the normal scores semivariograms of the gridded kriged estimates and the simulated values and compare these experimental variograms to the input (Gaussian) variogram model.
#
# --------------------------------------------------

# X, Y, Z, Property = Cube2PointSet(krigged_transformed_data[0], array_defined)

# PointSet = (X, Y, zeros(len(X)))

# IndicatorData = []
# IndicatorData.append(Property)

# Params = {'HardData':IndicatorData}
# Function = CalcVariogramFunction

# XVariogram, XLagDistance1 = PointSetScanGridStyle(TVVariogramSearchTemplate(
    # LagWidth = 20, LagSeparation = 20, TolDistance = 20, NumLags = 12, 
    # Ellipsoid = TVEllipsoid(R1 = 1, R2 = 20, R3 = 1, Azimut = 315, Dip = 0, Rotation = 0)
# ), PointSet, Function, Params)

# Variogram1 = XVariogram[:, 0]
# print "XVariogram:"
# print Variogram1

back_krigged_transformed_data = copy(krigged_transformed_data)
# Back transform to original data space
back_cdf_transform(back_krigged_transformed_data[0], props, values, -99)

# X, Y, Z, Property = Cube2PointSet(back_krigged_transformed_data[0], array_defined)

# PointSet = (X, Y, zeros(len(X)))

# IndicatorData = []
# IndicatorData.append(Property)

# Params = {'HardData':IndicatorData}
# Function = CalcVariogramFunction

# XVariogram, XLagDistance2 = PointSetScanGridStyle(TVVariogramSearchTemplate(
    # LagWidth = 20, LagSeparation = 20, TolDistance = 20, NumLags = 12, 
    # Ellipsoid = TVEllipsoid(R1 = 1, R2 = 20, R3 = 1, Azimut = 315, Dip = 0, Rotation = 0)
# ), PointSet, Function, Params)

# Variogram2 = XVariogram[:, 0]
# print "XVariogram:"
# print Variogram2

# figure ()
# plot(XLagDistance1, Variogram1, 'bo', color = 'blue')
# plot(XLagDistance2, Variogram2, 'bo', color = 'green')
# xlabel("Distance")
# ylabel("Gamma")
# title("Directional semivariogram of kriged points")


#---------------------------------------------------
#	Problem:
#
#	Run a sequential Gaussian simulation program on the same data and domain to generate a single realization. Here too, run one simulation on the normal score transformed data without a back-transform after the simulation to compare the variograms, and run a second simulation on the original data including a back transform to original data space to compare the histograms. Whatever software is used, ensure that the random path between the two realizations in the same (i.e., by using the same random number seed) so that the impact of the path is removed from the comparison. Alternatively, run one sumulation using the normal score transformed data, and back transform the entire realization to original units. In this case. a straight back transform using only the transformation table between the original data units and the normal-scores values is correct.
#
# --------------------------------------------------

# Generate a simulated field by performing SGS on transformed property with 315 azimuth direction
variogram2 = CovarianceModel(type=covariance.spherical, ranges = (80, 20, 1), sill = 1, angles = (315, 0, 0))
sgs_params = {"cov_model": variogram2, "radiuses": (160, 40, 1), "max_neighbours": 20, "force_single_thread": True}
sgs_transformed_data = sgs_simulation(prop_transformed, grid, seed=542783, **sgs_params)
print "SGS_transformed_data result:", sgs_transformed_data[0]

# X, Y, Z, Property = Cube2PointSet(sgs_transformed_data[0], array_defined)

# PointSet = (X, Y, zeros(len(X)))

# IndicatorData = []
# IndicatorData.append(Property)

# Params = {'HardData':IndicatorData}
# Function = CalcVariogramFunction

# XVariogram, XLagDistance3 = PointSetScanGridStyle(TVVariogramSearchTemplate(
    # LagWidth = 20, LagSeparation = 20, TolDistance = 20, NumLags = 12, 
    # Ellipsoid = TVEllipsoid(R1 = 1, R2 = 20, R3 = 1, Azimut = 315, Dip = 0, Rotation = 0)
# ), PointSet, Function, Params)

# Variogram3 = XVariogram[:, 0]
# print "XVariogram:"
# print Variogram3

# Generate a simulated field by performing SGS on initial property with 315 azimuth direction
sgs_initial_data = sgs_simulation(prop_initial, grid, seed=542783, **sgs_params)
print "SGS_initial_data result:", sgs_initial_data[0]

sgs_res = copy(sgs_transformed_data)
# Back transform to original data space
back_cdf_transform(sgs_res[0], props, values, -99)

# X, Y, Z, Property = Cube2PointSet(sgs_res[0], array_defined)

# PointSet = (X, Y, zeros(len(X)))

# IndicatorData = []
# IndicatorData.append(Property)

# Params = {'HardData':IndicatorData}
# Function = CalcVariogramFunction

# XVariogram, XLagDistance4 = PointSetScanGridStyle(TVVariogramSearchTemplate(
    # LagWidth = 20, LagSeparation = 20, TolDistance = 20, NumLags = 12, 
    # Ellipsoid = TVEllipsoid(R1 = 1, R2 = 20, R3 = 1, Azimut = 315, Dip = 0, Rotation = 0)
# ), PointSet, Function, Params)

# Variogram4 = XVariogram[:, 0]
# print "XVariogram:"
# print Variogram4

# figure ()
# plot(XLagDistance3, Variogram3, 'bo', color = 'blue')
# plot(XLagDistance4, Variogram4, 'bo', color = 'green')
# xlabel("Distance")
# ylabel("Gamma")
# title("Directional semivariogram of simulated points")

# Simulated conditional realization in normal-score space
figure()
imshow(sgs_transformed_data[0][:,:,0], vmin = -3, vmax = 3, extent = [0,50,0,50])
title("Simulated conditional realization in normal-score space")

# Gridded kriged estimates in normal-score transformed space
figure()
imshow(krigged_transformed_data[0][:,:,0], vmin =-3, vmax =3, extent = [0,50,0,50])
title("Gridded kriged estimates in normal-score transformed space")

#---------------------------------------------------
#	Problem:
#
#	Plot the histograms of the back transformed kriged estimates and the simulated values and compare these to the histogram of the data. Note the reduction in the variance of the kriged estimates. Also note the shape changes in the histograms.
#
# ----------------------------------------------------

# Gridded kriged estimates
figure()
hist(back_krigged_transformed_data[0])
xlabel("Kriged estimate")
title("Gridded kriged estimates")

# Simulated grid of back tranformed values
figure()
hist(sgs_res[0])
title("Simulated grid of back tranformed values")

# Simulated conditional realization in initial
figure()
imshow(sgs_initial_data[0][:,:,0], vmin = sgs_initial_data[0].min(), vmax = sgs_initial_data[0].max(), extent = [0,50,0,50])
title("Simulated conditional realization in initial")

# Simulated grid of initial values
figure()
hist(sgs_initial_data[0])
title("Simulated grid of initial values")

show()


