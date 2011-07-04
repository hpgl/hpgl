from geo import *
from geo.routines import *
from matplotlib import *
from sys import *
from pylab import *

import os
import time
import pylab

size = (166, 141, 20)

print "loading image..."
data_3D = load_ind_property("BIG_SOFT_DATA_160_141_20.INC", -99, [0,1], size)
 
data = data_3D[0][:,:,0]
mask = data_3D[1][:,:,0]

figure()
pylab.imshow(data[:,:], vmin = 0, vmax = 2)
pylab.savefig("hard_data")

#initial_data = copy(data)

# test with 98% of harddata
#n = 0.995

# for i in xrange(size[0]):
	# for j in xrange(size[1]):
		# value = numpy.random.uniform()
		# if (value < n):
			# mask[i,j] = 0
			# data[i,j] = -99
			# initial_data[i,j] = -99

# for i in xrange(size[0]):
	# for j in xrange(size[1]):
		# if (mask[i,j] <> 0):
			# if (initial_data[i,j] <> -99):
				# initial_data[i,j] = float32(numpy.random.normal(4.0, 1.0))

# for i in xrange(size[0]):
	# for j in xrange(size[1]):
		# if (initial_data[i,j] <> -99):
			# initial_data[i,j] = abs(initial_data[i,j,0]) / initial_data.max()

prop = (data, mask, 2)
write_property(prop, "IND_data.INC", "Ind_data", -99)
# write_property((initial_data, mask), "CONT_data.INC", "Cont_data", -99)
