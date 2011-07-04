#ifndef NEIGHBOUR_SET_H_INCLUDED_ASDLLAS_ASDJASDLASDASDASDJKASDELWIOCWNENRNWER
#define NEIGHBOUR_SET_H_INCLUDED_ASDLLAS_ASDJASDLASDASDASDJKASDELWIOCWNENRNWER

#include "select.h"
#include "combiner.h"

namespace hpgl
{
	template<typename grid_t, typename data_t, typename means_t>
	class neighbour_set_t
	{
		struct filter_predicate_t
		{
			const reverse_path_t & m_path;
			node_index_t m_index;
			
			filter_predicate_t(const reverse_path_t & path, node_index_t index): m_path(path), m_index(index)
			{}
			
			bool operator()(node_index_t neighbour)const
			{
				if (neighbour < 0)
					return false;
					return m_path[m_index] > m_path[neighbour];
						}
		};
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

		void set_data_and_means(const data_t * data, const means_t * mp)
		{
			m_data = data; 
			m_mp = mp;
		}

		void init_center_node(node_index_t node)
		{
		  m_node = node;
		}

		template<typename nb_lookup_t, typename defineds_t>
		void init_neighbours(const nb_lookup_t & nb_lookup, const defineds_t & defineds)
		{
			nb_lookup.find(m_node, defineds, m_center, m_nb_indices, m_nb_coords);
		}

		template<typename nb_lookup_t, typename reverse_path_t>
		void find_neighbours_with_reverse_path(
											   const nb_lookup_t & nb_lookup,
											   const reverse_path_t & reverse_path,
											   node_index_t center_index)
		{			
			init_neighbours<nb_lookup_t, filter_predicate_t>(nb_lookup, filter_predicate_t(reverse_path, center_index));
		}

		template<typename wc_t, typename cov_model_t>
		void init_weights(const cov_model_t & cov_model, wc_t weight_calculator)
		{
			m_kriging_success = weight_calculator(m_center,
					m_nb_coords, cov_model, m_weights);
		}

		template<typename wc_t, typename cov_model_t>
		void init_weights_ind(const cov_model_t & cov_model, wc_t weight_calculator)
		{
			std::vector<mean_t> means;
			select(*m_mp, m_nb_indices, means);
			m_kriging_success = weight_calculator(
												  m_center,
												  (*m_mp)[m_node],
												  m_nb_coords, 
												  means,
												  cov_model, 
												  m_weights);
		}

		template<typename wc_t, typename cov_model_t>
		void calc_weights_first_stage(const cov_model_t & cov_model, const wc_t & wc)
		{
			m_kriging_success =  wc.first_stage(m_center, m_nb_coords, cov_model, m_weights);
		}

		template<typename wc_t>
		void calc_weights_second_stage(const wc_t & wc)
		{
			std::vector<mean_t> means;
			select(*m_mp, m_nb_indices, means);
			m_kriging_success |= wc.second_stage(m_weights, (*m_mp)[m_node], means);
		}

		template<typename wc_t, typename cov_model_t>
		void init_weights_and_variance(const cov_model_t & cov_model, wc_t weight_calculator)
		{
			m_kriging_success = weight_calculator(m_center,
					m_nb_coords, cov_model, m_weights, m_variance);
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
