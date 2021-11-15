#ifndef SEQUENTIAL_SIMULATION_H_INCLUDED_DEU0932RMNPDVCPDZFIG03249MROPWEUFSDHVIAERT0934NTFMD0GN2304UNLKWEJOFDSZF0G32R
#define SEQUENTIAL_SIMULATION_H_INCLUDED_DEU0932RMNPDVCPDZFIG03249MROPWEUFSDHVIAERT0934NTFMD0GN2304UNLKWEJOFDSZF0G32R

#include "cdf_utils.h"
#include "path_random_generator.h"
#include "bs_random_generator.h"
#include "typedefs.h"
#include "sample.h"
#include "covariance_field.h"
#include "progress_reporter.h"
#include "combiner.h"
#include "kriging_interpolation.h"
#include "precalculated_covariance.h"
#include "neighbourhood_lookup.h"
#include "is_informed_predicate.h"
#include "cov_model.h"

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

	template <typename grid_t, typename mean_provider_t, typename weight_calculator_t, typename mask_t>
	void do_sequential_gausian_simulation(
		cont_property_array_t & property,
		const grid_t & grid,
		const sgs_params_t & params,
		const mean_provider_t & mp,		
		const weight_calculator_t & weight_calculator_sgs,
		const mask_t & mask)
	{

		if (property.size() != grid.size())
			throw hpgl_exception("do_sequential_gausian_simulation", boost::format("Property size '%s' is not equal to grid size '%s'") % property.size() % grid.size());

		mt_random_generator_t gen;
		gen.seed(params.m_seed);
		path_random_generator_t path_gen(property.size(), params.m_seed);

		typedef precalculated_covariances_t cov_t;
		cov_t pcov(cov_model_t(params), params.m_radiuses);
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

			double value = ki_result == ki_result_t::KI_SUCCESS
				? sample(gen, gaussian_cdf_t(mean, variance))
				: sample(gen, gaussian_cdf_t());		
			
			property.set_at(node, value);
			//neighbour_lookup.add_node(node);
		}
		report.stop();
		write(boost::format("Done. Average speed: %1% point/sec.\n") % report.iterations_per_second());
	}

	template <typename grid_t, typename mean_provider_t, typename weight_calculator_t>
	void do_sequential_gausian_simulation_in_points(
		cont_property_array_t & property,
		const grid_t & grid,
		const sgs_params_t & params,
		const mean_provider_t & mp,		
		const weight_calculator_t & weight_calculator_sgs,
		const std::vector<int> & points_indexes)
	{
		if (property.size() != grid.size())
			throw hpgl_exception("do_sequential_gausian_simulation_in_points", boost::format("Property size '%s' is not equal to grid size '%s'") % property.size() % grid.size());

		mt_random_generator_t gen;
		gen.seed(params.m_seed);
		path_random_generator_t path_gen(points_indexes.size(), params.m_seed);

		typedef precalculated_covariances_t cov_t;
		//typedef precalculated_covariances_t<cov_model_t, sugarbox_location_t> cov_t;
		cov_t pcov(cov_model_t(params), params.m_radiuses);
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
				? sample(gen, gaussian_cdf_t(mean, variance))
				: sample(gen, gaussian_cdf_t());		
			
			property.set_at(node, value);
			//neighbour_lookup.add_node(node);
			report.next_lap();
		}
		report.stop();
		write(boost::format("Done. Average speed: %1% point/sec.") % report.iterations_per_second());
	}
}

#endif //SEQUENTIAL_SIMULATION_H_INCLUDED_DEU0932RMNPDVCPDZFIG03249MROPWEUFSDHVIAERT0934NTFMD0GN2304UNLKWEJOFDSZF0G32R
