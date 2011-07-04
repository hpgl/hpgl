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
from geo import *

center_point = [ 10.1, 10.5, 10.102 ]
n_x = [ 9.4, 1, 2, 3, 4 ]
n_y = [ 9.5, 1, 2, 3, 4 ]
n_z = [ 9.1, 1, 2, 3, 4 ]

print simple_kriging_weights(center_point, n_x, n_y, n_z)
