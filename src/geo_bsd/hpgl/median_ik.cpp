#include "stdafx.h"
#include "median_ik.h"

#include "covariance_field.h"
#include "progress_reporter.h"
#include "my_kriging_weights.h"
#include "kriging_interpolation.h"
#include "sugarbox_indexed_neighbour_lookup.h"
#include "is_informed_predicate.h"
#include "mean_provider.h"
#include "output.h"

namespace hpgl
{
	inline indicator_index_t choose_indicator(indicator_probability_t prob)
	{
		if (prob <= 0.5)
			return 0;
		else
			return 1;
	}

	
void median_ik_for_two_indicators(
		const median_ik_params & params, 
		const sugarbox_grid_t & grid,
		const indicator_property_array_t & input_property,
		indicator_property_array_t & output_property
)
{
	typedef sugarbox_grid_t grid_t;

	progress_reporter_t report(grid.size());
	assert(input_property.size() == output_property.size());
	if (input_property.size() != output_property.size())
	{
		throw hpgl_exception(
				"median_ik_for_two_indicators", 
				boost::format("Input property size %1% is not equal output property size %2%") % input_property.size() % output_property.size());
	}
	
	if (input_property.size() != grid.size())
		throw hpgl_exception("median_ik_for_two_indicators", boost::format("Properties size '%s' is not equal to grid size '%s'") % input_property.size() % grid.size());


	size_t prop_size = input_property.size();
	
	covariance_field_t cov_field(params.m_radiuses, cov_model_t(params));

	std::vector<node_index_t> indices;

	indicator_array_adapter_t prop_adapter(&input_property, 1);

	typedef indexed_neighbour_lookup_t<grid_t, covariance_field_t> nl_t;

	nl_t nl(&grid, &cov_field, params);

	for (node_index_t node = 0; node < input_property.size(); ++node)
	{
		if (input_property.is_informed(node))
			nl.add_node(node);
	}	

	report.start();
	#pragma omp parallel
	{
		#pragma omp for
		for (node_index_t idx = 0; idx < prop_size; ++idx)
		{
			double prob;
			indicator_index_t ind_index;

			kriging_interpolation(
				prop_adapter, 
				is_informed_predicate_t<indicator_property_array_t>(input_property), 
				idx, 				 
				cov_field, 
				single_mean_t(params.m_marginal_probs[1]), 
				nl, sk_weight_calculator_t(), prob);
		
			ind_index = choose_indicator(prob);
			output_property.set_at(idx, ind_index);
			
			#pragma omp critical
			{
				report.next_lap();
			}
		}
	}
	report.stop();	
	write(boost::format("Done. Average speed: %1% point/sec.\n") 
		  % report.iterations_per_second());
}
}

