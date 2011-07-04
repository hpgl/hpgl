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
from numpy import *
from scipy import *
from sys import *

def load_property_python(x,y,z,filename,intype=False):
        values = []
        if intype == False:
                intype = int
        elif intype == True:
                intype = float
        values_right = zeros( (x,y,z), dtype=intype)
        f = open(filename)
        for line in f:
                if "--" in line:
                        line = line[+1]
                ss = line.split()
                for s in ss:
                        try:
                               values += [intype(s.strip())]
                        except:
                               pass
        values = array(values).reshape(z,y,x)

        for i in range(x):
                for j in range(y):
                        for k in range(z):
                                values_right[i,j,k] = values[k,j,i]
                                             
        return values_right

def save_property_python(prop_array,x,y,z,filename,cube_name = "CUBE"):
        f = open(filename, "w+")
        f.write(cube_name)
        prop_array = array(prop_array).reshape(x*y*z)
        for i in range(x*y*z):
                if mod(i,12) == 0:
                        f.write("\n")
                f.write(str(prop_array[i]))
                f.write(" ")
        if mod(x*y*z,12)>=0:
                f.write("\n")
        f.write("/")
        f.close()
              
