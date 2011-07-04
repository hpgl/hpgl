#
#	Solved Problems in Geostatistics
#
# ------------------------------------------------
#	Script for lesson 4.2
#	"Bootstrap & Spatial Bootstrap"
# ------------------------------------------------

import sys
sys.path.append(r'../shared')

from numpy import *
from geo import *
from decl_grid import *
from statistics import *
from matplotlib import *
from pylab import *
from gslib import *

# ---------------------------------------------------
#	Problem:
#
#	Perform declustering and determine the representative mean of the 2D well-averaged porosity. Also, calculate the correlation between the average porosity and the collocated seismic data (using declustering weights if required). Perform the bootstrap to evaluate the uncertainty in these summary statistics. Perform the bootstrap for the average and the correlation coefficient, respectively. 
#
# ----------------------------------------------------

print "----------------------------------------------------"
print "Loading data & initializing..."

# Loading sample data from file

x = 62
y = 6
n = 62
l = 1000
dx = 7
dy = 8

dict = load_gslib_file("welldata.txt")

array1 = dict['X']
array2 = dict['Y']
array3 = dict['Por']
array4 = dict['Seis']

coef = corr_coef(array3, array4)
print "Correlation between porosity and seismic =", coef

min_max = get_rect(array1, array2)
l1 = (min_max[2] - min_max[0])/dx
l2 = (min_max[3] - min_max[1])/dy

# Lets define 2D grid with dx*dy cells and l1/l2 cells length
array_grid = Grid(min_max[0], min_max[1], dx, dy, l1, l2)

# Add points to 2D grid
for i in xrange(x):
	array_grid.add_point(array1[i], array2[i])

#Cell declustering calculation
w_cell = array_grid.get_weights_cell()

# Weights standardization
w_cell = stand_weight(w_cell, x)

print "Cell declustering"
print w_cell

mean_poro = []
coef_rand = []
mean_seis = w_mean(w_cell, array4)
mean_por = w_mean(w_cell, array3)

for i in xrange(l):
	poro_rand = rand_array(array3, n)
	seismic_rand = rand_array(array4, n)
	mean_por = calc_mean_array(poro_rand)
	mean_poro.append(mean_por)
	coef_rand.append(w_corr_coef(poro_rand, seismic_rand, w_cell))

#std_err, conf_interval = bootstrap_correlation(array3, array4)
#print std_err, conf_interval

#Calculate weightning correlation coefficient
coef = w_corr_coef(array3, array4, w_cell)
print "Weightning correlation coefficient"
print coef

#Draw histogram "Distribution of bootstrapped mean for porosity"
figure()
hist(mean_poro, 50)
xlabel("Porosity (%)")
ylabel("Frequency")
title("Distribution of bootstrapped mean for porosity")

#Draw histogram "The correlation between porosity and collocated seismic"
figure()
hist(coef_rand, 50)
xlabel("Correlation coefficient")
ylabel("Frequency")
title("The correlation between porosity and collocated seismic")

#Draw cross plot with seismic data
figure()
plot(array3, array4, 'bo')
xlabel("Porosity (%)")
ylabel("Seismic")
title("Cross plot with seismic data")
show()
