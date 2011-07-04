#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from sys import *
from geo import *
from razn import *
from python_property import *
import os
x=166
y=141
z=20

if not os.path.exists("results/"):
	os.mkdir("results/")

print "Loading continuous property..."
prop_cont = load_cont_property("BIG_SOFT_DATA_CON_160_141_20.INC", -99)
print "done"
print "Loading indicator property..."
prop_ind = load_ind_property("test_data/BIG_SOFT_DATA_160_141_20.INC", -99, [0,1])
print "done"

print "Saving continuous property..."
write_property(prop_cont, "results/cont_prop_load_save_test.inc", "CONT_PROP", -99)
del(prop_cont)
print "done"

print "Saving indicator property..."
write_property(prop_ind, "results/ind_prop_load_save_test.inc", "IND_PROP", -99)
del(prop_ind)
print "done"
	
print "Comparing continuous property..."
initial_cont_prop = load_property_python(x,y,z,"results/cont_prop_load_save_test.inc")
saved_cont_prop = load_property_python(x,y,z,"test_data/BIG_SOFT_DATA_CON_160_141_20.INC")
if all(initial_cont_prop==saved_cont_prop):
    print "ok"
else:
    print "error"
	
print "Comparing indicator property..."
initial_ind_prop = load_property_python(x,y,z,"results/ind_prop_load_save_test.inc")
saved_ind_prop = load_property_python(x,y,z,"test_data/BIG_SOFT_DATA_160_141_20.INC")
if all(initial_ind_prop==saved_ind_prop):
    print "ok"
else:
    print "error"
    
