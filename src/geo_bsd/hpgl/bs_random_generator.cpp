#include "stdafx.h"
#include "bs_random_generator.h"

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
