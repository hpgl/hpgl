/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __GSALGO_INDICATOR_KRIGING_COMMAND_H__56F6BFCF_ACC9_4F63_A197_57316908E436__
#define __GSALGO_INDICATOR_KRIGING_COMMAND_H__56F6BFCF_ACC9_4F63_A197_57316908E436__

#include <ik_params.h>
#include <sugarbox_grid.h>
#include "progress_reporter.h"
#include "cdf_utils.h"
#include "pretty_printer.h"
#include "my_kriging_weights.h"
#include "precalculated_covariance.h"
#include "kriging_interpolation.h"
#include "neighbourhood_lookup.h"
#include "is_informed_predicate.h"
#include "cov_model.h"

namespace hpgl
{
	
	typedef std::vector<indicator_probability_t> marginal_probs_t;

	

	/*template<
		typename grid_t,
		typename data_t,
		typename means_t,
		typename weight_calculator_t>*/

	namespace detail
	{
		template<typename cov_t>
		void create_precalucated_cov_models(const ik_params_t & params, std::vector<cov_t> & result)
		{
			result.resize(params.m_category_count);
			for (int i = 0; i < params.m_category_count; ++i)
			{
				result[i].init(cov_model_t(params.m_cov_params[i]), params.m_radiuses[i]);	
			}
		}

		template<typename nl_t, typename prop_t>
		void add_defined_cells(std::vector<nl_t> & neighbour_lookups, const prop_t & prop)
		{
			for (int node = 0, end_node = prop.size(); node < end_node; ++node)
			{
				if (prop.is_informed(node))
				{
					for (int i = 0, end_i = neighbour_lookups.size(); i < end_i; ++i)
					{
						neighbour_lookups[i].add_node(node);
					}
				}
			}
		}
	}

	template<typename grid_t, typename marginal_probs_t>
	void do_indicator_kriging(
		const indicator_property_array_t & input_property, 
		const grid_t & grid, 
		const ik_params_t & params, 
		const marginal_probs_t & mps,
		indicator_property_array_t & output_property)
	{
		using namespace hpgl::detail;

		print_algo_name("Indicator Kriging");
		print_params(params);
		progress_reporter_t report(grid.size());

		typedef precalculated_covariances_t cov_t;
		std::vector<cov_t> covariances;
		create_precalucated_cov_models(params, covariances);		

		typedef indexed_neighbour_lookup_t<grid_t, cov_t> nl_t;
		std::vector<nl_t> nblookups;

		for (int i = 0; i < params.m_category_count; ++i)
		{			
			nblookups.push_back(nl_t(&grid, &covariances[i], params.m_nb_params[i]));
		}	

		add_defined_cells(nblookups, input_property);	

		std::vector<indicator_array_adapter_t> ind_props;
		for (int i = 0; i < params.m_category_count; ++i)
		{
			ind_props.push_back(indicator_array_adapter_t(&input_property, i));
		}

		report.start();

		size_t size = input_property.size();

		#pragma omp parallel
		{
			#pragma omp for
			for (node_index_t node_idx = 0;	node_idx < size; ++node_idx)
			{
				std::vector<indicator_probability_t> probs;
				for (int idx = 0; idx < params.m_category_count; ++idx)
				{
					indicator_probability_t prob;

					ki_result_t ki_result = kriging_interpolation(ind_props[idx], is_informed_predicate_t<indicator_property_array_t>(input_property), node_idx, covariances[idx], mps[idx], nblookups[idx], weight_calculator(sk_constraints, input_property), prob);

					if (ki_result != KI_SUCCESS)
					{
						prob = mps[idx][node_idx];
					}
					probs.push_back(prob);
				}				

				output_property.set_at(node_idx, most_probable_category(probs));			
				#pragma omp critical
				{
					report.next_lap();
				}
			}
		}

		report.stop();	
		std::cout << "\nDone. Average speed: " << report.iterations_per_second() << " point/sec." << std::endl;
	}
}



#endif //__GSALGO_INDICATOR_KRIGING_COMMAND_H__56F6BFCF_ACC9_4F63_A197_57316908E436__
