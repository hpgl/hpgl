/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include "bs_assert.h"

#include <property_array.h>
#include <path_random_generator.h>
#include <random_node_enumerator.h>

#include "hpgl_exception.h"

namespace hpgl
{
	template <typename T>
	random_node_enumerator_t<T> :: random_node_enumerator_t()
		: m_initialized(false)
	{}

	template<typename T>
	random_node_enumerator_t<T> :: random_node_enumerator_t(
			const sugarbox_grid_t * grid, 
			property_array_t<T> * property, 
			int seed)
			: m_initialized(false)
	{
		init(grid, property, seed);
	}

	template<typename T>
	void random_node_enumerator_t<T> :: init(
			const sugarbox_grid_t * grid, 
			property_array_t<T> * property, 
			int seed)
	{
		BS_ASSERT(!m_initialized && "Already initialized.");
		m_seed = seed;
		m_geometry = grid;
		m_property = property;
		int grid_size = grid->size();
		int property_size = property->size();
		BS_ASSERT(grid_size == property_size)(grid_size)(property_size);
		m_initialized = true;
		reset();
		
	}

	template<typename T>
	void random_node_enumerator_t<T> :: reset()
	{
		m_generator.reset(
				new path_random_generator_t(
						m_geometry->size(), 
						m_seed));
		m_current_idx = -1;
		m_start_idx = m_seed % m_geometry->size();
		m_watch_counter = 0;
	}

	template<typename T>
	bool random_node_enumerator_t<T> :: move_next()
	{
		BS_ASSERT(m_generator && "generator is not created");
		BS_ASSERT(m_watch_counter <= m_geometry->size())(m_watch_counter);
		m_watch_counter++;	

		if (m_current_idx < 0)
		{
			m_start_idx = m_generator->next();
			m_current_idx = m_start_idx;
			return true;
		}
		else
		{
			m_current_idx = m_generator->next();
			return m_current_idx != m_start_idx;
		}
		
	}


	template class random_node_enumerator_t<cont_value_t>;
	template class random_node_enumerator_t<indicator_value_t>;

	random_path_generator_t::random_path_generator_t(size_t size, int seed)
		: m_path_gen(size, seed),
		m_counter(0),
		m_size(size),
		m_eop(false)
	{
		if (size <= 0)
		{
			throw hpgl_exception("random_path_generator_t", 
				boost::format("Invalid size: %s") % size);
		}
	}

	

	bool random_path_generator_t::end_of_path()
	{
		if (m_eop)
		{
			return true;
		}
		else if (m_counter == m_size)
		{
			m_eop = true;
			node_index_t next_index = m_path_gen.next();
			if (next_index != m_first_index)
			{
				throw hpgl_exception("random_path_generator",
					boost::format("Invariant invalid: %s != %s") % m_first_index % next_index);
			}
			return true;
		} 
		else 
		{
			return false;
		}
	}

	node_index_t random_path_generator_t::get_next()
	{
		if (end_of_path())
			throw hpgl_exception("random_path_generator", "End of path reached.");
		node_index_t result = m_path_gen.next();
		if (m_counter == 0)
		{			
			m_first_index = result;
		}
		m_counter++;
		return result;
	}

	size_t random_path_generator_t::get_next(size_t count, std::vector<node_index_t> &indices)
	{
		indices.clear();
		while (!end_of_path() && (indices.size() < count))
		{
			indices.push_back(get_next());
		}
		return indices.size();			
	}
}
