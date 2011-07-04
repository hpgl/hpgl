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
from hpgl_prop_functions import *
from geo import *

#prop = load_cont_property("test_data/NEW_TEST_PROP.INC", -99)
prop = load_ind_property("test_data/NEW_TEST_PROP_01.INC", -99, [0,1])

array_p = prop2array(prop,286,10,1, -99)

prop = array2prop(array_p, -99)

write_property(prop,"results/test_new_prop2array.inc", "test_prop2array" ,-99)
