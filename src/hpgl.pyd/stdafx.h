/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __STDAFX_H__C0C62E4A_23E8_4FD8_9C6C_9361610D7977
#define __STDAFX_H__C0C62E4A_23E8_4FD8_9C6C_9361610D7977

//#define _SECURE_SCL 0

#include <boost/python.hpp>


// Basic Math
#include <GsTL/cdf.h>
//#include <GsTL/sampler.h>
#include <GsTL/univariate_stats.h>

// Geostatistic templates
#include <GsTL/kriging.h>
#include <GsTL/simulation.h>
//#include <GsTL/kriging/kriging_combiner.h>

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

#endif //__STDAFX_H__C0C62E4A_23E8_4FD8_9C6C_9361610D7977