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
import os

def pseudo_gaussian_transform(prop, pk_prop):
        for i in xrange(pk_prop.size()):
                if (prop.get_at(i) == 0):
                        prop.set_at(i,random.uniform(0.0,pk_prop.get_at(i)))
                if (prop.get_at(i) == 1):
                        prop.set_at(i,random.uniform(pk_prop.get_at(i),1.0))
        return prop

def tk_calculation(pk_prop):
	mean = 0
	quad_diff = 1
	values = zeros(pk_prop.size(), dtype=float)             
	for i in xrange(pk_prop.size()):
		values[i] = 1./(quad_diff*sqrt(2*pi))*exp((1./2)*((pk_prop.get_at(i)-mean)/quad_diff)*((pk_prop.get_at(i)-mean)/quad_diff))
	for i in xrange(pk_prop.size()):
		pk_prop.set_at(i,values[i])
	return pk_prop
        
def gtsim_2ind(grid, prop, sk_params, do_sk = True, pk_prop = None, sgs_params = None):
	# prop must be continious!

	# 1. calculate pk_prop
		# check pk_prop, if presented, use it, if not - do SK

        if (pk_prop==None):
                print "Testing SK..."
                pk_prop = simple_kriging(prop, grid, **sk_params)
                print "Done."
        else:
                if type(pk_prop) is hpgl.cont_property_array:
                        hpgl.write_cont_property(pk_prop, filename, prop_name, undefined_value)
                else:
                        print "error"

	# 2. calculate tk_prop
		# t0_prop = 0
		# t1_prop = tk_calculation(pk_prop)
		# (for 2 indicators)

        print "Calculate tk_prop..."
        tk_prop = tk_calculation(pk_prop)
        print "Done."

	# 3. pseudo gaussian transform of initial property (prop) with pk_prop
	# del(pk_prop)

        print "Pseudo gaussian transforming..."
        prop = pseudo_gaussian_transform(prop,pk_prop)
        del(pk_prop)
        print "Done."
	
	# 4. SGS on prop (after transfrom in 3)
		# if sgs_params defined - use it
		# if not, use sk_params
		# sill of covariance must be 1

        if (sgs_params == None):
                sgs_params = sk_params
        print "Testing SGS..."
        prop1 = sgs_simulation(prop, grid, seed=3439275, **sgs_params)
        print "Done."
        write_property(prop1,"2.INC","PROP1",-99)
        
	# 5. Truncation
		# if sgs_result(u) >= tk_prop(u) -> sgs_result(u) = 1
		# if sgs_result(u) < tk_prop(u) -> sgs_result(u) = 0
		
        print "Truncation."
        for i in xrange(prop.size()):
                if (prop1.get_at(i)>=tk_prop.get_at(i)):
                        prop1.set_at(i,1)
                if (prop1.get_at(i)<tk_prop.get_at(i)):
                        prop1.set_at(i,0)
        write_property(prop1,"3.INC","prop1",-99)
        print "Done."
        return prop1
