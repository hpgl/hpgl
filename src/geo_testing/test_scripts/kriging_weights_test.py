#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from geo import *

center_point = [ 10.1, 10.5, 10.102 ]
n_x = [ 9.4, 1, 2, 3, 4 ]
n_y = [ 9.5, 1, 2, 3, 4 ]
n_z = [ 9.1, 1, 2, 3, 4 ]

print simple_kriging_weights(center_point, n_x, n_y, n_z)
