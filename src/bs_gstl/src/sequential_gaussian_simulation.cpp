/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include "property_array.h"
#include "sugarbox_grid.h"
#include "sgs_params.h"
#include "pretty_printer.h"
#include "sequential_simulation.h"
#include "calc_mean.h"
#include "mean_provider.h"
#include "my_kriging_weights.h"
#include "sugarbox_indexed_neighbour_lookup.h"
#include "lvm_utils.h"
#include "gaussian_distribution.h"
#include "non_parametric_cdf.h"

namespace hpgl
{

	template<typename cdf_t>
	mean_t get_mean(const cont_property_array_t & output, const sk_params_t & params, const cdf_t & target_cdf)
	{
		double mean;
		if (params.m_calculate_mean)
		{
			bool mean_valid;
			mean = calc_mean(output, &mean_valid);
			if (!mean_valid)
					std::cout << "WARNING: No data to calculate mean. Defaulting to 0.\n";
			print_param("Calculated SGS mean", mean);			
			std::cout.flush();
		} else {
			mean = transform_cdf(params.mean(), target_cdf, gaussian_cdf_t());
		}	
		return mean;
	}

void sequential_gaussian_simulation(
		const sugarbox_grid_t & grid,
		const sgs_params_t & params,
		bool use_harddata,
		cont_property_array_t & output,
		const cont_property_array_t & cdf_property,
		const indicator_property_array_t * mask)
{
	print_algo_name("Sequential Gaussian Simulation");
	print_params(params);
	std::cout.flush();

	if (output.size() != grid.size())
		throw hpgl_exception("sequential_gaussian_simulation",
		boost::format("Input property size: %s. Grid size: %s. Must be equal.") % output.size() % grid.size());
	
	/*No_TI no_ti;
	Tail_interpolator ti(&no_ti);
	Non_param_cdf<> target_cdf(ti, Linear_interpol(), ti);
	build_cdf_from_property(cdf_property, target_cdf);
	transform_cdf(output, target_cdf, Gaussian_cdf());*/
	non_parametric_cdf_t<cont_value_t, double> new_cdf(cdf_property);
	transform_cdf(output, new_cdf, gaussian_cdf_t());

	if (params.m_kriging_kind == KRIG_SIMPLE)
	{
		double mean = get_mean(output, params, new_cdf);
		
		if (mask != NULL)
		{
			do_sequential_gausian_simulation( output, grid, params, 
					single_mean_t(mean), sk_constraints, 
					weight_calculator(sk_constraints, output), 
					*mask);
		}
		else
		{
			do_sequential_gausian_simulation( output, grid, params, 
					single_mean_t(mean), sk_constraints, 
					weight_calculator(sk_constraints, output),
					no_mask_t());
			
		}
	} else {
		if (mask != NULL)
		{
			do_sequential_gausian_simulation( output, grid, params,
				no_mean_t(), ok_constraints,
				weight_calculator(ok_constraints, output),
				*mask);
		} else {
			do_sequential_gausian_simulation( output, grid, params,
				no_mean_t(), ok_constraints,
				weight_calculator(ok_constraints, output),
				no_mask_t());
		}
	}
	
	
	transform_cdf(output, gaussian_cdf_t(), new_cdf);
	
}

void sequential_gaussian_simulation_lvm(
		const sugarbox_grid_t & grid,
		const sgs_params_t & params,
		bool use_harddata,
		const std::vector<mean_t> & mean_data,
		cont_property_array_t & output,
		const cont_property_array_t & cdf_property,
		const indicator_property_array_t * mask
		)
{
	print_algo_name("Sequential Gaussian Simulation with Local Varying Mean");
	print_params(params);
	std::cout.flush();

	if (output.size() != grid.size())
		throw hpgl_exception("sequential_gaussian_simulation_lvm",
		boost::format("Input property size: %s. Grid size: %s. Must be equal.") % output.size() % grid.size());
	if (output.size() != mean_data.size())
		throw hpgl_exception("sequential_gaussian_simulation_lvm",
		boost::format("Input property size: %s. Mean data size: %s. Must be equal.") % output.size() % mean_data.size());

	subtract_means(output, mean_data);	

	sequential_gaussian_simulation(grid, params, use_harddata, output, cdf_property, mask);	
	
	add_means(output, mean_data);
	
}

}
