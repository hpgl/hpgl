/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef NEIGHBOUR_SET_H_INCLUDED_ASDLLAS_ASDJASDLASDASDASDJKASDELWIOCWNENRNWER
#define NEIGHBOUR_SET_H_INCLUDED_ASDLLAS_ASDJASDLASDASDASDJKASDELWIOCWNENRNWER

#include "select.h"
#include "combiner.h"

namespace hpgl
{
	template<typename grid_t, typename data_t, typename means_t>
	class neighbour_set_t
	{
	public:	
		typedef typename grid_t::coord_t coord_t;
		const data_t * m_data;
		const means_t * m_mp;

		node_index_t m_node;
		coord_t m_center;
		std::vector<coord_t> m_nb_coords;
		std::vector<node_index_t> m_nb_indices;
		bool m_kriging_success;
		kriging_variance_t m_variance;
		std::vector<kriging_weight_t> m_weights;
		
	public:
		neighbour_set_t()
			: m_data(0), m_mp(0)
		{}

		neighbour_set_t(const data_t * data, const means_t * mp)
			: m_data(data), m_mp(mp)
		{}

		void init_center_node(node_index_t node)
		{
			m_node = node;
		}

		template<typename nb_lookup_t, typename defineds_t>
		void init_neighbours(const nb_lookup_t & nb_lookup, const defineds_t & defineds)
		{
			nb_lookup.find(m_node, defineds, m_center, m_nb_indices, m_nb_coords);
		}

		template<typename constraints_t, typename cov_model_t>
		void init_weights(const cov_model_t & cov_model, constraints_t constraints)
		{
			m_kriging_success = kriging_weights_2(m_center,
					m_nb_coords, cov_model, constraints, m_weights);
		}

		template<typename constraints_t, typename cov_model_t>
		void init_weights_and_variance(const cov_model_t & cov_model, constraints_t constraints)
		{
			m_kriging_success = kriging_weights_2(m_center,
					m_nb_coords, cov_model, constraints, m_weights, m_variance);
		}

		template<typename result_t>
		result_t combine()
		{
			std::vector<mean_t> means;
			std::vector<typename data_t::value_type> values;
			select(*m_mp, m_nb_indices, means);
			select(*m_data, m_nb_indices, values);
			return hpgl::combine<typename data_t::value_type, result_t>
				(values, m_weights, means, (*m_mp)[m_node]);
		}	
	};
}

#endif
