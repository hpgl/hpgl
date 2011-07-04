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
from statistics import *
from matplotlib import *
from pylab import *
from gslib import *
from scipy import *
from scipy.sparse import *
from scipy.linalg.decomp import *
from gaussian_cdf import *
from scipy.interpolate import *

# ---------------------------------------------------
#	Problem:
#
#	Now consider that the data are spatially correlated and that this correlation is modeled by a spherical variogram with a range of 8500 m in the N-S direction and 6500 m in the E-W direction with a 10% nugget contribution. Mathematically, this correlation model is written in function cov(h), where h is the Euclidean distance vector between data pairs standardized be the range - function get_distance(x1, y1, x2, y2, a_x, a_y)
#
# ----------------------------------------------------
print "----------------------------------------------------"

# -------------------------------------------------------------------
# Spatial Bootstrap functions
# -------------------------------------------------------------------

def cov(h):
	if (h==0):
		return 1.0
	elif (h>1):
		return 0.0
	else:
		return (0.9*(1.0 - 1.5*(h) + 0.5*(h**3)))

def get_distance(x1, y1, x2, y2, a_x, a_y):
	return sqrt( ((x1 - x2)/a_x)**2 + ((y1 - y2)/a_y)**2 )
	
def spatial_bootstrap(x_num, y_num, poro_values, l):
	#print "Building covariance matrix..."

	A = matrix(zeros(shape=(x_num, x_num)))

	# Set covariance values

	for i in xrange(x_num):
		for j in xrange(x_num):
		# Covariance between points
			h = get_distance(x_coord[i], y_coord[i], x_coord[j], y_coord[j], a_x, a_y)
			A[i,j] = cov(h)

#Calculate lower triangular matrix by a Cholesky LU decomposition
	L = cholesky(A, lower = 1, overwrite_a = 0)

#Vector of independent Gaussian values
	w = random.randn(x_num)
	
#Matrix multiplication
	y = dot(L, w)
	
	to_delete = []
	for i in xrange(len(y)):
		if (i>l):
			to_delete.append(i)
	y = delete(y, to_delete)
	
	[x, y1] = histogram(poro_values, bins = 15)

	for i in xrange(len(x)-1):
		x[i+1] = x[i+1] + x[i]
	x_float = zeros( (len(x)), dtype = float)
	
	for i in xrange(len(x)):
		x_float[i] =  float(x[i]) / x_num
	
	y1 = delete(y1, [0])
	
	x_min = x_float.min()

#Calculate p (n by 1 vector of probability values [0,1]) with standard normal distribution
	to_delete = []
	p = zeros( (l), dtype = float)
	for i in xrange(l):
		p[i] = abs(normal_score(y[i]))
		if(p[i] > 1.0):
			to_delete.append(i)
		if( p[i] <  x_min):
			to_delete.append(i)
	
	p = delete(p, to_delete)
	
	z = zeros( (len(p)), order = 'F', dtype = float)

#Interpolation
	int_obj = interp1d(x_float, y1)
	for i in xrange(len(p)):
		z[i] = int_obj(p[i])
	return z
	
# -------------------------------------------------------------------
# Main program
# -------------------------------------------------------------------
print "Loading data & initializing..."

# Loading sample data from file

x_num = 62
y_num = 6

#The range of correlation in the x and y directions
a_x = 8500
a_y = 6500

# Number of realizations
n = 1000
# Number of data to bootstrap
l = 62

dict = load_gslib_file("welldata.txt")

x_coord = dict['X']
y_coord = dict['Y']
poro_values = dict['Por']
seismic_values = dict['Seis']

print "Done."
print "----------------------------------------------------"

print "Number of bootstrapped realizations: ", n
print "Number of points to bootstrap: ", l
print "----------------------------------------------------"
print "Running bootstrap..."

z_mean = []
for i in xrange(n):
	z_mas = spatial_bootstrap(x_num, y_num, poro_values, l)
	z_mean.append(calc_mean_array(z_mas))

print "Done!"
print "Drawing histogram..."

#Draw histogram "Distribution of mean porosity considering spatial correlation between the data"
figure()
hist(z_mean, 15, normed=True)
ylabel("Frequency")
show()
