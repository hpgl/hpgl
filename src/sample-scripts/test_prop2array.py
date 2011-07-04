#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from hpgl_prop_functions import *
from geo import *

#prop = load_cont_property("test_data/NEW_TEST_PROP.INC", -99)
prop = load_ind_property("test_data/NEW_TEST_PROP_01.INC", -99, [0,1])

array_p = prop2array(prop,286,10,1, -99)

prop = array2prop(array_p, -99)

write_property(prop,"results/test_new_prop2array.inc", "test_prop2array" ,-99)
