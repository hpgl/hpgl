/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


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
		const std::vector<mean_t> & mean_data,
		const sugarbox_grid_t & grid,
		const ok_params_t & params,
		cont_property_array_t & output)
{
	print_algo_name("LVM Kriging");
	print_params(params);
	std::cout.flush();
	
	progress_reporter_t reporter(grid.size());
	kriging_stats_t stats;

	typedef precalculated_covariances_t covariances_t;
	covariances_t pcov(cov_model_t(params), params.m_radiuses);

	hpgl::cont_kriging(input, grid, params,mean_data, pcov, 
		weight_calculator(sk_constraints, input), 
		output, reporter, stats, mean_on_failure); 
	
	std::cout << stats << std::endl;
}

}
