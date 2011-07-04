from numpy import *
from geo import *

def prop2array(prop,x,y,z, undefined_value):
	if(prop.size() != (x*y*z)):
		print "Error! Property size is: ", prop.size(), "but given x*y*z size is: ",x*y*z
		return
	if type(prop) is hpgl.cont_property_array:
		values = zeros(prop.size(), dtype=float64)
		values_right = zeros( (x,y,z), dtype=float64)
	else:
		values = zeros(prop.size(), dtype=int32)
		values_right = zeros( (x,y,z), dtype=int32)
	
	for i in xrange(prop.size()):
		if(prop.is_informed(i) == True):
			values[i]=prop.get_at(i)
		else:
			values[i] = undefined_value
	
	values = array(values).reshape(z,y,x)

	for i in range(x):
		for j in range(y):
			for k in range(z):
				values_right[i,j,k] = values[k,j,i]

	return values_right
	
def array2prop(array_prop, undefined_value):
	x = size(array_prop,0)
	y = size(array_prop,1)
	z = size(array_prop,2)
	
	if array_prop.dtype is not dtype('int32'):
		print "Array is float, creating continious property..."
		prop = hpgl.cont_property_array(array_prop.size)
		print "Done"
	else:
		print "Array is int, creating indicator property..."
		indicators = unique(array_prop)
		indicators_t = []
		for k in xrange(indicators.size):
			if(indicators[k] != undefined_value):
				indicators_t.append(indicators[k])
		print "Indicators is: ", indicators_t
		prop = hpgl.byte_property_array(array_prop.size, indicators_t )
		print "Done"
	
	values_for_prop = zeros( (z,y,x), dtype=array_prop.dtype)
	
	for i in range(x):
		for j in range(y):
			for k in range(z):
				values_for_prop[k,j,i] = array_prop[i,j,k]
	
	size_p = x*y*z
	values_for_prop = values_for_prop.reshape(size_p)
	
	for k in xrange(size_p):
		if(values_for_prop[k] != undefined_value):
			prop.set_at(k,values_for_prop[k])
	return prop
	
