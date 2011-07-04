/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __BS_RANDOM_GENERATOR_H__E14CD484_0326_4847_A950_AEBF0B62D94D__
#define __BS_RANDOM_GENERATOR_H__E14CD484_0326_4847_A950_AEBF0B62D94D__

#include <boost/random/mersenne_twister.hpp>
#include <typedefs.h>

namespace hpgl
{
	class random_generator_t
	{
	public:
		//virtual void seed(long int seed) = 0;
		virtual long int operator() (long int N) = 0;
	};

	class mt_random_generator_t : public random_generator_t
	{
		boost::mt11213b gen;
	public:
		mt_random_generator_t(){};
		mt_random_generator_t(long int seed)
			: gen((boost::mt11213b::result_type)seed)
		{}
		virtual void seed(long int seed);
		virtual long int operator() (long int N);	
		virtual double operator()();
	};	
}

#endif //__BS_RANDOM_GENERATOR_H__E14CD484_0326_4847_A950_AEBF0B62D94D__