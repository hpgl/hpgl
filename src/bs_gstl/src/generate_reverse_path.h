/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __GENERATE_REVERSE_PATH_H__68FBB897_E796_4FE8_9F58_58DEEBB84F9D
#define __GENERATE_REVERSE_PATH_H__68FBB897_E796_4FE8_9F58_58DEEBB84F9D

#include "random_node_enumerator.h"

namespace hpgl
{
	template<typename prop_t>
	void generate_reverse_path(const prop_t & prop, int seed, std::vector<node_index_t> & reverse_path)
	{
		reverse_path.resize(prop.size());
		random_path_generator_t rpg(prop.size(), seed);
		int counter = 1;
		while (!rpg.end_of_path())
		{
			node_index_t node = rpg.get_next();
			if (prop.is_informed(node))
			{
				reverse_path[node] = 0;
			} else {
				reverse_path[node] = counter;
			}
			++counter;
		}
	}
	
	template<typename prop_t>
	void generate_reverse_path_in_points(const prop_t & prop, const std::vector<int> & points_indexes, int seed, std::vector<node_index_t> & reverse_path)
	{
		int not_modeled = prop.size() + 1;
		reverse_path.resize(prop.size(), not_modeled);
		random_path_generator_t rpg(points_indexes.size(), seed);
		int counter = 1;

		while (!rpg.end_of_path())
		{
			node_index_t path_index = rpg.get_next();
			node_index_t node = points_indexes[path_index];
			if (prop.is_informed(node))
			{
				reverse_path[node] = 0;
			} 
			else
			{
				reverse_path[node] = counter;
				++counter;
			}
		}
	}

	template<typename mask_t>
	void generate_reverse_path_with_mask(int length, int seed, 
			const mask_t & mask, 
			std::vector<node_index_t> & reverse_path)
	{
		reverse_path.resize(length);
		random_path_generator_t rpg(length, seed);
		int counter = 0;
		while (!rpg.end_of_path())
		{
			++counter;
			node_index_t node = rpg.get_next();
			if (mask[node] == 1)
				reverse_path[node] = 0;
			else
				reverse_path[node] = counter;
		}
	}

}

#endif //__GENERATE_REVERSE_PATH_H__68FBB897_E796_4FE8_9F58_58DEEBB84F9D
