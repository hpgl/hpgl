/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __RANDOM_NODE_ENUMERATOR_H__89D32835_1CF0_4966_BA62_6F0C4B0FBE43__
#define __RANDOM_NODE_ENUMERATOR_H__89D32835_1CF0_4966_BA62_6F0C4B0FBE43__

#include <boost/smart_ptr.hpp>


#include <property_array.h>
#include <sugarbox_grid.h>

#include "path_random_generator.h"

namespace hpgl
{
	class path_random_generator_t;

	class random_path_generator_t
	{
		path_random_generator_t m_path_gen;
		size_t m_counter;
		size_t m_size;
		bool m_eop;
		node_index_t m_first_index;
	public:
		random_path_generator_t(size_t size, int seed);		
		bool end_of_path();
		node_index_t get_next();
		size_t get_next(size_t count, std::vector<node_index_t> & indices);
	};
}

#endif //__RANDOM_NODE_ENUMERATOR_H__89D32835_1CF0_4966_BA62_6F0C4B0FBE43__
