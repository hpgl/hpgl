/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

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
