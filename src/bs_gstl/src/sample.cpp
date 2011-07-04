/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include <sample.h>

namespace hpgl
{
	indicator_index_t
	sample(mt_random_generator_t & gen, const std::vector<indicator_probability_t> & probs)
	{
		double sum = std::accumulate(probs.begin(), probs.end(), 0.0);
		double s = gen() * sum;
		double p = 0;
		for (size_t i = 0, end_i = probs.size(); i < end_i; ++i)
		{
			p += probs[i];
			if (s < p)
				return i;
		}
		return probs.size() - 1;
	}
}