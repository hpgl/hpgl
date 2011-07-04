/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

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

	sugarbox_covariance_t covariance;	
	covariance_from_param(params, covariance);	
	typedef precalculated_covariances_t<sugarbox_covariance_t, sugarbox_location_t> covariances_t;
	covariances_t pcov(covariance, params.m_radiuses);

	hpgl::cont_kriging(input, grid, params,mean_data, pcov, 
		weight_calculator(sk_constraints, input), 
		output, reporter, stats, mean_on_failure); 
	
	std::cout << stats << std::endl;
}

}
