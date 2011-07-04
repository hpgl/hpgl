/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include <cdf_utils.h>
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
