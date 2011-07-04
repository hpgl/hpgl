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
#include "sk_params.h"
#include "pretty_printer.h"
#include "cont_kriging.h"
#include "calc_mean.h"
#include "mean_provider.h"
#include "sugarbox_indexed_neighbour_lookup.h"
#include "my_kriging_weights.h"

namespace hpgl
{

            void simple_kriging_weights(
                real_covariance_t cov_params,
                real_location_t center_point,
                std::vector<real_location_t> neighbourhoods_coords,
                std::vector<kriging_weight_t> & weights,
                double & variance)
            {
                
               bool result = kriging_weights_2(
			center_point,
			neighbourhoods_coords,
			cov_params,
			sk_constraints,
			weights,
			variance);

               if(result == false)
               {
                   weights.resize(0);
               }

            }

	void simple_kriging(
			const cont_property_array_t & input,
			const sugarbox_grid_t & grid,
			const sk_params_t & params,
			cont_property_array_t & output)
	{
		print_algo_name("Simple Kriging");
		print_params(params);

		double mean;
		if (params.m_calculate_mean)
		{	
			mean = calc_mean(input);
			print_param("Calculated mean: ", mean);		
		}
		else
			mean = params.mean();

		std::cout.flush();
		kriging_stats_t stats;			
		progress_reporter_t report(grid.size());

		sugarbox_covariance_t covariance;	
		covariance_from_param(params, covariance);	
		typedef precalculated_covariances_t<sugarbox_covariance_t, sugarbox_location_t> covariances_t;
		covariances_t pcov(covariance, params.m_radiuses);

		hpgl::cont_kriging(input, grid, params, single_mean_t(mean), pcov, 
			weight_calculator(sk_constraints, input), 
			output, report, stats, mean_on_failure); 

		std::cout << stats << std::endl;
	}
}
