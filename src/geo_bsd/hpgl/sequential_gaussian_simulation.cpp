#include "stdafx.h"
#include "api.h"
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
	void sequential_gaussian_simulation(
		const sugarbox_grid_t& grid,
		const sgs_params_t& params,
		cont_property_array_t& output,
		const hpgl_non_parametric_cdf_t* cdf,
		const unsigned char* mask)
	{
		print_algo_name("Sequential Gaussian Simulation");
		print_params(params);

		if (output.size() != grid.size())
			throw hpgl_exception("sequential_gaussian_simulation",
				boost::format("Input property size: %s. Grid size: %s. Must be equal.") % output.size() % grid.size());

		if (cdf != 0)
		{
			transform_cdf_p(output, non_parametric_cdf_2_t(cdf), gaussian_cdf_t());
		}

		if (params.m_kriging_kind == KRIG_SIMPLE)
		{
			double mean = 0;

			if (mask != NULL)
			{
				do_sequential_gausian_simulation(output, grid, params,
					single_mean_t(mean),
					sk_weight_calculator_t(),
					mask);
			}
			else
			{
				do_sequential_gausian_simulation(output, grid, params,
					single_mean_t(mean),
					sk_weight_calculator_t(),
					no_mask_t());

			}
		}
		else {
			if (mask != NULL)
			{
				do_sequential_gausian_simulation(output, grid, params,
					no_mean_t(),
					ok_weight_calculator_t(),
					mask);
			}
			else {
				do_sequential_gausian_simulation(output, grid, params,
					no_mean_t(),
					ok_weight_calculator_t(),
					no_mask_t());
			}
		}

		if (cdf != 0)
			transform_cdf_p(output, gaussian_cdf_t(), non_parametric_cdf_2_t(cdf));
	}

	void sequential_gaussian_simulation_lvm(
		const sugarbox_grid_t& grid,
		const sgs_params_t & params,
		const mean_t* mean_data,
		cont_property_array_t& output,
		const hpgl_non_parametric_cdf_t* cdf,
		const unsigned char* mask
	)
	{
		print_algo_name("Sequential Gaussian Simulation with Local Varying Mean");
		print_params(params);

		if (output.size() != grid.size())
			throw hpgl_exception("sequential_gaussian_simulation_lvm",
			boost::format("Input property size: %s. Grid size: %s. Must be equal.") % output.size() % grid.size());

		if (output.size() != grid.size())
			throw hpgl_exception("sequential_gaussian_simulation",
			boost::format("Input property size: %s. Grid size: %s. Must be equal.") % output.size() % grid.size());

		std::vector<mean_t> mean_data_vec;
		mean_data_vec.assign(mean_data, mean_data + output.size() );

		if (cdf != 0)
		{
			non_parametric_cdf_2_t new_cdf(cdf);
			transform_cdf_p(output, new_cdf, gaussian_cdf_t());
			transform_cdf_ptr(mean_data, mean_data_vec, new_cdf, gaussian_cdf_t());
		}

			if (mask != NULL)
			{
				do_sequential_gausian_simulation( output, grid, params,
						mean_data_vec,
						sk_weight_calculator_t(),
						mask);
			}
			else
			{
				do_sequential_gausian_simulation( output, grid, params,
						mean_data_vec,
						sk_weight_calculator_t(),
						no_mask_t());

			}

			if (cdf != 0)
			{
				non_parametric_cdf_2_t new_cdf(cdf);
				transform_cdf_p(output, gaussian_cdf_t(), new_cdf);
			}
	}
}
