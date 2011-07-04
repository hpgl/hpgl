#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from geo import *

from numpy import *
#from scipy import *
from pylab import *

from gsr_calc import *

x = 166
y = 141
z = 20

prop = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC",-99,[0,1])
gsr_prop = gsr_calc(prop,x,y,z)

#print gsr_soft_ind

#prop = load_cont_property("test_data/NRAND_PORO.INC",-99)
#gsr_poro_nrand = gsr_calc(prop,x,y,z)

#prop = load_cont_property("test_data/BIG_SOFT_DATA_CON_160_141_20.INC",-99)
#gsr_soft_con = gsr_calc(prop,x,y,z)


grid = SugarboxGrid(166, 141, 20)

ik_data =  [    {
                         "cov_type": 0,
                         "ranges": (10, 10, 10),
                         'sill': 1,
                         "radiuses": (20, 20, 20),
                         "max_neighbours": 12,
                         "marginal_prob": 0.5,
                         "value": 0.8
                 },
                 {
                         "cov_type": 0,
                         "ranges": (10, 10, 10),
                         "sill": 1,
                         "radiuses": (20, 20, 20),
                         "max_neighbours": 12,
                         "marginal_prob": 0.2,
                         "value": 1
                 }]

sis_result = sis_simulation(prop, grid, ik_data, seed=3241347, use_corellogram = False)
gsr_sis = gsr_calc(sis_result,x,y,z)

print "GSR on Prop:"
print gsr_prop 
#plot(gsr_prop)
print "GSR on SIS result:"
print gsr_sis
#plot(gsr_sis)
print "Diff:"
diff = gsr_prop - gsr_sis
print diff
#show()
