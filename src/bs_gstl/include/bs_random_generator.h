/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

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