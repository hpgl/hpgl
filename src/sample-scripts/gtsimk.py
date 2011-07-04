from sys import *
from geo import *
from numpy import *
from scipy import *
from python_property import *
from gaussian_cdf import *
from copy import *
import os
#gtsim for K indicators

def indicator_transform(prop, indicator):
	for i in xrange(prop.size()):
		if(prop.is_informed(i) == True):
			if (prop.get_at(i)==indicator):
				prop.set_at(i,1)
			else:
				prop.set_at(i,0)
	return prop

def tk_calculation(p):
	value = 0.0
	for i in xrange(p.size()):
		value = inverse_normal_score(p.get_at(i))
		p.set_at(i,value)
	return p

def pseudo_gaussian_transform(prop, tk_prop, indicator):
	for i in xrange(prop.size()):
		for j in xrange(indicator):
			if(prop.is_informed(i) == True):
				if (j==0):
					prop.set_at(i,random.uniform(inverse_normal_score(0.0),tk_prop[j].get_at(i)))
				elif (j==(indicator-1)):
					prop.set_at(i,random.uniform(tk_prop[j-1].get_at(i),inverse_normal_score(1.0)))
				else:
					prop.set_at(i,random.uniform(tk_prop[j-1].get_at(i),tk_prop[j].get_at(i)))
	return prop

def gtsim_Kind(grid, prop, indicator, sk_params = None, pk_prop = None, sgs_params = None):
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
			print "Simple Kriging parameters FOUND, doing SK..."
			for k in xrange(indicator):
				ind_prop = prop.clone()
				ind_prop = indicator_transform(ind_prop,k)
				pk_prop.append(simple_kriging(ind_prop, grid, **sk_params))
	else:
			if type(pk_prop) is hpgl.cont_property_array:
				print "User-defined probability properties FOUND."
			else:
				print "ERROR: WRONG TYPE of user-defined probability properties"
				return
	#write_property(pk_prop[2], "results\GTSIM_SKPROP.INC", "SKPROP", -99)

	# 2. Calculate tk_prop
	print "Calculating Pk..."
	p = []
	for k in xrange(indicator-1):
		if k == 0:
			p.append(pk_prop[0].clone())
		else:
			p.append(hpgl.cont_property_array(prop.size()))

	for i in xrange(indicator-1):
		if (i>0):
			for j in xrange(prop.size()):
				# pi[k] = p1+...+pk
				p[i].set_at(j,pk_prop[i].get_at(j) + p[i-1].get_at(j))
	print "Done."
	print "Calculating threshold curves (tk)..."
	#write_property(pk_prop, "results\GTSIM_PKPROP.INC", "PKPROP", -99)
	del(pk_prop)
	
	tk_prop = []
	for k in xrange(indicator-1):
		tk_prop.append(hpgl.cont_property_array(prop.size()))
	
	for i in xrange(indicator-1):
		tk_prop[i] = tk_calculation(p[i])
	#write_property(tk_prop[1], "results\GTSIM_TKPROP.INC", "TKPROP", -99)
	print "Done."
	
	#3. pseudo gaussian transform of initial property (prop) with tk_prop
	print "Pseudo gaussian transform of initial property (hard data)..."
	prop2 = prop.clone()
	prop2 = pseudo_gaussian_transform(prop,tk_prop,indicator)
	#write_property(prop, "results\GTSIM_TRANSFORMED_PROP.INC", "TRANSPROP", -99)
	print "Done."
	
	# 4. SGS on prop (after transfrom in 3)
		# if sgs_params defined - use it
		# if not, use sk_params
		# sill of covariance must be 1
	print "Starting SGS on transformed property..."
	if (sgs_params == None):
			sgs_params = sk_params
	prop1 = sgs_simulation(prop, grid, seed=3439275, **sgs_params)
	#write_property(prop1, "results\GTSIM_SGS_RESULT.INC", "SGS_RESULT_GT", -99)
	
	# 5. Truncation
	 #				0, 1, .... k-2
	 # tk_prop = [ t1, t2, ... t(k-1) ]
	
	
	print "Truncating SGS result..."
	# for i in xrange(prop1.size()):
		# for j in xrange(indicator-1):
			# if (prop1.get_at(i)>=tk_prop[j].get_at(i)):
				# if j==(indicator-2):
					# prop1.set_at(i,j+1)
				# else:
					# prop1.set_at(i,j)
			# else:
				# if (prop1.get_at(i)<tk_prop[j].get_at(i)):
					# if j==0:
						# prop1.set_at(i,0)
						# break
					# else:
						# prop1.set_at(i,j)

	for i in xrange(prop1.size()):
		for k in xrange(indicator-1):
			if(prop1.get_at(i) <= tk_prop[k].get_at(i)):
				prop1.set_at(i,k)
				break
			else:
				if(prop1.get_at(i) > tk_prop[k].get_at(i)):
					prop1.set_at(i,k+1)
	
	write_property(prop1, "results\GTSIM_TRUNC_RESULT.INC", "TRUNC_RESULT_GT", -99)
	print "Done."
	print "GTSIM: Finished."
	return prop1