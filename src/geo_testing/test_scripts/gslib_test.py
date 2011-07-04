#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from geo import *
from sys import *
from razn import *
from python_property import *
import os

if not os.path.exists("results/"):
	os.mkdir("results/")

#prop = load_ind_property("test_data/gslib_ind_pattern.inc", -99, [0,1])
#prop_cont = load_cont_property("test_data/gslib_pattern.inc", -99)
#write_property(prop, "test_data/gslib_ind_pattern.inc", "GSLIB_PROP_IND_P", -99);
#write_property(prop_cont, "test_data/gslib_pattern.inc", "GSLIB_PROP_P", -99);
	
#print "Loading Indicator GSLIB Property..."
#prop1 = load_gslib_ind_property("test_data\input_gslib_small.inc", -99, [0,1], 1,1,4)
#print 'ok'

#print "Loading Continious GSLIB Property..."
#prop2 = load_gslib_cont_property("test_data\input_gslib_small_cont.inc", -99, 1,1,4)
#print 'ok'

print "Loading BIG Indicator GSLIB Property..."
prop3 = load_gslib_ind_property("test_data\input_gslib_ind.inc", -99, [0,1], 166,141,20)
print 'ok'

print "Loading BIG Continious GSLIB Property..."
prop4 = load_gslib_cont_property("test_data\input_gslib.inc", -99, 166,141,20)
print 'ok'


print "Saving results ..."
#write_property(prop1, "results/gslib_small_output.inc", "SMALL_GSLIB", -99);
#write_property(prop2, "results/gslib_small_cont_output.inc", "SMALL_GSLIB_CONT", -99);
write_property(prop4, "results/gslib_output_eclipse.inc", "GSLIB_PROP", -99);
write_property(prop3, "results/gslib_output_ind_eclipse.inc", "GSLIB_PROP_IND", -99);

write_gslib_property(prop4, "results/gslib_output.inc", "GSLIB_PROP", -99);
write_gslib_property(prop3, "results/gslib_output_ind.inc", "GSLIB_PROP_IND", -99);

print "ok"