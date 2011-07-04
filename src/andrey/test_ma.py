#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from pylab import *
from geo import *
from pp_geo_routines import *
import time

size_prop = [166, 141, 20]
undefined_value = -99

prop_cont = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC", undefined_value, size_prop)
ik_prop = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", undefined_value ,[0,1], size_prop)

Radiuses = (10, 10, 10)
print "Calculating Moving Average..."
print "Radiuses: ", Radiuses

print "Cont. property..."
time1 = time.time()
MACube_cont = MovingAverage3DP(prop_cont, Radiuses, undefined_value, GetCubicalMask)
time2 = time.time()
print "Done. Time: ", time2 - time1

print "Saving images..."
for k in xrange(MACube_cont.shape[2]):
	imshow(MACube_cont[:,:,k])
	savefig("images/" + "CONT_MA_RSMALL_" + str(k))
print "Done."
stdin.readline()

#print "Ind. property..."
#time1 = time.time()
#MACube_ind = MovingAverage3D(ik_prop, Radiuses, undefined_value)
#time2 = time.time()
#print "Done. Time: ", time2 - time1


