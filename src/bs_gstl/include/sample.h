/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __SAMPLE_H__F14739AA_FF94_421E_9E5B_6DF7AE2F8BDB
#define __SAMPLE_H__F14739AA_FF94_421E_9E5B_6DF7AE2F8BDB

#include <bs_random_generator.h>
#include <typedefs.h>

namespace hpgl
{
	indicator_index_t
		sample(mt_random_generator_t & gen, const std::vector<indicator_probability_t> & probs);

	template <typename Cdf>
	double sample(mt_random_generator_t & gen, const Cdf& f) {
    	return f.inverse(gen());    
	}
}
#endif //__SAMPLE_H__F14739AA_FF94_421E_9E5B_6DF7AE2F8BDB