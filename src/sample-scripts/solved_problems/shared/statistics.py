#
#	Solved Problems in Geostatistics
#
# ------------------------------------------------
#	This file includes some general statistics 
#	functions, which are needed for the problems
# ------------------------------------------------
#
#

from numpy import *

# Standartize the array
def stand(z,mean,st_dev):
	z = (z-mean)/st_dev
	return z

# Calculate weighted variance
def w_var(weights, data, w_mean):
	var = 0.0
	weights = norm(weights)
	for i in xrange(len(data)):
		var = var + weights[i]*(data[i]-w_mean)**2
	var = sqrt(var/weights.sum())
	return var

# Calculate weighted mean
def w_mean(weights, data):
	z_1 = 0.0
	z = 0.0
	for i in xrange(len(data)):
		z = z + weights[i]*data[i]
		z_1 = z_1 + weights[i]
	z = z/z_1
	return z

# Calculate standard deviation (sqrt[var])
def calc_quadr_var(array,mean):
	var = 0.0
	for i in xrange(len(array)):
		var = var + ((array[i]-mean)**2)/len(array)
	var = sqrt(var)
	return var

# Calculate mean
def calc_mean_array(array):
	sum = 0.0
	size = len(array)
	for i in xrange(size):
		sum = sum + array[i]
	sum = sum/size
	return sum

# Normalize array
def norm(array):
	sum = 0.0
	for i in xrange(len(array)):
		sum = array[i]+sum
	for i in xrange(len(array)):
		array[i] = array[i]/sum
	return array

# Calculate corellation coefficient
def corr_coef(x,y):
	coef = 0.0
	mean_x = calc_mean_array(x)
	var_x = calc_quadr_var(x,mean_x)
	mean_y = calc_mean_array(y)
	var_y = calc_quadr_var(y,mean_y)
	var = var_x * var_y
	for i in xrange(len(x)):
		coef = coef + x[i]*y[i]
	#print coef
	coef = (coef - len(x)*mean_x*mean_y) / ((len(x)-1)* var)
	return coef

# Make random array from existing with same values
def rand_array(array, n):
	array_rand = zeros( (n), dtype = float)
	for i in xrange(n):
		value = random.randint(len(array))
		array_rand[i] = array[value]
	return array_rand

# Calculate weighted covariance
def calc_cov(x, y, w):
	cov = 0.0
	weigths = 0.0
	for i in xrange(len(x)):
		cov = cov + w[i] * (x[i] - w_mean(w,x)) * (y[i] - w_mean(w,y))
		weigths = weigths + w[i]
	cov = cov / weigths
	return cov

# Calculate weighted correlation coefficient
def w_corr_coef(x, y, w):
	w_coef = calc_cov(x, y, w) / sqrt(calc_cov(x, x, w) * calc_cov(y, y, w))
	return w_coef

# Calculate distance between points
def calc_distance(x1,y1,x2,y2):
	h = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))
	return h

from numpy import mean, std, sum, sqrt, sort, corrcoef, tanh, arctanh
from numpy.random import randint

def bootstrap_correlation(x,y):
	idx = randint(len(x),size=(1000,len(x)))
	bx = x[idx]
	by = y[idx]
	mx = mean(bx,1)
	my = mean(by,1)
	sx = std(bx,1)
	sy = std(by,1)
	r = sort(sum( (bx - mx.repeat(len(x),0).reshape(bx.shape)) *
	(by - my.repeat(len(y),0).reshape(by.shape)), 1)
	/ ((len(x)-1)*sx*sy))
#bootstrap confidence interval (NB! biased)
	conf_interval = (r[25],r[975])
#bootstrap standard error using Fisher's z-transform (NB! biased)
	std_err = tanh(std(arctanh(r))*(len(r)/(len(r)-1.0)))
	return (std_err, conf_interval)