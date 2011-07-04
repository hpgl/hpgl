/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include "property_array.h"
#include "sugarbox_grid.h"
#include "sgs_params.h"
#include "pretty_printer.h"
#include "sequential_simulation.h"
#include "calc_mean.h"
#include "mean_provider.h"
#include "covariance_from_param.h"
#include "my_kriging_weights.h"
#include "sugarbox_indexed_neighbour_lookup.h"
#include "lvm_utils.h"

namespace hpgl
{

	mean_t get_mean(const cont_property_array_t & output, const sk_params_t & params, const Non_param_cdf<> & target_cdf)
	{
		double mean;
		if (params.m_calculate_mean)
		{
			mean = calc_mean(output);
			print_param("Calculated SGS mean", mean);			
			std::cout.flush();
		} else {
			mean = transform_cdf(params.mean(), target_cdf, Gaussian_cdf());
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
	
	No_TI no_ti;
	Tail_interpolator ti(&no_ti);
	Non_param_cdf<> target_cdf(ti, Linear_interpol(), ti);

	build_cdf_from_property(cdf_property, target_cdf);
	transform_cdf(output, target_cdf, Gaussian_cdf());

	if (params.m_kriging_kind == KRIG_SIMPLE)
	{
		double mean = get_mean(output, params, target_cdf);
		
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
	
	transform_cdf(output, Gaussian_cdf(), target_cdf);
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
