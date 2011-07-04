#
#
#   Copyright 2009 HPGL Team
#
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.
#
#   HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.
#
# GTSIM for 2 indicators (facies)
from sys import *
from geo import *
from numpy import *
from scipy import *
from python_property import *
from gaussian_cdf import *
import os

def pseudo_gaussian_transform(prop, tk_prop):
	pg_prop = prop.clone()
	
	for i in xrange(tk_prop.size()):
		if (pg_prop.get_at(i) == 0):
			pg_prop.set_at(i,random.uniform(inverse_normal_score(0.0),tk_prop.get_at(i)))
		if (pg_prop.get_at(i) == 1):
			pg_prop.set_at(i,random.uniform(tk_prop.get_at(i),inverse_normal_score(1.0)))
	
	return pg_prop


def tk_calculation(pk_prop):
	tk_prop = pk_prop.clone()
	value = 0.0
	for i in xrange(tk_prop.size()):
		value = inverse_normal_score(tk_prop.get_at(i))
		tk_prop.set_at(i,value)
	return tk_prop


def gtsim_2ind(grid, prop, sk_params = None, sgs_params = None, pk_prop = None):
	# prop must be continious!

	print "Starting GTSIM for 2 Indicator variables..."

	# 1. calculate pk_prop
	# check pk_prop, if presented, use it, if not - do SK
	
	print "Extracting probability information..."
	
	if (pk_prop==None):
			print "User-defined probability properties NOT FOUND."
			if (sk_params==None):
				print "Simple Kriging parameters NOT FOUND."
				print "ERROR: Cannot retrieve probability information."
				return
			print "Simple Kriging parameters FOUND, doing SK..."
			pk_prop = simple_kriging(prop, grid, **sk_params)
	else:
			if type(pk_prop) is hpgl.cont_property_array:
				print "User-defined probability properties FOUND."
			else:
				print "ERROR: WRONG TYPE of user-defined probability properties"
				return

	# 2. calculate tk_prop
		# t0_prop = 0
		# t1_prop = tk_calculation(pk_prop)
		# (for 2 indicators)

	print "Calculating threshold curves (tk)..."
	write_property(pk_prop, "results\GTSIM_PKPROP.INC", "PKPROP", -99)
	tk_prop = tk_calculation(pk_prop)
	write_property(tk_prop, "results\GTSIM_TKPROP.INC", "TKPROP", -99)
	print "Done."

	# 3. pseudo gaussian transform of initial property (prop) with pk_prop
	# del(pk_prop)

	print "Pseudo gaussian transform of initial property (hard data)..."
	prop_pg = pseudo_gaussian_transform(prop,tk_prop)
	write_property(prop, "results\GTSIM_TRANSFORMED_PROP.INC", "TRANSPROP", -99)
	del(pk_prop)
	print "Done."
	
	# 4. SGS on prop (after transfrom in 3)
		# if sgs_params defined - use it
		# if not, use sk_params
		# sill of covariance must be 1
	print "Starting SGS on transformed property..."
	if (sgs_params == None):
			sgs_params = sk_params
	prop_sgs = sgs_simulation(prop_pg, grid, seed=3439275, **sgs_params)
	#write_property(prop1, "results\GTSIM_SGS_RESULT.INC", "SGS_RESULT_GT", -99)
		
	# 5. Truncation
		# if sgs_result(u) >= tk_prop(u) -> sgs_result(u) = 1
		# if sgs_result(u) < tk_prop(u) -> sgs_result(u) = 0
		
	print "Truncating SGS result..."
	for i in xrange(prop_sgs.size()):
		if (prop_sgs.get_at(i)<tk_prop.get_at(i)):
			prop_sgs.set_at(i,0)
		if (prop_sgs.get_at(i)>=tk_prop.get_at(i)):
			prop_sgs.set_at(i,1)
	print "Done."
	print "GTSIM: Finished."
	return prop_sgs
