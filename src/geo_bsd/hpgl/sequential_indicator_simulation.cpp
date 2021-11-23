#include "stdafx.h"

#include "mean_provider.h"
#include "ik_params.h"
#include "random_node_enumerator.h"
#include "bs_random_generator.h"
#include "sample.h"
#include "my_kriging_weights.h"
#include "pretty_printer.h"
#include "progress_reporter.h"
#include "precalculated_covariance.h"
#include "kriging_interpolation.h"
#include "sugarbox_indexed_neighbour_lookup.h"
#include "is_informed_predicate.h"
#include "cov_model.h"
#include "hpgl_exception.h"

namespace hpgl
{

namespace
{	

template<typename grid_t, typename marginal_probs_t, typename weight_calculator_t, typename mask_t>
void do_sis(
		indicator_property_array_t & property,
		const grid_t & grid,
		const ik_params_t & params,
		int seed,
		const marginal_probs_t & marginal_probs,		
		progress_reporter_t & reporter,
		const weight_calculator_t & weight_calculator_sis,
		const mask_t & mask)
{
	typedef precalculated_covariances_t cov_t;
	typedef neighbour_lookup_t<grid_t, cov_t> nl_t;
	std::vector<cov_t> 							covariances(params.m_category_count);	
	std::vector<nl_t> 							nblookups;
	std::vector<indicator_array_adapter_t> 		ind_props;

	for (int i = 0; i < params.m_category_count; ++i)
	{		
		covariances[i].init(cov_model_t(params.m_cov_params[i]), params.m_radiuses[i]);
		nblookups.push_back(nl_t(&grid, &covariances[i], params.m_nb_params[i]));
		ind_props.push_back(indicator_array_adapter_t(&property, i));
	}

	random_path_generator_t path(property.size(), seed);
	
	mt_random_generator_t gen(seed);
	reporter.start();

	if(params.m_category_count == 2)
	{
		write("Only 2 indicators found, Median SIS will be performed.");
	}

	while (!path.end_of_path())	
	{
		node_index_t node = path.get_next();
		reporter.next_lap();
		if (property.is_informed(node))
			continue;

		if (mask[node] != 1)
			continue;

		std::vector<indicator_probability_t> probs;		

		// median SIS
		if(params.m_category_count == 2)
		{
				int idx = 0;

				double prob;
				std::vector<node_index_t> indices;
				std::vector<kriging_weight_t> weights;				
				
				ki_result_t ki_result = kriging_interpolation(
					ind_props[idx], is_informed_predicate_t<indicator_property_array_t>(property), 
					node, covariances[idx], marginal_probs[idx], nblookups[idx], weight_calculator_sis, prob);

				if (ki_result != ki_result_t::KI_SUCCESS)
				{
					prob = marginal_probs[idx][node];
				}

				probs.push_back(prob);
				probs.push_back(1 - prob);

		}
		else
		{
			for (indicator_index_t idx = 0; idx < params.m_category_count; ++idx)
			{
				double prob;
				std::vector<node_index_t> indices;
				std::vector<kriging_weight_t> weights;				
				
				ki_result_t ki_result = kriging_interpolation(
					ind_props[idx], is_informed_predicate_t<indicator_property_array_t>(property), 
					node, covariances[idx], marginal_probs[idx], nblookups[idx], weight_calculator_sis, prob);

				if (ki_result != ki_result_t::KI_SUCCESS)
				{
					prob = marginal_probs[idx][node];
				}
				probs.push_back(prob);
			}
		}
		property.set_at(node, sample(gen, probs));
	}

	reporter.stop();	
}

struct no_mask_t
{
	int operator[](node_index_t index)const
	{
		return 1;
	}
};

} //namespace


void sequential_indicator_simulation(
			indicator_property_array_t & property,
			const sugarbox_grid_t & grid,
			const ik_params_t & params,
			int seed,
			progress_reporter_t & report,
			bool use_corellogram, // UNUSED!
			const unsigned char * mask)
{
	print_algo_name("Sequential Indicator Simulation");
	print_params(params);
	if (property.size() != grid.size())
		throw hpgl_exception("sequential_indicator_simulation", boost::format("Property size '%s' is not equal to grid size '%s'") % property.size() % grid.size());
	
	std::vector<single_mean_t> single_means;
	create_means(params.m_marginal_probs, single_means);	
	if (mask == NULL)	
		do_sis(property, grid, params, seed, single_means, report, sk_weight_calculator_t(), no_mask_t());
	else
		do_sis(property, grid, params, seed, single_means, report, sk_weight_calculator_t(), mask);

}
	
void sequential_indicator_simulation_lvm(
		indicator_property_array_t & property,
		const sugarbox_grid_t & grid,
		const ik_params_t & params,
		int seed,
		const mean_t ** mean_data,		
		progress_reporter_t & report,
		bool use_corellogram,
		const unsigned char * mask)
{
	print_algo_name("Sequential Indicator Simulation");
	print_params(params);
	print_param("LVM", "on");
		
	if(use_corellogram)
	{
		print_param("Corellogram", "on");
		if (mask == NULL)
			do_sis(property, grid, params, seed, mean_data, report, corellogram_weight_calculator_t(), no_mask_t());
		else
			do_sis(property, grid, params, seed, mean_data, report, corellogram_weight_calculator_t(), mask);
	} else {
		print_param("Corellogram", "off");
		if (mask == NULL)
			do_sis(property, grid, params, seed, mean_data, report, sk_weight_calculator_t(), no_mask_t());
		else
			do_sis(property, grid, params, seed, mean_data, report, sk_weight_calculator_t(), mask);
	}
}

} //namespace hpgl
