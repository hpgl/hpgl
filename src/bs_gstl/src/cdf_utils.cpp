/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include <cdf_utils.h>
#include "hpgl_exception.h"

namespace hpgl
{

indicator_index_t most_probable_category(const Categ_non_param_cdf<> & cdf, size_t size )
{
	double max_prob = cdf.prob(0);
	int max_idx = 0;
	for (size_t i = 1; i < size; i++)
	{
		double prob = cdf.prob(i);
		if (prob > max_prob)
		{
			max_prob = prob;
			max_idx = i;
		}
	}
	return max_idx;
}

indicator_index_t most_probable_category(const std::vector<indicator_probability_t> & cdf)
{
	size_t size = cdf.size();	
	double max_prob = cdf[0];
	indicator_index_t max_idx = 0;
	for (indicator_index_t i = 1; i < size; i++)
	{
		double prob = cdf[i];
		if (prob > max_prob)
		{
			max_prob = prob;
			max_idx = i;
		}
	}
	return max_idx;
}

void build_cdf_from_property(const property_array_t<cont_value_t> & property, Non_param_cdf<> & cdf)
{
	std::vector<double> hard_data; 
	for (int idx = 0; idx < property.size(); ++idx)
	{
		if (property.is_informed(idx))
		{			
			hard_data.push_back(property.get_at(idx)); 	
		} 
	}

	if (hard_data.size() == 0)
		throw hpgl_exception("build_cdf_from_property", "Can't build cdf from harddataless property.");

	build_cdf(hard_data.begin(), hard_data.end(), cdf);
}


} //namespace hpgl
