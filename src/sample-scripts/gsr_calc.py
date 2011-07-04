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
from geo import *
from python_property import *
#from matplotlib import *
from numpy import *
from scipy import *
#from pylab import plot

def gsr_calc(prop, x, y, z):
	if type(prop) is hpgl.cont_property_array:
		values_result = zeros([x,y,z], dtype=float)
	else:
		values_result = zeros([x,y,z], dtype=int)
	n = 0
	for k in xrange(z):
		for j in xrange(y):
			for i in xrange(x):
				if(prop.is_informed(n) == True):
					values_result[i,j,k] = prop.get_at(n)
				else:
					values_result[i,j,k] = -99999
				n = n + 1

	gsr = empty( (z) , dtype = float)

	if type(prop) is not hpgl.cont_property_array:
		czeros = 0.0
		cones = 0.0
		for k in xrange(z):
			for i in xrange(x):
				for j in xrange(y):
			                if values_result[i,j,k]==1:
			                    cones=cones+1
			                if values_result[i,j,k]==0:
			                   czeros=czeros+1
			if((cones+czeros) == 0):
				gsr[k] = 0
			else:
				gsr[k]=cones/(cones+czeros)
#			print gsr[k]
			czeros = 0.0
			cones = 0.0
	else:
		mean = 0.0
		num = 0.0
		for k in xrange(z):
			for i in xrange(x):
				for j in xrange(y):
					if(values_result[i,j,k] != -99999):
						mean = mean + values_result[i,j,k]
						num = num + 1
			if(num == 0):
				gsr[k] = 0
			else:
				gsr[k] = mean / num
#			print gsr[k]
			mean = 0.0
			num = 0.0

	return gsr
