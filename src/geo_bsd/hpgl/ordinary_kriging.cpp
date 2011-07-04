#include "stdafx.h"
#include "property_array.h"
#include "sugarbox_grid.h"
#include "ok_params.h"
#include "pretty_printer.h"
#include "cont_kriging.h"
#include "mean_provider.h"
#include "my_kriging_weights.h"
#include "sugarbox_indexed_neighbour_lookup.h"
#include "kriging_stats.h"

namespace hpgl
{
	void ordinary_kriging(
		const cont_property_array_t & input,
		const sugarbox_grid_t & grid,
		const ok_params_t & params,		
		cont_property_array_t & output,
		bool use_new_cov)
	{			
		print_algo_name("Ordinary Kriging");
		print_params(params);
		
		kriging_stats_t stats;
		progress_reporter_t report(grid.size());

		if (use_new_cov)
		{
			cov_model_t covariance(params);

			hpgl::cont_kriging(input, grid, params, no_mean_t(), covariance,
				ok_weight_calculator_t(), 
				output, report, stats, undefined_on_failure);
		}
		else
		{	
			typedef precalculated_covariances_t covariances_t;
			covariances_t pcov(cov_model_t(params), params.m_radiuses);

			hpgl::cont_kriging(input, grid, params, no_mean_t(), pcov, 
				ok_weight_calculator_t(), 
				output, report, stats, undefined_on_failure);
		}

		write(boost::format("%1%") % stats);
	}
}
