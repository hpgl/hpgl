/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include "bs_assert.h"
#include <clusterizer.h>
#include "sugarbox_grid.h"

namespace hpgl
{
	class cluster_t
	{
		//non copyable
		cluster_t(const cluster_t &);
		cluster_t & operator=(const cluster_t &);	
	public:
		cluster_t (size_t limit)
		:	m_nodes(new std::vector<node_index_t>()),
			m_limit(limit),
			m_limit_exceeded(false)
		{
			m_nodes->reserve(limit);
		}		

		void add_node(node_index_t node)
		{
			if (m_limit_exceeded)
				return;
			if (m_nodes->size() >= m_limit)
			{
				m_limit_exceeded = true;
				m_nodes.reset();
				return;
			}
			m_nodes->push_back(node);
		}
		inline bool limit_exceeded() const { return m_limit_exceeded;}
		inline size_t count() const
		{
			if (m_limit_exceeded)	
				throw hpgl_exception("cluster_t", "Limit exceeded.");
			return m_nodes->size();
		}
		const std::vector<node_index_t> & nodes() const{ return *m_nodes;}

	private:
		std::auto_ptr<std::vector<node_index_t> > m_nodes;
		size_t m_limit;
		bool m_limit_exceeded;
	};
	

	struct clusterizer_t::state
	{
		//typedef std::vector<node_index_t> cluster_t;
		std::vector<boost::shared_ptr<cluster_t> > m_clusters;
		rect_3d_t<int> m_cluster_box;
		int m_x;
		int m_y;
		int m_z;
		
		const sugarbox_grid_t * m_geometry;
		size_t m_limit;
	};
	
	clusterizer_t::clusterizer_t()
		: m_state(new state())
	{
	}

	clusterizer_t::clusterizer_t(
			const sugarbox_grid_t * grid, 			
			const sugarbox_search_ellipsoid_t & ellipsoid,
			size_t limit)
		: m_state(new state())
	{		
		m_state->m_limit = limit;
		m_ellipsoid = ellipsoid;
		m_state->m_geometry = grid;
		
		int gx, gy, gz;
		grid->get_dimensions(gx, gy, gz);
		
		m_state->m_x = gx / ellipsoid[0] + 2;
		m_state->m_y = gy / ellipsoid[1] + 2;
		m_state->m_z = gz / ellipsoid[2] + 2;

		m_state->m_cluster_box = rect_3d_t<int>(0, 0, 0, m_state->m_x, m_state->m_y, m_state->m_z);
		m_state->m_clusters.resize(m_state->m_cluster_box.volume());		
		for (size_t i = 0; i < m_state->m_cluster_box.volume(); ++i)
		{
			m_state->m_clusters[i].reset(new cluster_t(m_state->m_limit));
		}
	}

	clusterizer_t::~clusterizer_t()
	{
		delete m_state;
	}


	int clusterizer_t::get_index(int cx, int cy, int cz)const
	{
		if (!m_state->m_cluster_box.has(cx, cy, cz))
			return -1;		
		return cz * m_state->m_x * m_state->m_y + cy * m_state->m_x + cx;
	}

	void clusterizer_t::add_node(node_index_t idx)
	{
		sugarbox_location_t loc = m_state->m_geometry->operator[](idx);	
		
		m_state->m_clusters[get_index_from_grid_point(loc)]->add_node(idx);
	}
	
	int clusterizer_t::get_nearby_harddata_count(node_index_t idx)const
	{
		sugarbox_location_t loc = m_state->m_geometry->operator[](idx);
		int cx = loc[0] / m_ellipsoid[0];
		int cy = loc[1] / m_ellipsoid[1];
		int cz = loc[2] / m_ellipsoid[2];

		int result = 0;
			
		for (int k = cz - 1; k <= cz + 1; ++k)
			for (int j = cy - 1; j	<= cy + 1; ++j)
				for (int i = cx - 1; i <= cx + 1; ++i)
				{
					int cluster_idx = get_index(i, j, k);
					if (cluster_idx >= 0)
					{
						if (m_state->m_clusters[cluster_idx]->limit_exceeded())
							throw hpgl_exception("clusterizer_t", "Limit exceeded.");
			   			size_t cluster_size = m_state->m_clusters[cluster_idx]->count();
						result += cluster_size;				
					}
				}
		return result;
	}

	bool clusterizer_t::get_nearby_harddata(node_index_t idx, std::vector<node_index_t> & neighbours)const
	{
		sugarbox_location_t loc = m_state->m_geometry->operator[](idx);
		int cx = loc[0] / m_ellipsoid[0];
		int cy = loc[1] / m_ellipsoid[1];
		int cz = loc[2] / m_ellipsoid[2];

		neighbours.clear();

		for (int k = cz - 1; k <= cz + 1; ++k)
			for (int j = cy - 1; j	<= cy + 1; ++j)
				for (int i = cx - 1; i <= cx + 1; ++i)
				{
					int cluster_idx = get_index(i, j, k);
					if (cluster_idx >= 0)
					{
			   			//size_t cluster_size = m_clusters[cluster_idx]->count();
						const std::vector<node_index_t> & nodes = m_state->m_clusters[cluster_idx]->nodes();
						neighbours.reserve(neighbours.size() + nodes.size());	
						std::copy(nodes.begin(), nodes.end(), std::back_inserter(neighbours));					
					}
				}
		return true;
	}
	
	bool clusterizer_t::limit_exceeded(node_index_t idx)const		
	{
		sugarbox_location_t loc = m_state->m_geometry->operator[](idx);
		int cx = loc[0] / m_ellipsoid[0];
		int cy = loc[1] / m_ellipsoid[1];
		int cz = loc[2] / m_ellipsoid[2];

		for (int k = cz - 1; k <= cz + 1; ++k)
			for (int j = cy - 1; j	<= cy + 1; ++j)
				for (int i = cx - 1; i <= cx + 1; ++i)
				{
					int cluster_idx = get_index(i, j, k);
					if (cluster_idx >= 0)
					{
						if (m_state->m_clusters[cluster_idx]->limit_exceeded())
							return true;			   		
					}
				}
		return false;
	}
		
}
