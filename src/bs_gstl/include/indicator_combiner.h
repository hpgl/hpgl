/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef INDICATOR_COMBINER_H_INCLUDED_SDA34R5T6Y7UMNRETYUNOIUFGYUI8O9OIJVBFGYUO0I9JUHBGVFCRTFGYHU7J
#define INDICATOR_COMBINER_H_INCLUDED_SDA34R5T6Y7UMNRETYUNOIUFGYUI8O9OIJVBFGYUO0I9JUHBGVFCRTFGYHU7J

#include <mean_provider.h>
#include "combiner.h"

namespace hpgl
{
	inline indicator_probability_t ind_combine(		
		const std::vector<indicator_value_t> & values,
		const std::vector<kriging_weight_t> & weights,
		const std::vector<mean_t> & marginal_probs,
		mean_t center_marginal_prob)
	{
		return combine<indicator_value_t, indicator_probability_t>
			(values, weights, marginal_probs, center_marginal_prob);		
	}
}

#endif 
