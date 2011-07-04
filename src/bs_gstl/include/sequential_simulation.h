/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef SEQUENTIAL_SIMULATION_H_INCLUDED_DEU0932RMNPDVCPDZFIG03249MROPWEUFSDHVIAERT0934NTFMD0GN2304UNLKWEJOFDSZF0G32R
#define SEQUENTIAL_SIMULATION_H_INCLUDED_DEU0932RMNPDVCPDZFIG03249MROPWEUFSDHVIAERT0934NTFMD0GN2304UNLKWEJOFDSZF0G32R

#include <cdf_utils.h>
#include <path_random_generator.h>
#include <bs_random_generator.h>
#include <typedefs.h>
#include "sample.h"
#include "covariance_field.h"
#include "progress_reporter.h"
#include "combiner.h"
#include "kriging_interpolation.h"
#include "precalculated_covariance.h"
#include <covariance_from_param.h>
#include "neighbourhood_lookup.h"
#include "is_informed_predicate.h"

namespace hpgl
{

	// predicate function for std::remove_if, to check is coordinates in grid, or not
	class bigger_grid_size
	{
	public:
		bigger_grid_size(int x, int y, int z)
		{
			m_x = x;
			m_y = y;
			m_z = z;
		};
		//~bigger_grid_size(){};
		
		bool operator()(sugarbox_grid_t::coord_t value) const
		{
			return (value[0]>=m_x || value[1]>=m_y || value[2]>=m_z);
		};
	private:
		int m_x,m_y,m_z;
		
	};

	struct no_mask_t
	{
		int operator[](node_index_t node)const
		{
			return 1;
		}
	};

	template <typename grid_t, typename mean_provider_t, typename constraints_t, typename weight_calculator_t, typename mask_t>
	void do_sequential_gausian_simulation(
		property_array_t<cont_value_t> & property,
		const grid_t & grid,
		const sgs_params_t & params,
		const mean_provider_t & mp,
		constraints_t constraints,
		const weight_calculator_t & weight_calculator_sgs,
		const mask_t & mask)
	{

		Covariance<sugarbox_location_t> covariance;
		covariance_from_param(params, covariance);

		if (property.size() != grid.size())
			throw hpgl_exception("do_sequential_gausian_simulation", boost::format("Property size '%s' is not equal to grid size '%s'") % property.size() % grid.size());

		mt_random_generator_t gen;
		gen.seed(params.m_seed);
		path_random_generator_t path_gen(property.size(), params.m_seed);

		typedef precalculated_covariances_t<sugarbox_covariance_t, sugarbox_location_t> cov_t;
		cov_t pcov(covariance, params.m_radiuses);
		typedef /*indexed_*/ neighbour_lookup_t<grid_t, cov_t> nl_t;
		nl_t neighbour_lookup(&grid, &pcov,	params);
		
		/*for (node_index_t node = 0; node < property.size(); ++node)
		{
			if (property.is_informed(node))
				neighbour_lookup.add_node(node);
		}*/
		
		progress_reporter_t report(property.size());	
		report.start();
		node_index_t node = path_gen.next();

		std::vector<sugarbox_location_t> coords;
		
		for (node_index_t counter = 0, counter_end = property.size(); counter < counter_end; ++counter, node = path_gen.next(), report.next_lap())		
		{
			if(property.is_informed(node)) 
				continue;

			if (mask[node] != 1)
				continue;

			std::vector<node_index_t> indices;
			std::vector<kriging_weight_t> weights;
			double variance;						
			sugarbox_location_t loc = grid[node];			

			cont_value_t mean;
			ki_result_t ki_result = kriging_interpolation(property, is_informed_predicate_t<cont_property_array_t>(property), node, pcov, mp, 
				neighbour_lookup, weight_calculator_sgs, mean, variance);			

			double value = ki_result == KI_SUCCESS 
				? sample(gen, Gaussian_cdf(mean, variance))
				: sample(gen, Gaussian_cdf());		
			
			property.set_at(node, value);
			//neighbour_lookup.add_node(node);
		}
		report.stop();
		std::cout << "\nDone. Average speed: " << report.iterations_per_second() << " point/sec." << std::endl;
	}

	template <typename grid_t, typename mean_provider_t, typename constraints_t, typename weight_calculator_t>
	void do_sequential_gausian_simulation_in_points(
		property_array_t<cont_value_t> & property,
		const grid_t & grid,
		const sgs_params_t & params,
		const mean_provider_t & mp,
		constraints_t constraints,
		const weight_calculator_t & weight_calculator_sgs,
		const std::vector<int> & points_indexes)
	{

		Covariance<sugarbox_location_t> covariance;
		covariance_from_param(params, covariance);

		if (property.size() != grid.size())
			throw hpgl_exception("do_sequential_gausian_simulation_in_points", boost::format("Property size '%s' is not equal to grid size '%s'") % property.size() % grid.size());

		mt_random_generator_t gen;
		gen.seed(params.m_seed);
		path_random_generator_t path_gen(points_indexes.size(), params.m_seed);

		typedef precalculated_covariances_t<sugarbox_covariance_t, sugarbox_location_t> cov_t;
		cov_t pcov(covariance, params.m_radiuses);
		typedef /*indexed_*/ neighbour_lookup_t<grid_t, cov_t> nl_t;	
		nl_t neighbour_lookup(&grid, &pcov,	params);
		
		/*for (node_index_t node = 0; node < property.size(); ++node)
		{
			if (property.is_informed(node))
				neighbour_lookup.add_node(node);
		}*/
		
		progress_reporter_t report(points_indexes.size());	
		report.start();
		node_index_t node = points_indexes[path_gen.next()];
		//node_index_t node = path_gen.next();

		std::vector<sugarbox_location_t> coords;
		
		for (node_index_t counter = 0, counter_end = points_indexes.size(); counter < counter_end; ++counter, node = points_indexes[path_gen.next()])		
		//for (node_index_t counter = 0, counter_end = points_indexes.size(); counter < counter_end; ++counter, node = path_gen.next())		
		{
		//	std::cout << node << std::endl;
			if(property.is_informed(node)) 
				continue;

			std::vector<node_index_t> indices;
			std::vector<kriging_weight_t> weights;
			double variance;						
			sugarbox_location_t loc = grid[node];			

			cont_value_t mean;
			ki_result_t ki_result = kriging_interpolation(property, is_informed_predicate_t<cont_property_array_t>(property), node, pcov, mp, 
				neighbour_lookup, weight_calculator_sgs, mean, variance);			

			double value = ki_result == KI_SUCCESS 
				? sample(gen, Gaussian_cdf(mean, variance))
				: sample(gen, Gaussian_cdf());		
			
			property.set_at(node, value);
			//neighbour_lookup.add_node(node);
			report.next_lap();
		}
		report.stop();
		std::cout << "\nDone. Average speed: " << report.iterations_per_second() << " point/sec." << std::endl;
	}
}

#endif //SEQUENTIAL_SIMULATION_H_INCLUDED_DEU0932RMNPDVCPDZFIG03249MROPWEUFSDHVIAERT0934NTFMD0GN2304UNLKWEJOFDSZF0G32R
