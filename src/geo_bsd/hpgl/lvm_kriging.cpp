#include "stdafx.h"

#include "typedefs.h"
#include "property_array.h"
#include "sugarbox_grid.h"
#include "ok_params.h"
#include "pretty_printer.h"
#include "cont_kriging.h"
#include "mean_provider.h"
#include "my_kriging_weights.h"
#include "sugarbox_indexed_neighbour_lookup.h"

namespace hpgl
{

void lvm_kriging(
		const cont_property_array_t & input,
		const mean_t * mean_data,
		const sugarbox_grid_t & grid,
		const ok_params_t & params,
		cont_property_array_t & output)
{
	print_algo_name("LVM Kriging");
	print_params(params);
	
	progress_reporter_t reporter(grid.size());
	kriging_stats_t stats;

	typedef precalculated_covariances_t covariances_t;
	covariances_t pcov(cov_model_t(params), params.m_radiuses);

	hpgl::cont_kriging(input, grid, params, mean_data, pcov, 
		sk_weight_calculator_t(), 
		output, reporter, stats, mean_on_failure); 

	write(boost::format("%1%") % stats);	
}

}
