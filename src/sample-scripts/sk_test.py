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

def sk_calc(x,y,z,prop1):
    print "Creating Grid... "
    grid = SugarboxGrid(x,y,z)
    print "Done.\n"
    print "Loading property... "
    prop1 = load_cont_property("CUB.INC",-99)
    print "Done.\n"
    i = -1
    n = 60
    while n>10:
        i = i+1
        prop_result = simple_kriging(prop1, grid,
                                     radiuses = (20,20,20),
                                     max_neighbours = n,
                                     covariance_type = covariance.exponential,
                                     ranges = (10,10,10),
                                     sill = 1,
                                     mean = 0.487)
        write_property(prop_result,"RESULT_SK"+str(i)+".INC","SK_RESULT"+str(i), -99)
        values_result = load_property_python(x,y,z,"RESULT_SK"+str(i)+".INC",intype)
        save_property_python(values_result,x,y,z,"RES"+str(i)+".INC")
        n = n-1
    razn = zeros( (i) )
    max_n = load_property_python(x,y,z,"RES0.INC",intype)
    for j in range(i):   
        min_n = load_property_python(x,y,z,"RES"+str(i-j)+".INC",intype)
        for a in range(x):
            for b in range(y):
                for c in range(z):
                    razn[j] = max_n[a,b,c] - min_n[a,b,c] + razn[j]
        razn[j] = abs(razn[j])
    print razn
    mas = zeros( (i) )
    for f in range(i):
        n = n+1
        mas[f] = mas[f] + n
    plot(mas,razn)
    ylabel("D")
    xlabel("max neighbours")
    show()
