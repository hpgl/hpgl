/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

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
                const covariance_param_t * cov_params,
                real_location_t center_point,
                std::vector<real_location_t> neighbourhoods_coords,
                std::vector<kriging_weight_t> & weights,
                double & variance)
            {
                cov_model_t cov(*cov_params);
               bool result = sk_kriging_weights_3<cov_model_t, true, real_location_t>(
					center_point,
					neighbourhoods_coords,
					cov,					
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
			bool valid_mean;
			mean = calc_mean(input, &valid_mean);
			if (!valid_mean)
				std::cout << "WARNING: No data to calculate mean. Defaulting to 0.\n";
			print_param("Calculated mean: ", mean);		
		}
		else
			mean = params.mean();

		std::cout.flush();
		kriging_stats_t stats;			
		progress_reporter_t report(grid.size());

		cov_model_t covariance(params);		
		typedef precalculated_covariances_t covariances_t;
		covariances_t pcov(covariance, params.m_radiuses);

		hpgl::cont_kriging(input, grid, params, single_mean_t(mean), pcov, 
			sk_weight_calculator_t(), 
			output, report, stats, mean_on_failure); 

		std::cout << stats << std::endl;
	}
}
