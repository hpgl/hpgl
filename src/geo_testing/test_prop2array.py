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

PP = load_property_python(166,141,225,"test_data/BIG_HARD_DATA.INC",intype=int)
prop = load_cont_property("test_data/BIG_HARD_DATA.INC",-99)
PC = prop2array(166,141,225,prop)
if all(PP==PC):
    print "ok"
else:
    print "error"

