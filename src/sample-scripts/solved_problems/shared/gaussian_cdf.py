#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#


from math import sqrt, log
from numpy import *




# Computes zk such that P(Z<z) = p.
# This function uses a numerical approximation from : Statistical Computing,
# by W.J. Kennedy, Jr. and James E. Gentle, 1980, p. 95.
# @return z such that P(Z<z)=p
# @param p is a real number between 0 and 1 (probability)

def inverse_normal_score(prob, mean = 0.0, var = 1.0):
	Pconst = [ -0.322232431088,-1.0,-0.342242088547, -0.0204231210245,-0.0000453642210148 ];
	Qconst = [ 0.0993484626060,0.588581570495,0.531103462366, 0.103537752850,0.0038560700634 ];
	
	
	if prob >= 1.0:
		return 3
	elif prob <= 0.0:
		return -3
	
	tmp_prob=prob;
	if(prob>0.5):
		tmp_prob=1-prob; 
	
	y = sqrt(log(1.0/(tmp_prob*tmp_prob))) # logarithm is natural
	num=(((y*Pconst[4]+Pconst[3])*y+Pconst[2])*y+Pconst[1])*y+Pconst[0]
	denom=(((y*Qconst[4]+Qconst[3])*y+Qconst[2])*y+Qconst[1])*y+Qconst[0]
	
	result = y+num/denom
	
	if (prob==tmp_prob):
		result = -result
	
	R = result*sqrt(var) + mean
	return R
	
def normal_score(prob):
	a1 = 0.4361836
	a2 = -0.1201676
	a3 = 0.9372980
	p = 0.33267
	z = exp(-(prob**2)/2)/(sqrt(2*pi))
	t = 1./(1 + p*prob)
	P_x = 1 - z*(a1*t + a2*(t**2) + a3*(t**3))
	return P_x
	
	
def gaussian_cdf(value, mean = 0.0, var = 1.0):
	p = 0.2316419
	b = array([0.319381530, -0.356563782, 1.781477937, -1.821255978, 1.330274429])
	sx = (value - mean) / sqrt(var)

	neg = sx < 0

	if (neg == False):
		sx = - sx

	t = 1 / (1 + p * sx)

	zx = 1 / sqrt(2 * 3.14159265358) * exp(- sx * sx / 2)
	result = 1 - zx * ((((b[4] * t + b[3]) * t + b[2]) * t + b[1]) * t + b[0]) * t

	if (neg == False):
		result = 1 - result

	return result


def cdf_transform(array_data, undefined_value):
	array_copy = copy(array_data)
	value = 0.0
	props = array([])
	values = array([])
	defined_values_count = float(sum(array_copy != undefined_value))
	for i in xrange (array_copy.shape[0]):
		for j in xrange (array_copy.shape[1]):
			if(array_copy[i,j] != undefined_value):
				value += float(sum(array_copy == array_copy[i,j])) / defined_values_count
				props = append(props, value)
				values = append(values, array_copy[i,j])
				array_data[i,j] = inverse_normal_score(value)
	return props, values

def back_cdf_transform(property, props, values, undefined_value):
	for i in xrange(property.shape[0]):
		for j in xrange(property.shape[1]):
			for k in xrange(property.shape[2]):
				if(property[i, j, k] != undefined_value):
					property[i, j, k] = interp(gaussian_cdf(property[i,j,k]), props, values)