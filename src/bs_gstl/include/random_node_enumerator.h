/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

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

	template <typename T>
	class BS_API_PLUGIN random_node_enumerator_t
	{
		boost::shared_ptr<path_random_generator_t> m_generator;
		int m_seed;
		const sugarbox_grid_t * m_geometry;
		property_array_t<T> * m_property;
		int m_current_idx;
		bool m_initialized;
		int m_watch_counter;
		int m_start_idx;
	public:
		random_node_enumerator_t();
		random_node_enumerator_t(
				const sugarbox_grid_t * grid,
				property_array_t<T> * property,
				int seed);
		void init(
				const sugarbox_grid_t * grid,
				property_array_t<T> * property,
				int seed);
		void reset();
		bool move_next();
		inline const sugarbox_grid_t * grid()
		{
			return m_geometry;
		}

	public:
		node_index_t current_index()const
		{
			return m_current_idx;
		}
	};

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
