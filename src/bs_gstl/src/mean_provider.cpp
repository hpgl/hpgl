/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include "mean_provider.h"
#include "hpgl_exception.h"

namespace hpgl
{
	void create_means(
			const std::vector<indicator_probability_t> & marginal_probs, 
			std::vector<boost::shared_ptr<single_mean_t> > & means)
	{
		for (indicator_index_t i = 0; i < marginal_probs.size(); ++i)
			means.push_back(
				boost::shared_ptr<single_mean_t>(
					new single_mean_t(marginal_probs[i])));
	}
		
}
