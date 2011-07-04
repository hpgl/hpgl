/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include <bs_random_generator.h>

namespace hpgl
{
	void mt_random_generator_t::seed(long int seed)
	{
		gen.seed((boost::mt11213b::result_type)seed);
	}

	long int mt_random_generator_t::operator ()(long int N)
	{
		return (long int) (double(gen()) / double(gen.max BOOST_PREVENT_MACRO_SUBSTITUTION ()) * N);
	}

	double mt_random_generator_t::operator()()
	{
		return double(gen()) / double(gen.max BOOST_PREVENT_MACRO_SUBSTITUTION ());
	}	
}
