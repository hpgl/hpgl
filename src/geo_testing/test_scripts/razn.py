#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from numpy import *
from scipy import *

def calc_razn(x,y,z,array1,array2):
        n = 0
        obshee1 = 0
        obshee2 = 0
        array_diff = zeros( (x,y,z), dtype=float)
        qwadro_diff = 0
        for i in range(x):
                for j in range(y):
                        for k in range(z):
                                obshee1 = obshee1 + array1[i,j,k]
                                obshee2 = obshee2 + array2[i,j,k]
                                array_diff[i,j,k] = array1[i,j,k] - array2[i,j,k] 
        srednee1 = float(obshee1)/float((x*y*z))
        srednee2 = float(obshee2)/float((x*y*z))
        razn = abs(srednee1-srednee2)
        for i in range(x):
                for j in range(y):
                        for k in range(z):
                                if array1[i,j,k] <> array2[i,j,k]:
                                        n = n+1
        m = (float(n)/float(x*y*z))*100
        for i in range(x):
                for j in range(y):
                        for k in range(z):
                                qwadro_diff = array_diff[i,j,k]*array_diff[i,j,k] + qwadro_diff
        qwadro_diff = qwadro_diff/(x*y*z)                                
        print "Mean in Fixed =",srednee1
        print "Mean in Current =",srednee2
        print "Means diff =",razn
        print "Diff points =",m,"%"
        print "Qwadro_diff =", qwadro_diff
        return srednee1, srednee2, razn, qwadro_diff
               
