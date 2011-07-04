/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#define WIN32_LEAN_AND_MEAN
//#define _SECURE_SCL 0



// Basic Math
#include <GsTL/cdf.h>
//#include <GsTL/sampler.h>
#include <GsTL/univariate_stats.h>
#include <GsTL/cdf/non_param_cdf.h>
#include <GsTL/geometry/geometry.h>
#include <GsTL/matrix_library/matrix_lib_traits.h>
#include <GsTL/matrix_library/gstl_tnt_lib.h>
//#include <GsTL/kriging/kriging_constraints_traits.h>
#include <GsTL/kriging/helper_functions.h>

// Geostatistic templates
//#include <GsTL/kriging.h>
//#include <GsTL/simulation.h>

// Misc stuff
#include <GsTL/geometry/covariance.h>

#include <vector>
#include <deque>
#include <list>
#include <string>
#include <iostream>
#include <time.h>
#include <fstream>
#include <ostream>
#include <sstream>
#include <exception>
#include <boost/smart_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/format.hpp>

#include "setup_plugin_api.h"