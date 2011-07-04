/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __SUGARBOX_NEIGHBOUR_LOOKUP_H__B31AEF89_3C7A_48C8_97DD_F2983ED6A237
#define __SUGARBOX_NEIGHBOUR_LOOKUP_H__B31AEF89_3C7A_48C8_97DD_F2983ED6A237

#include <typedefs.h>
#include "covariance_field.h"
#include "neighbourhood_param.h"
#include "sugarbox_grid.h"
#include "neighbourhood_lookup.h"

namespace hpgl
{
	template<typename covariances_t>
	class neighbour_lookup_t<sugarbox_grid_t, covariances_t>
	{		
		template <typename grid_t, typename cov_t> friend class indexed_neighbour_lookup_t;
		boost::shared_ptr<std::vector<sugarbox_vector_t> > m_vectors;
		const sugarbox_grid_t * m_grid;
		int m_max_neighbours;
	public:		
		typedef sugarbox_location_t coord_t;
		typedef sugarbox_grid_t grid_t;

		neighbour_lookup_t(
				const sugarbox_grid_t * grid,				
				const covariances_t * cov, 
				const neighbourhood_param_t & nb_param
				)
				:	m_max_neighbours(nb_param.m_max_neighbours), 
					m_grid(grid), 
					m_vectors(new std::vector<sugarbox_vector_t>())
		{
			calc_cov_field<covariances_t, sugarbox_location_t>(nb_param.m_radiuses, *cov, *m_vectors);
		}

		template<typename filter_pred_t>
		void find(node_index_t node, const filter_pred_t & pred, coord_t & node_coord, std::vector<node_index_t> & indices, std::vector<coord_t> & coords)const
		{
			indices.clear();
			indices.reserve(m_max_neighbours);
			coords.clear();
			coords.reserve(m_max_neighbours);
			const std::vector<sugarbox_vector_t> & vectors = *m_vectors;
			int count = 0;
			const sugarbox_vector_t * vec = &vectors[0];
			sugarbox_location_t center = (*m_grid)[node];
			node_coord = coord_t(center[0], center[1], center[2]);
			for (int idx = 0, end_idx = vectors.size(); idx < end_idx && count < m_max_neighbours; ++idx, ++vec)
			{
				sugarbox_location_t point = center + *vec; //vectors[idx];
				int index = m_grid->get_index(point);
				//if (index >= 0)
				{		
					if (pred(index)) 
					{
						indices.push_back(index);
						coords.push_back(coord_t(point[0], point[1], point[2]));						
						++count;
					}
				}
			}			
		}		
	};

}		

#endif //__SUGARBOX_NEIGHBOUR_LOOKUP_H__B31AEF89_3C7A_48C8_97DD_F2983ED6A237
