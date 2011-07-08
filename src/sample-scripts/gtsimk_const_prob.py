from sys import *
from geo import *
from numpy import *
from scipy import *
from python_property import *
from gaussian_cdf import *
from copy import *
import os
#gtsim for K indicators

def mean_ind(prop_init,indicator):
	prop = prop_init.clone()
	ind_size = zeros( (indicator-1), dtype=float)
	prop_size = 0.0
	for j in xrange(prop.size()):
		if(prop.is_informed(j) == True):
			prop_size = prop_size + 1.0
			for i in xrange(indicator-1):
				if (prop.get_at(j) == i):
					ind_size[i] = ind_size[i] + 1.0
	for i in xrange(indicator-1):
		ind_size[i] = ind_size[i]/prop_size
	return ind_size

def calc_ver(pk_prop,indicator):
	s = zeros( (indicator-1), dtype=float)
	for i in xrange(indicator-1):
		if (i>0):
			s[i] = pk_prop[i] + s[i-1]
		else:
			s[i] = pk_prop[i]
	return s

def indicator_transform(prop_init, indicator):
	prop = prop_init.clone()
	for i in xrange(prop.size()):
		if(prop.is_informed(i) == True):
			if (prop.get_at(i)==indicator):
				prop.set_at(i,1)
			else:
				prop.set_at(i,0)
	return prop

def tk_calculation(p):
	value = 0.0
	value = inverse_normal_score(p)
	p= value
	return p

def pseudo_gaussian_transform(prop_init, tk_prop, indicator):
	prop = prop_init.clone()
	for i in xrange(prop.size()):
		#for j in xrange(indicator):
		if(prop.is_informed(i) == True):
			if (prop.get_at(i)==0):
				v = random.uniform(inverse_normal_score(0.0),tk_prop[prop.get_at(i)])
				prop.set_at(i,v)
				#print "Found ,", j, " setting ", v, "from range [", inverse_normal_score(0.0), ",", tk_prop[j], ")"
				#print "Found ,", j, " setting ", prop.get_at(i), "from range [", inverse_normal_score(0.0), ",", tk_prop[j], ")"
			elif (prop.get_at(i)==(indicator-1)):
				v = random.uniform(tk_prop[prop.get_at(i)-1],inverse_normal_score(1.0))
				prop.set_at(i,v)
				#print "Found ,", j, " setting ", v, "from range [", tk_prop[j-1], ",", inverse_normal_score(1.0), ")"
				#print "Found ,", j, " setting ", prop.get_at(i), "from range [", tk_prop[j-1], ",", inverse_normal_score(1.0), ")"
			else:
				v = random.uniform(tk_prop[prop.get_at(i)-1],tk_prop[prop.get_at(i)])
				prop.set_at(i,v)
				#print "Found ,", j, " setting ", v, "from range [", tk_prop[j-1], ",", tk_prop[j], ")"
				#print "Found ,", j, " setting ", prop.get_at(i), "from range [", tk_prop[j-1], ",", tk_prop[j], ")"
	write_property(prop, "results\GTSIM_TRANSFORMED_PROP.INC", "TRANSPROP", -99)
	return prop

def gtsim_Kind_const_prop(grid, prop, indicator, sk_params = None, pk_prop = None, sgs_params = None):
	# prop must be continious!

	print "Starting GTSIM for K Indicator variables..."

	# 1. calculate pk_prop
	# check pk_prop, if presented, use it, if not - do SK
	
	print "Extracting probability information..."
	
	if (pk_prop==None):
			print "User-defined probability properties NOT FOUND."
			pk_prop = []
			if (sk_params==None):
				print "Simple Kriging parameters NOT FOUND."
				print "ERROR: Cannot retrieve probability information."
				return
			print "Calculating pk_prop..."
			#print "Simple Kriging parameters FOUND, doing SK..."
			#for k in xrange(indicator-1):
			#	ind_prop = prop.clone()
			#	ind_prop = indicator_transform(ind_prop,k)
			#	pk_prop.append(simple_kriging(ind_prop, grid, **sk_params))
			pk_prop = mean_ind(prop,indicator)
	else:
			if type(pk_prop) is hpgl.cont_property_array:
				print "User-defined probability properties FOUND."
			else:
				print "ERROR: WRONG TYPE of user-defined probability properties"
				return
	#write_property(pk_prop[1], "results\GTSIM_SKPROP.INC", "SKPROP", -99)
	print pk_prop

	# 2. Calculate tk_prop
	print "Calculating Pk..."
	p = calc_ver(pk_prop,indicator)
	print p
	print "Done."
	print "Calculating threshold curves (tk)..."
	#write_property(pk_prop, "results\GTSIM_PKPROP.INC", "PKPROP", -99)
	del(pk_prop)
	
	tk_prop = zeros( (indicator-1), dtype=float)
	
	for i in xrange(indicator-1):
		tk_prop[i] = tk_calculation(p[i])
	print tk_prop
	#write_property(tk_prop[1], "results\GTSIM_TKPROP.INC", "TKPROP", -99)
	print "Done."
	
	#3. pseudo gaussian transform of initial property (prop) with tk_prop
	print "Pseudo gaussian transform of initial property (hard data)..."
	prop1 = pseudo_gaussian_transform(prop,tk_prop,indicator)
	#write_property(prop1, "results\GTSIM_TRANSFORMED_PROP.INC", "TRANSPROP", -99)
	print "Done."
	
	# 4. SGS on prop (after transfrom in 3)
		# if sgs_params defined - use it
		# if not, use sk_params
		# sill of covariance must be 1
	print "Starting SGS on transformed property..."
	
	if (sgs_params == None):
			sgs_params = sk_params
	prop1 = sgs_simulation(prop1, grid, seed=3439275, **sgs_params)
	write_property(prop1, "results\GTSIM_SGS_RESULT.INC", "SGS_RESULT_GT", -99)
	
	# 5. Truncation
	 #				0, 1, .... k-2
	 # tk_prop = [ t1, t2, ... t(k-1) ]
	
	print "Truncating SGS result..."
	
	for i in xrange(prop1.size()):
		for k in xrange(indicator-1):
			if(prop1.get_at(i) < tk_prop[k]):
				prop1.set_at(i,k)
				break
			else:
				if(k == (indicator-2)):
					prop1.set_at(i,k+1)
	
	write_property(prop1, "results\GTSIM_TRUNC_RESULT.INC", "TRUNC_RESULT_GT", -99)
	print "Done."
	print "GTSIM: Finished."
	#return prop1