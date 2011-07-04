#include "stdafx.h"

#include "cdf_utils.h"
#include "hpgl_exception.h"

namespace hpgl
{

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


} //namespace hpgl
