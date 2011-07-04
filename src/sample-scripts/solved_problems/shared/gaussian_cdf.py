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
