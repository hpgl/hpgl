#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from geo import *
from python_property import *
from matplotlib import *
from numpy import *
from scipy import *
from pylab import *

def ntg_calc_hist(x,y,z,n,sis_prop):
    print "Creating Grid... "
    grid = SugarboxGrid(x,y,z)
    print "Done.\n"
    print "Loading property... "
    sis_prop = load_ind_property("NEW_TEST_PROP_01.INC",-99,[0,1])
    print "Done.\n"
    ntg = empty( (n) )
    for c in xrange(n):
        print "Creating SIS params... "
        sis_data = [    {
                        "cov_type":0,
                        "ranges":(10,10,10),
                        "sill":0.4,
                        "radiuses":(10,10,10),
                        "max_neighbours":12,
                        "marginal_prob":0.5,
                        "value":0
                    },
                     {
                        "cov_type":0,
                        "ranges":(10,10,10),
                        "sill":0.4,
                        "radiuses":(10,10,10),
                        "max_neighbours":12,
                        "marginal_prob":0.5,
                        "value":1
                    }]
        print "Done.\n"
        sis_result = sis_simulation(sis_prop, grid, sis_data, seed=3141347-1000*c+500, use_vpc = False)
        write_property(sis_result, "RESULT.INC", "S_RESULT", -99)
        values_result = load_property_python(x,y,z,"RESULT.INC",intype)

        zeros = 0.0
        ones = 0.0
        
        for i in xrange(x):
            for j in xrange (y):
                for k in xrange(z):
                    if values_result[i,j,k]==1:
                        ones=ones+1
                    if values_result[i,j,k]==0:
                        zeros=zeros+1
        ntg[c]=ones/(ones+zeros)
        print ntg[c]
        del(sis_result)
    hist(ntg, histtype='bar', orientation='vertical')
    show()
    



                    

