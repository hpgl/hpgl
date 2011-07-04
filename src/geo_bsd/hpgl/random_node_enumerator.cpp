#include "stdafx.h"

#include "bs_assert.h"

#include "property_array.h"
#include "path_random_generator.h"
#include "random_node_enumerator.h"

#include "hpgl_exception.h"

namespace hpgl
{	
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
