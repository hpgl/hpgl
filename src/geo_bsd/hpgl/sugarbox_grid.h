#ifndef SUGARBOX_GRID_H_INCLUDED_RANDOM_STUFF_ASKLJDALSDJLASKDJLASDJKEWOERO
#define SUGARBOX_GRID_H_INCLUDED_RANDOM_STUFF_ASKLJDALSDJLASKDJLASDJKEWOERO

#include "typedefs.h"

namespace hpgl
{
	/*!
	 * Manages 
	 *
	 */	
	class sugarbox_grid_t
	{
		int m_x, m_y, m_z; //grid dimensions
		bool m_initialized; 
	public:
		typedef int size_type;
		typedef sugarbox_location_t value_type;
	public:
		//Coord_provider concept memebers
		typedef sugarbox_location_t coord_t;
		node_index_t size()const;
		sugarbox_location_t operator[](node_index_t index)const;
	public:
		void init(size_type x, size_type y, size_type z);
		inline bool contains(const sugarbox_location_t &)const;
		inline bool contains(node_index_t)const;
		inline node_index_t get_index(const sugarbox_location_t &)const;		
		inline node_index_t get_index(node_index_t index)const { return index; }	
		void get_dimensions(int &x, int &y, int &z)const
		{
			x = m_x; y = m_y; z = m_z;
		}
		~sugarbox_grid_t();	
	};

	typedef boost::shared_ptr<sugarbox_grid_t> sp_sugarbox_grid_t;

	bool sugarbox_grid_t::contains(const sugarbox_location_t & loc)const
	{
		return (unsigned) loc[0] < (unsigned)m_x 
				&& (unsigned) loc[1] < (unsigned)m_y
				&& (unsigned) loc[2] < (unsigned)m_z;
	}

	bool sugarbox_grid_t::contains(node_index_t index)const
	{
		return (unsigned) index < unsigned(m_x * m_y * m_z);
	}

	node_index_t sugarbox_grid_t::get_index(const sugarbox_location_t & loc)const
	{
		if (contains(loc))
			return loc[2] * m_x * m_y + loc[1] * m_x + loc[0];
		else
			return -1;
	}
	
}
#endif // SUGARBOX_GRID_H_INCLUDED_RANDOM_STUFF_ASKLJDALSDJLASKDJLASDJKEWOERO
