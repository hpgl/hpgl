#include "stdafx.h"

#include "mean_provider.h"
#include "hpgl_exception.h"

namespace hpgl
{
	void create_means(
			const std::vector<indicator_probability_t> & marginal_probs, 
			std::vector<single_mean_t> & means)
	{
		for (indicator_index_t i = 0; i < marginal_probs.size(); ++i)
			means.push_back(single_mean_t(marginal_probs[i]));
	}
		
}
