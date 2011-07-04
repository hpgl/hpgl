/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include <sugarbox_grid.h>

namespace hpgl
{

/*	
 *	These functions double sugarbox_grid_t algorithms.
 *	For now this algos aren't used anywhere else so there is no sense in refactoring sugarbox_grid_t.
 *
 *	struct grid_size_t
	{
		int x;
		int y;
		int z;
	};

	inline node_index_t get_grid_size(const grid_size_t &);
	inline bool contains(const grid_size_t & grid, const sugarbox_location_t & loc);
	inline bool contains(const grid_size_t & grid, node_index_t node);
	inline node_index_t get_node_index(const grid_size_t & grid, const sugarbox_location_t & loc);
	inline sugarbox_location_t get_node_location(const grid_size_t & grid, node_index_t node);
	
	inline node_index_t get_grid_size(const grid_size_t & grid)
	{
		return grid.x * grid.y * grid.z;
	}

	inline bool contains(const grid_size_t & grid, const sugarbox_location_t & loc)
	{
		return (unsigned) loc[0] < (unsigned)grid.x 
				&& (unsigned) loc[1] < (unsigned)grid.y
				&& (unsigned) loc[2] < (unsigned)grid.z;
	}

	inline bool contains(const grid_size_t & grid, node_index_t node)
	{
		return (unsigned) node < unsigned(grid.x * grid.y * grid.z);
	}

	template<typename sugarbox_location_t>
	inline node_index_t get_node_index(const grid_size_t & grid, const sugarbox_location_t & loc)
	{
		if (contains(grid, loc))
			return loc[2] * grid.x * grid.y + loc[1] * grid.x + loc[0];
		else
			return -1;
	}

	inline sugarbox_location_t get_node_location(const grid_size_t & grid, node_index_t index)
	{
		return sugarbox_location_t(index % grid.x, index % (grid.x * grid.y) / grid.x, index / (grid.x * grid.y) );
	}
*/


	void sugarbox_grid_t::init(size_type x, size_type y, size_type z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	node_index_t sugarbox_grid_t::size()const
	{
		return m_x * m_y * m_z;
	}


	sugarbox_location_t sugarbox_grid_t::operator[](node_index_t index)const
	{
			
		return sugarbox_location_t(index % m_x, index % (m_x * m_y) / m_x, index / (m_x * m_y) );
	}

	sugarbox_grid_t::~sugarbox_grid_t()
	{
	}
}//namespace hpgl
